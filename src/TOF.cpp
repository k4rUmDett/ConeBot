#include "TOF.h"

TOF::TOF() {}

bool TOF::begin() 
{
    // Initialize the sensor with the default I2C address (0x29)
    if (sensor.InitSensor(0x29) != VL53L4CX_ERROR_NONE) {
        Serial.println("Failed to initialize VL53L4CX sensor.");
        return false;
    }

    // Configure the sensor
    sensor.VL53L4CX_SetDistanceMode(VL53L4CX_DISTANCEMODE_LONG);
    sensor.VL53L4CX_SetMeasurementTimingBudgetMicroSeconds(50000); // 50ms timing budget
    sensor.VL53L4CX_StartMeasurement(); // Start continuous measurement

    Serial.println("VL53L4CX sensor initialized successfully.");
    return true;
}

uint16_t TOF::getDistance()
{
    uint8_t isDataReady = 0;
    const TickType_t timeout = pdMS_TO_TICKS(100); // 100ms timeout
    TickType_t startTime = xTaskGetTickCount();

    // Wait until data is ready or timeout occurs
    while (!isDataReady) {
        sensor.VL53L4CX_GetMeasurementDataReady(&isDataReady);

        if (xTaskGetTickCount() - startTime > timeout) {
            Serial.println("Timeout waiting for measurement data.");
            return 0; // Timeout occurred
        }

        vTaskDelay(pdMS_TO_TICKS(5)); // Yield to other tasks
    }

    // Fetch the multi-ranging data
    if (sensor.VL53L4CX_GetMultiRangingData(&multiRangingData) != VL53L4CX_ERROR_NONE) {
        Serial.println("Failed to fetch ranging data.");
        return 0;
    }

    // Clear interrupt and prepare for the next measurement
    sensor.VL53L4CX_ClearInterruptAndStartMeasurement();

    // Return the distance to the first target
    if (multiRangingData.NumberOfObjectsFound > 0) {
        return multiRangingData.RangeData[0].RangeMilliMeter; // First target distance
    } else {
        Serial.println("No target detected.");
        return 0;
    }
}