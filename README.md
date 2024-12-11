# ConeBot Software

## Overview

ConeBot is a balancing robot designed to navigate and interact with its environment. The software integrates motor control, sensor feedback, and MQTT communication, making it suitable for real-time control and monitoring applications.

This repository contains the software implementation for ConeBot, including features such as:
- Real-time motor control using FreeRTOS tasks.
- Sensor data acquisition and processing (IMU, TOF, GPS).
- Remote communication and control using MQTT.

**Note:** The implementation does not include a controller, such as a PI or PID controller, to calculate motor commands for maintaining balance. This should be added by the user for the robot to function as a balancing bot.

---

## Features

### 1. **Motor Control**
- ConeBot uses two motors for movement and balance.
- The control logic is based on a Finite State Machine (FSM) with states such as:
  - `IDLE`: Robot is stationary.
  - `MOVING_FORWARD`: Robot moves forward.
  - `MOVING_BACKWARD`: Robot moves backward.
  - `CORRECTING_TILT`: Robot adjusts its tilt to maintain balance.
  - `AVOIDING_OBSTACLE`: Robot stops or changes direction upon detecting obstacles.
  - `STOPPED`: Robot is halted after completing its task or encountering critical conditions.

**User Note:** The FSM uses placeholder commands for motor speeds. For balance and precise movement, you must implement a feedback controller, such as a PI controller, to compute motor commands dynamically based on tilt angle and other feedback variables.

### 2. **Sensor Integration**
- **IMU (Inertial Measurement Unit):** Measures the tilt angle of the robot for balancing.
- **TOF (Time of Flight Sensor):** Detects obstacles in the robot’s path.
- **GPS (Global Positioning System):** Tracks the robot's position for navigation.

### 3. **MQTT Communication**
- ConeBot communicates with a remote server or dashboard using the MQTT protocol.
- Topics:
  - **State Telemetry:** Publishes real-time state updates (position, angle, obstacle status).
  - **Control Commands:** Subscribes to control commands for dynamic behavior adjustment.

---

## Software Components

1. **FreeRTOS Tasks:**
   - `motorControlTask`: Manages motor speed and direction based on FSM states.
   - `measurementTask`: Collects data from sensors and updates shared variables.
   - `mqttTask`: Handles MQTT communication, including publishing sensor data and subscribing to control topics.

2. **Shared Variables:**
   - `botState`: Stores the robot's current position and tilt angle.
   - `measurement`: Holds raw sensor data.
   - `obstacleDetected`: Boolean flag indicating whether an obstacle is detected.

3. **Finite State Machine (FSM):**
   - Implements the robot's behavioral logic based on sensor inputs and control commands.

4. **Feedback Controller (Not Implemented):**
   - The software does not include a controller to calculate motor commands for balancing.
   - Users should implement a feedback controller, such as a PI or PID controller, to:
     - Calculate motor speeds dynamically based on tilt angle and position.
     - Maintain balance and stability.

---

## Hardware Requirements

1. **Microcontroller:**
   - ESP32 for running FreeRTOS and handling MQTT communication.

2. **Sensors:**
   - IMU for tilt measurement.
   - TOF sensor for obstacle detection.
   - GPS module for position tracking.

3. **Motors:**
   - Two DC motors with appropriate drivers.

---

## Installation

### Prerequisites
- Arduino IDE or PlatformIO.
- Mosquitto MQTT broker or any compatible MQTT server.
- Wi-Fi access point for ESP32 connection.
