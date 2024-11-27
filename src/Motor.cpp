#include "Motor.h"

Motor::Motor(uint8_t pwmPin, uint8_t dirPin, uint8_t encAPin, uint8_t encBPin, pcnt_unit_t pcntUnit)
    : pwmPin(pwmPin), dirPin(dirPin), encAPin(encAPin), encBPin(encBPin), pcntUnit(pcntUnit), encoderCount(0) {}

void Motor::begin()
{
    // Motor control pin setup
    pinMode(pwmPin, OUTPUT);
    pinMode(dirPin, OUTPUT);
    stop();

    // Setup PCNT for encoder
    setupPCNT();
}

void Motor::setupPCNT()
{
    pcnt_config_t pcntConfig;
    pcntConfig.pulse_gpio_num = encAPin;        // Channel A (pulse input)
    pcntConfig.ctrl_gpio_num = encBPin;         // Channel B (control input)
    pcntConfig.channel = PCNT_CHANNEL_0;        // Use PCNT channel 0
    pcntConfig.unit = pcntUnit;                 // PCNT unit
    pcntConfig.pos_mode = PCNT_COUNT_INC;       // Count up on rising edge
    pcntConfig.neg_mode = PCNT_COUNT_DEC;       // Count down on falling edge
    pcntConfig.lctrl_mode = PCNT_MODE_KEEP;     // Keep the primary count direction
    pcntConfig.hctrl_mode = PCNT_MODE_REVERSE;  // Reverse direction when CTRL is high
    pcntConfig.counter_h_lim = 32767;           // High limit for counter
    pcntConfig.counter_l_lim = -32768;          // Low limit for counter

    // Configure PCNT unit
    pcnt_unit_config(&pcntConfig);

    // Initialize PCNT counter
    pcnt_counter_pause(pcntUnit);
    pcnt_counter_clear(pcntUnit);
    pcnt_counter_resume(pcntUnit);
}

void Motor::setSpeed(int speed)
{
    speed = constrain(speed, -255, 255); // Ensure speed is within valid range

    if (speed > 0) {
        digitalWrite(dirPin, HIGH); // Forward direction
    } else if (speed < 0) {
        digitalWrite(dirPin, LOW);  // Reverse direction
        speed = -speed;            // Use absolute value for PWM
    } else {
        stop();
        return;
    }

    analogWrite(pwmPin, speed); // Set PWM duty cycle
}

void Motor::stop()
{
    analogWrite(pwmPin, 0); // Set PWM duty cycle to 0
}

int32_t Motor::getPosition()
{
    int16_t count;
    pcnt_get_counter_value(pcntUnit, &count); // Get current PCNT count
    encoderCount += count;                   // Accumulate total position
    pcnt_counter_clear(pcntUnit);            // Clear PCNT counter
    return encoderCount;
}

void Motor::resetPosition()
{
    encoderCount = 0;            // Reset accumulated position
    pcnt_counter_clear(pcntUnit); // Clear PCNT counter
}
