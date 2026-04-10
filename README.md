# Smart AIoT Fan System

A multi-node, AI-powered smart fan system built with the ESP32 ecosystem. The system features person-tracking, environment-aware speed control using TinyML, and a cloud-integrated dashboard for remote management.

## 🚀 Overview

This project implements an intelligent cooling solution that not only adjusts speed based on ambient conditions but also physically tracks the user's position using computer vision. It utilizes a distributed architecture with three ESP32 nodes to balance the computational load of AI inference, sensor fusion, and motor control.

## 🛠 Hardware Architecture

The system is divided into three specialized nodes:

1.  **Vision Node (ESP32-CAM):** Captures raw frames and transmits image data via **UART**.
2.  **AI Inference Node (ESP32-S3):** * Runs **TensorFlow Lite for Microcontrollers (TFLite)**.
    * Processes images to detect human presence in 3 zones: **Left, Center, and Right**.
    * Transmits directional commands to the Main Controller via **ESP-NOW**.
3.  **Main Controller Node (ESP32-S3):**
    * **TinyML (Regression):** Processes **DHT20** sensor data to determine optimal fan speed.
    * **Actuation:** Controls the **28BYJ-48 Stepper Motor** for oscillation and a DC fan motor.
    * **Connectivity:** Connects to **Adafruit IO** via **MQTT** for real-time dashboard control.
    * **HMI:** Displays system status on an **LCD screen**.

## 🧠 Smart Features & Modes

The fan operates in three distinct modes, selectable via the Adafruit Dashboard:

* **Manual Low:** Constant low-speed operation.
* **Manual High:** Maximum cooling performance.
* **Auto Mode (TinyML):** A Neural Network model predicts the ideal fan speed based on Temperature and Humidity inputs from the DHT20 sensor.

### Person Tracking
Using the AI Inference Node, the fan automatically adjusts its orientation. The camera frame is divided into three segments. If a person is detected in the "Left" or "Right" zone, the stepper motor rotates the fan head to center the user.

## 💻 Software Stack

* **Development Environment:** PlatformIO
* **Framework:** Arduino with **FreeRTOS**
* **AI/ML:** TensorFlow Lite for Microcontrollers (TinyML)
* **Protocols:** MQTT (Adafruit IO), ESP-NOW (Inter-board), UART (Serial communication)
* **Task Management:** Multi-tasking handled via FreeRTOS (Tasks for Sensor Reading, AI Data Handling, Motor Control, and Cloud Sync).
