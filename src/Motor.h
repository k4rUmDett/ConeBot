#ifndef MOTOR_H
#define MOTOR_H

#include <Arduino.h>
#include <driver/pcnt.h>

/**
 * @brief Motor class to control motor speed, direction, and read encoder feedback.
 */
class Motor
{
public:
/**
     * @brief Constructs a Motor object.
     * 
     * @param pwmPin PWM pin used for motor speed control.
     * @param dirPin Direction pin to control motor direction.
     * @param encAPin Encoder A pin for position feedback.
     * @param encBPin Encoder B pin for position feedback.
     * @param pcntUnit PCNT unit used for encoder feedback.
     */
    Motor(uint8_t pwmPin, uint8_t dirPin, uint8_t encAPin, uint8_t encBPin, pcnt_unit_t pcntUnit);
 /**
     * @brief Initializes the motor by setting up control pins and configuring the encoder.
     */
    void begin();
/**
     * @brief Sets the speed of the motor.
     * 
     * @param speed Speed value ranging from -255 to 255. Positive values indicate forward direction,
     *              negative values indicate reverse direction, and zero stops the motor.
     */
    void setSpeed(int speed);   // Speed range: -255 to 255
/**
     * @brief Stops the motor.
     */
    void stop();                // Stop the motor
 /**
     * @brief Gets the current encoder position.
     * 
     * @return The accumulated encoder count representing the motor position.
     */
    int32_t getPosition();      // Get encoder position
/**
     * @brief Resets the encoder position to zero.
     */
    void resetPosition();       // Reset encoder position to zero

private:
    uint8_t pwmPin;             // PWM pin for speed control
    uint8_t dirPin;             // Direction pin
    uint8_t encAPin;            // Encoder A pin
    uint8_t encBPin;            // Encoder B pin
    pcnt_unit_t pcntUnit;       // PCNT unit for encoder
    volatile int32_t encoderCount; // Total encoder position

/**
     * @brief Configures the Pulse Counter (PCNT) unit for encoder feedback.
     */
    void setupPCNT();           // PCNT configuration for encoder
};

#endif
