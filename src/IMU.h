#ifndef IMU_H
#define IMU_H

#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BNO055.h>


class IMU
{
public:
    IMU(uint8_t address = BNO055_ADDRESS_A);

    bool begin();
    void calibrate();
    void update();

    float getPitch(); // Forward/backward tilt angle
    float getAngularVelocity(); // Angular velocity (pitch rate)

    bool isCalibrated() const;
    void printCalibrationStatus();

private:
    Adafruit_BNO055 bno;
    uint8_t address;
    bool calibrated;

    float pitch;              // Forward/backward tilt
    float angularVelocity;    // Angular velocity (pitch rate)

    void computeState();
};

#endif
