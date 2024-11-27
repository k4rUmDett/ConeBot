#include "IMU.h"
#include <Arduino.h>

IMU::IMU(uint8_t address)
    : bno(55, address), address(address), calibrated(false), pitch(0), angularVelocity(0) {}

bool IMU::begin()
{
    // Initialize the BNO055 with the correct operation mode
    if (!bno.begin((adafruit_bno055_opmode_t)0x0C)) {  // Use the enum or its equivalent directly
        Serial.println("Error initializing BNO055!");
        return false;
    }
    delay(1000); // Allow sensor to stabilize
    bno.setExtCrystalUse(true);
    return true;
}

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

void IMU::update()
{
    if (!calibrated) {
        Serial.println("IMU not calibrated yet!");
        return;
    }
    computeState();
}

float IMU::getPitch()
{
    return pitch;
}

float IMU::getAngularVelocity()
{
    return angularVelocity;
}

bool IMU::isCalibrated() const
{
    return calibrated;
}

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

void IMU::computeState()
{
    // Get Euler angles (yaw, pitch, roll)
    imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);
    pitch = euler.z(); // Forward/backward tilt angle

    // Get angular velocity from gyroscope
    imu::Vector<3> gyro = bno.getVector(Adafruit_BNO055::VECTOR_GYROSCOPE);
    angularVelocity = gyro.y(); // Pitch rate of change
}
