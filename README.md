# Smart AIoT Fan System

A multi-node, AI-powered smart fan system built with the ESP32 ecosystem and Computer Vision. The system features real-time person-tracking using YOLOv8-Pose, environment-aware speed control using TinyML, and a cloud-integrated dashboard for remote management.

## 🚀 Overview

This project implements an intelligent cooling solution that not only adjusts fan speed based on ambient conditions but also physically tracks the user's position with zero-latency computer vision. It utilizes a highly distributed architecture involving a PC for heavy AI inference and three specialized ESP32 nodes to balance sensor fusion, motor control, and network communications.

## 🛠 Hardware Architecture

The system is divided into one PC host and three specialized microcontrollers to eliminate bottlenecks and ensure ultra-smooth operation:

1.  **Vision & Gateway Node (ESP32-CAM):**
    * Captures raw frames and transmits image data to the PC via high-speed **UART (921600 bps)** using a Ping-Pong protocol.
    * Acts as a bridge, receiving AI decisions from the PC and instantly broadcasting them to the Stepper Node via **ESP-NOW**.

2.  **AI Inference Host (PC / Laptop):**
    * Runs a Python script utilizing **OpenCV** and **Ultralytics YOLOv8-Pose**.
    * Tracks the user's nose keypoint to dynamically classify the target into 3 zones: **Left (40%), Center (20%), and Right (40%)**.
    * Sends directional commands back to the ESP32-CAM.

3.  **Tracking Actuator Node (Dedicated ESP32):**
    * Operates purely on **ESP-NOW** (no Wi-Fi Router connection) to guarantee millisecond latency.
    * Controls a **28BYJ-48 Stepper Motor** using the `AccelStepper` library (Velocity Control Mode) for buttery-smooth panning.
    * Features Software Limits, a Hardware Watchdog (Anti-lag), and a 3-second grace period Auto-Sweep algorithm.

4.  **Main Environment Node (ESP32):**
    * **TinyML (Regression):** Processes **DHT22** sensor data to predict and set the optimal DC fan speed.
    * **Connectivity:** Connects to **Adafruit IO** via **MQTT** and hosts a local **WebServer** for dashboard control.

## 🧠 Smart Features & Modes

The fan operates in three distinct climate modes, selectable via the Adafruit Dashboard or Local WebServer:

* **Manual Low:** Constant low-speed operation.
* **Manual High:** Maximum cooling performance.
* **Auto Mode (TinyML):** A Neural Network model predicts the ideal fan speed based on real-time Temperature and Humidity inputs from the DHT22 sensor.

### 🎯 Advanced Person Tracking
The tracking mechanism is completely isolated from the main WebServer to prevent motor jitter. It includes:
* **Keypoint Tracking:** Uses YOLOv8 Pose estimation to track the nose, solving the "bounding box" issue when users are close to the camera.
* **Deadzone Optimization:** A 4-2-4 screen split ratio prevents the fan from making micro-adjustments when the user is relatively centered.
* **Anti-Lag & Auto-Sweep:** If the AI loses the target (or experiences network lag), the fan holds its position for 3 seconds. If the user doesn't return, it automatically initiates a slow sweeping motion to search for people.
* **Software Limits:** Prevents wire tangling by restricting the stepper motor within a safe [-512, 512] step boundary.

## 💻 Software Stack

* **Development Environment:** PlatformIO (C++) & PyCharm/VSCode (Python)
* **Framework:** Arduino with **FreeRTOS** (Task management via Queues and Mutexes)
* **AI/Computer Vision:** TensorFlow Lite for Microcontrollers (TinyML), YOLOv8-Pose, OpenCV
* **Protocols:** ESP-NOW (Ultra-low latency Inter-board), MQTT (Adafruit IO), High-speed Serial/UART
* **Libraries:** `AccelStepper`, `esp_camera`, `WiFi`, `PubSubClient`
