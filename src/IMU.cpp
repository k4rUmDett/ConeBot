#include "IMU.h"
#include <Arduino.h>

/**
 * @brief Constructor for the IMU class.
 * @param address The I2C address of the BNO055 sensor.
 */
IMU::IMU(uint8_t address)
    : bno(55, address), address(address), calibrated(false), pitch(0), angularVelocity(0) {}

/**
 * @brief Initializes the BNO055 sensor.
 * @return True if initialization is successful, false otherwise.
 */
bool IMU::begin()
{
    if (!bno.begin((adafruit_bno055_opmode_t)0x0C)) {
        Serial.println("Error initializing BNO055!");
        return false;
    }
    delay(1000);
    bno.setExtCrystalUse(true);
    return true;
}

/**
 * @brief Calibrates the IMU sensor by ensuring all systems are fully calibrated.
 */
void IMU::calibrate()
{
    Serial.println("Calibrating IMU...");
    uint8_t sys, gyro, accel, mag;
    do {
        bno.getCalibration(&sys, &gyro, &accel, &mag);
        Serial.print("Sys:");
        Serial.print(sys, DEC);
        Serial.print(" Gyro:");
        Serial.print(gyro, DEC);
        Serial.print(" Accel:");
        Serial.print(accel, DEC);
        Serial.print(" Mag:");
        Serial.println(mag, DEC);
        delay(100);
    } while (sys < 3 || gyro < 3 || accel < 3 || mag < 3);
    calibrated = true;
    Serial.println("IMU Calibrated!");
}

/**
 * @brief Updates the sensor's state, including pitch and angular velocity.
 */
void IMU::update()
{
    if (!calibrated) {
        Serial.println("IMU not calibrated yet!");
        return;
    }
    computeState();
}

/**
 * @brief Gets the current pitch (tilt angle) of the sensor.
 * @return The pitch in degrees.
 */
float IMU::getPitch()
{
    return pitch;
}

/**
 * @brief Gets the angular velocity of the sensor (pitch rate).
 * @return The angular velocity in radians per second.
 */
float IMU::getAngularVelocity()
{
    return angularVelocity;
}

/**
 * @brief Checks if the sensor is fully calibrated.
 * @return True if calibrated, false otherwise.
 */
bool IMU::isCalibrated() const
{
    return calibrated;
}

/**
 * @brief Prints the current calibration status to the serial monitor.
 */
void IMU::printCalibrationStatus()
{
    uint8_t sys, gyro, accel, mag;
    bno.getCalibration(&sys, &gyro, &accel, &mag);
    Serial.print("Sys:");
    Serial.print(sys, DEC);
    Serial.print(" Gyro:");
    Serial.print(gyro, DEC);
    Serial.print(" Accel:");
    Serial.print(accel, DEC);
    Serial.print(" Mag:");
    Serial.println(mag, DEC);
}

/**
 * @brief Computes the current state of the sensor, including pitch and angular velocity.
 */
void IMU::computeState()
{
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    pitch = euler.z();

    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    angularVelocity = gyro.y();
}
