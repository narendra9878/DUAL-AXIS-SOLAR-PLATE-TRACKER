# DUAL-AXIS-SOLAR-PLATE-TRACKER
# 🌞 Smart Solar Tracking System using ESP32 (IoT Enabled)

## 📌 Project Overview

This project presents a smart solar tracking system designed to maximize solar energy efficiency by continuously aligning the solar panel with the direction of maximum sunlight. The system uses LDR sensors for light detection and DC geared motors for panel movement. Additionally, environmental data such as temperature and humidity is monitored using a DHT11 sensor and uploaded to the cloud via ThingSpeak using ESP32.

---

## 🚀 Features

* 🌞 Dual-axis solar tracking using LDR sensors
* 🔄 DC motor-based panel movement
* 🌡️ Temperature & humidity monitoring (DHT11)
* ☁️ IoT integration with ThingSpeak
* 📡 Real-time data monitoring
* ⚡ Improved solar energy efficiency

---

## 🧰 Components Used

* ESP32 Microcontroller
* 4 × LDR Sensors
* 2 × 10 RPM DC Geared Motors
* Motor Driver (L298N / L293D)
* DHT11 Sensor
* 12V Solar Panel (20W)
* 12V 12Ah Battery
* Solar Charge Controller (12V, 10A)
* 150W UPS Unit
* Connecting Wires & Frame

---

## ⚙️ Working Principle

The system uses four LDR sensors placed around the solar panel to detect sunlight intensity. Based on the difference in light levels, the ESP32 controls DC motors through a motor driver to adjust the panel orientation.
Simultaneously, the DHT11 sensor measures temperature and humidity, and the data is sent to ThingSpeak for real-time monitoring.

---

## 🔌 Circuit Overview

* LDRs → Connected to analog pins of ESP32
* DC Motors → Controlled via motor driver (IN1–IN4)
* DHT11 → Connected to GPIO 4
* WiFi → Used for cloud communication

---

## 📡 IoT Integration (ThingSpeak)

* Sends temperature & humidity data every 15 seconds
* Requires:

  * Channel ID
  * Write API Key

---

## 🛠️ Setup Instructions

1. Install required libraries:

   * WiFi.h
   * ThingSpeak.h
   * DHT.h

2. Update credentials in code:

```cpp
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";
unsigned long channelNumber = YOUR_CHANNEL_ID;
const char* writeAPIKey = "YOUR_WRITE_API_KEY";
```

3. Upload code to ESP32
4. Connect hardware properly
5. Power the system

---

## 📊 Applications

* Solar power optimization systems
* Smart energy management
* IoT-based monitoring systems
* Educational and research projects

---

## 🔥 Future Scope

* Add MPPT optimization
* Mobile app integration
* GPS-based sun tracking
* AI-based efficiency improvement

---

## 🤝 Contributors

* Your Name / Company Name

---

## 📄 License

This project is open-source and available for educational and research purposes.

---
.
