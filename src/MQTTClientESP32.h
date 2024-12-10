/** @file MQTTClientESP32.h
 *  @brief Header file defining the MQTTClientESP32 class and related structures for MQTT communication.
 */

#ifndef MQTT_CLIENT_ESP32_H
#define MQTT_CLIENT_ESP32_H

#include <Arduino.h>
#include "PrintStream.h"
#include <WiFi.h>
#include <PubSubClient.h>
#include "taskshare.h"

/**
 *  @brief Structure representing the bot's position and tilt angle.
 */
typedef struct {
    float position;
    float tilt_angle;
} BotState;

/**
 *  @brief Extern variable to store the bot's current state.
 */

extern Share<BotState> botState;

/**
 *  @class MQTTClientESP32
 *  @brief Handles MQTT communication on an ESP32 device.
 */
class MQTTClientESP32 {
private:
    const char* ssid;
    const char* password;
    const char* mqtt_server;
    const uint16_t mqtt_port;
    bool isHotspot;                 

    IPAddress local_ip;            
    IPAddress gateway;
    IPAddress subnet;

    WiFiClient espClient;
    PubSubClient client;

    String lastReceivedTopic;
    String lastReceivedMessage;

    /**
     *  @brief Configures WiFi connection based on the mode (hotspot or client).
     */
    void setupWifi();

    /**
     *  @brief Reconnects to the MQTT server if the connection is lost.
     */
    void reconnect();

    /**
     *  @brief Static callback for handling MQTT messages.
     *  @param topic The topic of the received message.
     *  @param message The message payload.
     *  @param length The length of the message payload.
     */
    static void staticCallback(char* topic, byte* message, uint16_t length);

    /**
     *  @brief Instance-specific handler for MQTT messages.
     *  @param topic The topic of the received message.
     *  @param message The message payload.
     *  @param length The length of the message payload.
     */
    void callback(char* topic, byte* message, uint16_t length);

public:
    /**
     *  @brief Constructor for initializing the MQTT client.
     *  @param ssid WiFi SSID.
     *  @param password WiFi password.
     *  @param mqtt_server MQTT server address.
     *  @param mqtt_port MQTT server port.
     *  @param isHotspot Flag to determine if the ESP32 is acting as a hotspot.
     *  @param local_ip IP address when acting as a hotspot.
     *  @param gateway Gateway IP address.
     *  @param subnet Subnet mask.
     */
    MQTTClientESP32(const char* ssid, const char* password, const char* mqtt_server, uint16_t mqtt_port, bool isHotspot = false,
                    IPAddress local_ip = IPAddress(192, 168, 5, 1), IPAddress gateway = IPAddress(192, 168, 5, 1), IPAddress subnet = IPAddress(255, 255, 255, 0));

    /**
     *  @brief Initializes the MQTT client and establishes WiFi connection.
     */
    void begin();

    /**
     *  @brief Main MQTT loop for publishing and receiving messages.
     */
    void mqttLoop();

    /**
     *  @brief Gets the last received MQTT topic.
     *  @return Last received topic as a string.
     */
    String getLastReceivedTopic() const;

    /**
     *  @brief Gets the last received MQTT message.
     *  @return Last received message as a string.
     */
    String getLastReceivedMessage() const;
};

#endif
