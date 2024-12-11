#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BNO055.h>

/**
 * @class IMU
 * @brief A class to interface with the Adafruit BNO055 IMU sensor.
 * 
 * The IMU class provides methods to initialize, calibrate, and update the sensor's state,
 * and to retrieve its tilt angle and angular velocity. It supports calibration status checks
 * and prints the calibration data for debugging purposes.
 */
class IMU
{
public:
    /**
     * @brief Constructor for the IMU class.
     * @param address The I2C address of the BNO055 sensor (default: BNO055_ADDRESS_A).
     */
    IMU(uint8_t address = BNO055_ADDRESS_A);

    /**
     * @brief Initializes the BNO055 sensor.
     * @return True if initialization is successful, false otherwise.
     */
    bool begin();

    /**
     * @brief Calibrates the IMU sensor by ensuring all systems are fully calibrated.
     */
    void calibrate();

    /**
     * @brief Updates the sensor's state, including pitch and angular velocity.
     * 
     * This method reads sensor data to compute the current pitch and angular velocity.
     */
    void update();

    /**
     * @brief Gets the current pitch (tilt angle) of the sensor.
     * @return The pitch in degrees.
     */
    float getPitch();

    /**
     * @brief Gets the angular velocity of the sensor (pitch rate).
     * @return The angular velocity in radians per second.
     */
    float getAngularVelocity();

    /**
     * @brief Checks if the sensor is fully calibrated.
     * @return True if calibrated, false otherwise.
     */
    bool isCalibrated() const;

    /**
     * @brief Prints the current calibration status to the serial monitor.
     */
    void printCalibrationStatus();

private:
    Adafruit_BNO055 bno;      /**< Instance of the Adafruit BNO055 library. */
    uint8_t address;          /**< I2C address of the BNO055 sensor. */
    bool calibrated;          /**< Calibration status of the sensor. */

    float pitch;              /**< Current forward/backward tilt angle. */
    float angularVelocity;    /**< Current pitch rate of change. */

    /**
     * @brief Computes the current state of the sensor, including pitch and angular velocity.
     */
    void computeState();
};

#endif
