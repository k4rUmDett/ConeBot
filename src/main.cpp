/**
 * @file main.cpp
 * @brief A program to control a robotic system using MQTT, FreeRTOS, and sensor feedback.
 *
 * This program integrates motor control, IMU, TOF, GPS, and MQTT functionality into a single
 * system managed by FreeRTOS tasks. The system reacts to real-time sensor data and uses MQTT
 * for communication.
 *
 * @details The system's functionality includes:
 * - Controlling motors based on FSM states.
 * - Monitoring sensor data (IMU, TOF, GPS) for decision-making.
 * - Publishing and subscribing to MQTT topics for remote control and telemetry.
 *
 * Acknowledgment: This program's design and implementation were assisted by OpenAI's ChatGPT.
 */

#include <Arduino.h>
#include "Motor.h"
#include "IMU.h"
#include "GPS.h"
#include "TOF.h"
#include "MQTTClientESP32.h"

// Object instantiation
Motor motorLeft(25, 26, 34, 35, PCNT_UNIT_0);
Motor motorRight(27, 14, 36, 39, PCNT_UNIT_1);
IMU imuSensor(0x28);
TOF tofSensor;
GPS gpsSensor(Serial2, 9600);
MQTTClientESP32 mqttClient("SpectrumSetup-C8", "Pong_pang2499", "192.168.1.37", 1883);

/**
 * @brief Structure to store sensor measurements.
 */
typedef struct {
    float position;      /**< Robot's position. */
    float angle;         /**< Robot's tilt angle. */
    int32_t latitude;    /**< Latitude from GPS. */
    int32_t longtitude;  /**< Longitude from GPS. */
    uint32_t attitude;   /**< Placeholder for additional attitude data. */
    uint16_t distance;   /**< Distance from TOF sensor. */
} Measurement;

// Shared variables
Share<BotState> botState; /**< Shared variable for robot's state. */
Share<Measurement> measurement; /**< Shared variable for sensor measurements. */
Share<bool> obstacleDetected; /**< Shared variable for obstacle detection. */

// FSM States
/**
 * @brief Finite State Machine (FSM) states for the robot.
 */
enum ConeBotState {
    IDLE,               /**< Robot is idle. */
    CORRECTING_TILT,    /**< Robot is correcting tilt. */
    MOVING_FORWARD,     /**< Robot is moving forward. */
    MOVING_BACKWARD,    /**< Robot is moving backward. */
};

// Function prototypes
void motorControlTask(void *parameter);
void measurementTask(void *parameter);
void mqttTask(void *parameter);

void setup() {
    Serial.begin(115200);

    // Start FreeRTOS tasks
    xTaskCreate(motorControlTask, "MotorControlTask", 2048, NULL, 1, NULL);
    xTaskCreate(measurementTask, "MeasurementTask", 4096, NULL, 1, NULL);
    xTaskCreate(mqttTask, "MQTTTask", 4096, NULL, 1, NULL);
}

void loop() {
    // The main loop is intentionally left empty; FreeRTOS manages tasks
}

/**
 * @brief Motor control task to drive the robot forward or backward based on the FSM state.
 * 
 * @param parameter FreeRTOS task parameter (unused).
 */
void motorControlTask(void *parameter) {
    motorLeft.begin();
    motorRight.begin();
    ConeBotState currentState = IDLE;
    while (1) {
        switch (currentState) {
            case IDLE:
                motorLeft.stop();
                motorRight.stop();
                break;

            case MOVING_FORWARD:
                if (obstacleDetected.get()) {
                    currentState = IDLE;
                } else {
                    motorLeft.setSpeed(255);
                    motorRight.setSpeed(255);
                }
                break;

            case MOVING_BACKWARD:
                if (obstacleDetected.get()) {
                    currentState = IDLE;
                }
                else {
                    motorLeft.setSpeed(-256);
                    motorRight.setSpeed(-256);
                }
                break;

            case CORRECTING_TILT:
                if (abs(measurement.get().angle) < 5.0) {
                    motorLeft.setSpeed(50);
                    motorRight.setSpeed(50);
                }
                break;
        }
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief Measurement task to handle IMU, TOF, and GPS sensor updates.
 * 
 * @param parameter FreeRTOS task parameter (unused).
 */
void measurementTask(void *parameter) {
    Measurement local_measurement;
    obstacleDetected.put(false);
    if (!imuSensor.begin()) {
        Serial.println("Failed to initialize IMU");
    }
    if (!tofSensor.begin()) {
        Serial.println("Failed to initialize TOF sensor");
    }
    gpsSensor.begin();
    while (1) {
        local_measurement = measurement.get();
        // IMU Update
        imuSensor.update();
        local_measurement.angle = imuSensor.getPitch();

        // TOF Update
        uint16_t distance = tofSensor.getDistance();
        obstacleDetected.put(distance > 0 && distance < 10);

        // Update the shared state
        BotState local_state = botState.get();
        local_state.position = gpsSensor.getLatitude().toFloat();
        local_state.tilt_angle = local_measurement.angle;
        botState.put(local_state);

        // GPS Update
        gpsSensor.update();

        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

/**
 * @brief MQTT task to handle publishing and subscribing to topics.
 * 
 * @param parameter FreeRTOS task parameter (unused).
 */
void mqttTask(void *parameter) {
    mqttClient.begin();
    while (1) {
        mqttClient.mqttLoop();
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}