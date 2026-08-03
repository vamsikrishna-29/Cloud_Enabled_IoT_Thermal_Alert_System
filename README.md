# ☁️ Cloud-Enabled IoT 🌡️ Thermal Alert & 📊 Logging System
## 📖 Project Description

The **Cloud-Enabled IoT 🌡️ Thermal Alert & 📊 Logging System** is an embedded IoT project designed to monitor temperature in real time, log sensor data to the cloud, and generate thermal alerts when the temperature exceeds a predefined threshold. Built using the **LPC2148 ARM7 Microcontroller**, **LM35 Temperature Sensor**, **ESP-01 Wi-Fi Module**, and **ThingSpeak**, the system supports both local and remote set-point configuration, enabling efficient and reliable temperature monitoring from anywhere.

## 🎯 Aim
To design and implement a cloud-enabled IoT thermal monitoring system capable of real-time temperature monitoring, cloud data logging, thermal alert generation, and remote set-point management.
## ✨ Features

- 🌡️ **Real-Time Temperature Monitoring** – Continuously measures ambient temperature using the LM35 sensor.
- ☁️ **Cloud Data Logging** – Uploads temperature readings to the ThingSpeak cloud for remote monitoring and analysis.
- 📡 **Wi-Fi Connectivity** – Uses the ESP-01 module to enable seamless IoT communication.
- 🎯 **Remote Set-Point Configuration** – Allows users to update the temperature threshold remotely through the cloud.
- ⌨️ **Local Set-Point Adjustment** – Supports on-device set-point configuration using a 4×4 matrix keypad.
- 💾 **EEPROM Data Persistence** – Stores the configured set-point in EEPROM to retain settings after power loss.
- 🚨 **Automatic Thermal Alerts** – Activates a buzzer whenever the measured temperature exceeds the configured threshold.
- 🕒 **Real-Time Clock Integration** – Displays accurate date and time using the RTC module.
- 📺 **Live LCD Display** – Shows temperature, set-point, time, and system status on a 16×2 LCD.
- ⚡ **Embedded ARM7-Based Design** – Built around the LPC2148 microcontroller for reliable real-time operation.
  

 ### 🛠️ Hardware Components

| **Component** | **Model / Module** | **Function** |
|---------------|--------------------|--------------|
| 🧠 **Microcontroller** | LPC2148 (ARM7TDMI-S) | Controls the entire system, processes sensor data, and manages all peripherals. |
| 🌡️ **Temperature Sensor** | LM35 | Measures ambient temperature and provides an analog voltage proportional to temperature. |
| 📡 **Wi-Fi Module** | ESP-01 (ESP8266) | Enables wireless communication with the ThingSpeak cloud platform. |
| 🖥️ **Display** | 16×2 LCD | Displays temperature, set-point, date, time, and system status. |
| ⌨️ **Input Device** | 4×4 Matrix Keypad | Allows users to configure the temperature set-point locally. |
| 🕒 **Real-Time Clock** | DS1307 RTC | Maintains accurate date and time for timestamping. |
| 💾 **Memory** | I²C EEPROM (24Cxx Series) | Stores the configured set-point for non-volatile data retention. |
| 🚨 **Alert Device** | Piezo Buzzer | Generates an audible alarm when the temperature exceeds the set-point. |
| 🔌 **Power Supply** | 5V Regulated DC Supply | Provides stable power to all hardware components. |

## 💻 Software Requirements

| **Software** | **Version** | **Purpose** |
|--------------|-------------|-------------|
| 💻 **Keil µVision IDE** | v5.x | Develops, compiles, and debugs the Embedded C application. |
| 🔥 **Flash Magic** | v16.x or later | Uploads the compiled firmware to the LPC2148 microcontroller. |
| ☁️ **ThingSpeak** | Web Platform | Stores and visualizes real-time temperature data in the cloud. |
| 🌐 **Git** | Latest | Version control and source code management. |
| 📂 **GitHub** | Web Platform | Hosts the project repository and documentation. |
| 📝 **Embedded C** | — | Programming language used for firmware development. |

## **🏗️ System Architecture**
https://github.com/vamsikrishna-29/Cloud_Enabled_IoT_Thermal_Alert_System/blob/0623ae5022d89e75c99516583fbe8b98aa73e5f0/system%20architecture.png



