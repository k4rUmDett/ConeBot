#ifndef TOF_H
#define TOF_H

#include <Arduino.h>
#include <vl53l4cx_class.h> // Include STM32Duino VL53L4CX library

/**
 * @class TOF
 * @brief A class for interfacing with the VL53L4CX Time-of-Flight (TOF) sensor.
 * 
 * The TOF class provides methods to initialize the sensor and measure distances.
 * It uses the STM32Duino VL53L4CX library for communication and configuration.
 */
class TOF
{
public:
    /**
     * @brief Constructor for the TOF class.
     * 
     * Initializes the class instance but does not start the sensor.
     */
    TOF();

    /**
     * @brief Initializes the VL53L4CX sensor with default settings.
     * 
     * Configures the sensor to use long-distance mode and sets the measurement timing budget.
     * 
     * @return True if initialization is successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Retrieves the measured distance from the TOF sensor.
     * 
     * This method waits for measurement data to be ready and fetches the distance
     * to the closest detected target in millimeters.
     * 
     * @return The distance to the first detected target in millimeters, or 0 if an error occurs.
     */
    uint16_t getDistance();

private:
    VL53L4CX sensor; /**< Instance of the VL53L4CX sensor. */
    VL53L4CX_MultiRangingData_t multiRangingData; /**< Structure to store ranging results. */
};

#endif
