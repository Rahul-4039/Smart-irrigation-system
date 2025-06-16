# 🌿 Smart Irrigation System - Flutter App
A Flutter-based Smart Irrigation System that monitors humidity, soil moisture, and provides both manual and automatic control of a water pump. Designed to optimize water usage and maintain healthy soil conditions for agricultural or gardening needs.


## 🔧 Features:
📈 Real-Time Monitoring :
View live readings of soil moisture and humidity levels directly in the app.

## 💧 Smart Water Control :
1. Manual Mode: Users can turn the water pump ON/OFF directly from the app.
2. Automatic Mode: The system intelligently turns the pump ON or OFF based on the moisture level
3. Pump turns ON when soil is dry.
4. Pump turns OFF when moisture reaches a satisfactory level.

## 📊 Auto Data Logging :
1. Automatically generates and updates an Excel sheet to record:
2. Timestamp of pump activation/deactivation.
3. Corresponding soil moisture and humidity readings.

## 📱 Platform Support :
1. Built using Flutter, compatible with Android only

## 🧠 Technologies Used :
1. Flutter (Dart) for frontend
2. Firebase
3. Excel / CSV Writer Packages
4. IoT Sensors (Soil Moisture, Humidity)
5. Microcontroller (ESP8266 - Wifi Module)

## 🚀 Getting Started :
1. Clone the repo
2. Install dependencies : 'flutter pub get'
3. Run the app : 'flutter run'
4. Make sure your IoT device and backend (MQTT/Firebase) are properly configured.

