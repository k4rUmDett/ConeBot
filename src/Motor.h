#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <driver/pcnt.h>

class Motor
{
    public:
        Motor(uint8_t pwmPin, uint8_t dirPin, uint8_t encAPin, uint8_t encBPin, pcnt_unit_t pcntUnit);
        void begin();
        void setSpeed(int speed);   // Speed range: -255 to 255
        void stop();                // Stop the motor
        int32_t getPosition();      // Get encoder position
        void resetPosition();       // Reset encoder position to zero

    private:
        uint8_t pwmPin;             // PWM pin for speed control
        uint8_t dirPin;             // Direction pin
        uint8_t encAPin;            // Encoder A pin
        uint8_t encBPin;            // Encoder B pin
        pcnt_unit_t pcntUnit;       // PCNT unit for encoder
        volatile int32_t encoderCount; // Total encoder position

        void setupPCNT();           // PCNT configuration for encoder
};

#endif
