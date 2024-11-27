#ifndef TOF_H
#define TOF_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_VL53L4CX.h> // Include the Adafruit VL53L4CX library

class TOF {
public:
    TOF(TwoWire &wire = Wire, uint8_t i2cAddress = 0x29);

    bool begin();                  // Initialize the sensor
    uint16_t getDistance();        // Get the measured distance in mm
    void setTimeout(uint16_t timeout); // Set the I2C communication timeout
    bool isReady();                // Check if the sensor is ready for measurement

private:
    Adafruit_VL53L4CX sensor;      // Adafruit VL53L4CX instance
    TwoWire &i2cWire;              // I2C interface
    uint8_t i2cAddress;            // I2C address
    uint16_t timeout;              // Timeout for I2C communication
};

#endif
