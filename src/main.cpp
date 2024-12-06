#include <Arduino.h>
#include "Motor.h"
#include "IMU.h"
#include "GPS.h"
#include "TOF.h"

void tofTask(void *parameter);
void motorControlTask(void *parameter);
void gpsTask(void *parameter);
void imuTask(void *parameter);

// Object instantiation based on the provided classes
Motor motorLeft(25, 26, 34, 35, PCNT_UNIT_0);  // Example pins for left motor
Motor motorRight(27, 14, 36, 39, PCNT_UNIT_1); // Example pins for right motor
IMU imuSensor(0x28);
TOF tofSensor;
GPS gpsSensor(Serial2, 9600);

// Shared variables
bool obstacleDetected = false;
float currentPitch = 0.0;
int motorSpeed = 150;  // Example motor speed

// FSM States
enum ConeBotState {
    IDLE,
    CORRECTING_TILT,
    MOVING_FORWARD,
    MOVING_BACKWARD,
    AVOIDING_OBSTACLE,
    STOPPED
};

ConeBotState currentState = IDLE; // Initial state


void setup() {
    Serial.begin(115200);

    // Initialize components
    motorLeft.begin();
    motorRight.begin();
    if (!imuSensor.begin()) {
        Serial.println("Failed to initialize IMU");
    }
    if (!tofSensor.begin()) {
        Serial.println("Failed to initialize TOF sensor");
    }
    gpsSensor.begin();

    // Start FreeRTOS tasks
    xTaskCreatePinnedToCore(motorControlTask, "MotorControlTask", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(imuTask, "IMUTask", 2048, NULL, 1, NULL, 0);
    xTaskCreatePinnedToCore(tofTask, "TOFTask", 2048, NULL, 1, NULL, 1);
    xTaskCreatePinnedToCore(gpsTask, "GPSTask", 2048, NULL, 1, NULL, 1);
}

void loop() {
    // The main loop is intentionally left empty; FreeRTOS manages tasks
}


/**
 * @brief Motor control task to drive the robot forward or backward.
 */
void motorControlTask(void *parameter) {
    while (1) {
        switch (currentState) {
            case IDLE:
                // Stop the motors
                motorLeft.stop();
                motorRight.stop();
                break;

            case MOVING_FORWARD:
                if (obstacleDetected) {
                    currentState = AVOIDING_OBSTACLE;
                } else {
                    motorLeft.setSpeed(motorSpeed);
                    motorRight.setSpeed(motorSpeed);
                }
                break;

            case MOVING_BACKWARD:
                motorLeft.setSpeed(-motorSpeed);
                motorRight.setSpeed(-motorSpeed);
                break;

            case AVOIDING_OBSTACLE:
                // Simple example: Stop and go to STOPPED state
                motorLeft.stop();
                motorRight.stop();
                currentState = STOPPED;
                break;

            case STOPPED:
                // Motors are stopped
                motorLeft.stop();
                motorRight.stop();
                // Could transition to IDLE after a condition is met, e.g., a restart command
                break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}



/**
 * @brief IMU task to monitor the orientation of the robot.
 */
void imuTask(void *parameter) {
    while (1) {
        imuSensor.update();
        currentPitch = imuSensor.getPitch();

        // If the pitch exceeds a threshold, switch to CORRECTING_TILT state
        if (abs(currentPitch) > 15.0 && currentState == MOVING_FORWARD) {
            currentState = CORRECTING_TILT;
        }

        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief TOF sensor task to detect obstacles and update the state.
 */
void tofTask(void *parameter) {
    while (1) {
        uint16_t distance = tofSensor.getDistance();

        // Update the obstacle detection state
        if (distance > 0 && distance < 300) { // Example threshold distance in mm
            obstacleDetected = true;
            if (currentState == MOVING_FORWARD) {
                currentState = AVOIDING_OBSTACLE;
            }
        } else {
            obstacleDetected = false;
        }

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief GPS task to track the current location of the robot.
 */
void gpsTask(void *parameter) {
    while (1) {
        if (gpsSensor.update()) {
            String latitude = gpsSensor.getLatitude();
            String longitude = gpsSensor.getLongitude();
            String utcTime = gpsSensor.getUTC();

            // For debugging, print GPS info to Serial Monitor
            Serial.print("Latitude: ");
            Serial.println(latitude);
            Serial.print("Longitude: ");
            Serial.println(longitude);
            Serial.print("UTC Time: ");
            Serial.println(utcTime);
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}
