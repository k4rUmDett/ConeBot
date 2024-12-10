/** @file MQTTClientESP32.cpp
 *  @brief Implementation file for the MQTTClientESP32 class.
 */

#include "MQTTClientESP32.h"

/**
 *  @brief Extern variable definition for the bot's current state.
 */
BotState botState = {0.0, 0.0};

void MQTTClientESP32::setupWifi() {
    if (isHotspot) {
        Serial << "Setting up WiFi access point...";
        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(local_ip, gateway, subnet);
        WiFi.softAP(ssid, password);
        Serial << "done." << endl;
    } else {
        Serial << "Connecting to " << ssid;
        WiFi.begin(ssid, password);
        while (WiFi.status() != WL_CONNECTED) {
            vTaskDelay(1000);
            Serial.print(".");
        }
        Serial << "connected at IP address " << WiFi.localIP() << endl;
    }
}

void MQTTClientESP32::reconnect() {
    while (!client.connected()) {
        Serial << "Connecting to MQTT..." << endl;

        if (client.connect("ESP32_Client")) {
            Serial << "connected." << endl;
            client.subscribe("esp32/output");
        } else {
            Serial << "failed, rc = " << client.state() << endl;
            if (!isHotspot) {
                setupWifi();
            }
            delay(5000);
        }
    }
}

void MQTTClientESP32::callback(char* topic, byte* message, uint16_t length) {
    lastReceivedTopic = String(topic);
    lastReceivedMessage = String((char*)message, length);

    Serial << "MQTT received topic \"" << lastReceivedTopic << "\", message \"" << lastReceivedMessage << "\"" << endl;

    if (lastReceivedTopic == "esp32/output") {
        if (lastReceivedMessage == "command1") {
            Serial << "Executing Command 1" << endl;
        } else if (lastReceivedMessage == "command2") {
            Serial << "Executing Command 2" << endl;
        }
    }
}

MQTTClientESP32::MQTTClientESP32(const char* ssid, const char* password, const char* mqtt_server, uint16_t mqtt_port, bool isHotspot,
                                 IPAddress local_ip, IPAddress gateway, IPAddress subnet)
    : ssid(ssid), password(password), mqtt_server(mqtt_server), mqtt_port(mqtt_port), isHotspot(isHotspot),
      local_ip(local_ip), gateway(gateway), subnet(subnet), client(espClient) {}

void MQTTClientESP32::begin() {
    Serial.begin(115200);
    delay(100);
    while (!Serial) {
        ;
    }
    delay(1000);
    Serial << endl << F("\033[2JTesting Arduino MQTT") << endl;

    setupWifi();

    // Set up the callback using a lambda function
    client.setServer(mqtt_server, mqtt_port);
    client.setCallback([this](char* topic, byte* message, unsigned int length) {
        this->callback(topic, message, length);
    });
}

void MQTTClientESP32::mqttLoop() {
    for (;;) {
        if (!client.loop()) {
            reconnect();
        }

        char msg_string[64];
        sprintf(msg_string, "Position: %.2f, Tilt Angle: %.2f", botState.position, botState.tilt_angle);
        client.publish("bot/state", msg_string);

        vTaskDelay(1000);
    }
}

String MQTTClientESP32::getLastReceivedTopic() const {
    return lastReceivedTopic;
}

String MQTTClientESP32::getLastReceivedMessage() const {
    return lastReceivedMessage;
}