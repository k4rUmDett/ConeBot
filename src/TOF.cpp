#include "TOF.h"

TOF::TOF(TwoWire &wire, uint8_t i2cAddress)
    : i2cWire(wire), i2cAddress(i2cAddress), timeout(500) {}

bool TOF::begin() {
    // Initialize I2C
    i2cWire.begin(21, 22); // SDA = GPIO21, SCL = GPIO22 for DFR0478

    // Initialize the sensor
    if (!sensor.begin(i2cAddress, &i2cWire)) {
        Serial.println("Failed to initialize VL53L4CX sensor.");
        return false;
    }

    Serial.println("VL53L4CX sensor initialized successfully.");
    return true;
}

uint16_t TOF::getDistance() {
    if (!sensor.dataReady()) {
        Serial.println("Sensor data not ready.");
        return 0; // Return 0 if the sensor is not ready
    }

    VL53L4CX_MultiRangingData_t data;
    if (sensor.rangingTest(&data)) {
        if (data.RangeStatus == VL53L4CX_RANGE_STATUS_NONE) {
            return data.RangeMilliMeter; // Valid range in mm
        } else {
            Serial.println("Error: Invalid range status.");
            return 0; // Return 0 for invalid range
        }
    } else {
        Serial.println("Error: Failed to read ranging data.");
        return 0; // Return 0 for read failure
    }
}

void TOF::setTimeout(uint16_t timeout) {
    this->timeout = timeout;
    sensor.setTimeout(timeout);
}

bool TOF::isReady() {
    return sensor.dataReady();
}
