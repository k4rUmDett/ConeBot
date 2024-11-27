#ifndef TOF_H
#define TOF_H

#include <Arduino.h>
#include <vl53l4cx_class.h> // Include STM32Duino VL53L4CX library

class TOF {
public:
    TOF();                          // Constructor
    bool begin();                   // Initialize the sensor
    uint16_t getDistance();         // Get the measured distance in mm

private:
    VL53L4CX sensor;                // VL53L4CX sensor instance
    VL53L4CX_MultiRangingData_t multiRangingData; // Structure to store ranging results
};

#endif
