# ICU Multi-Patient Alarm Prioritization System

## About the Project

This project is an educational embedded-systems prototype designed to demonstrate pulse monitoring and alarm indication.

The system uses an ESP32 microcontroller and an analog pulse sensor to estimate heart rate in BPM. Based on programmed demonstration thresholds, LEDs and a buzzer indicate different status levels.

The current implementation focuses on a single patient channel and can be extended to multiple patient channels in future development.

## Hardware Used

- ESP32 Development Board
- Analog 3-pin Pulse Sensor
- Green LED
- Yellow LED
- Red LED
- 220Ω Resistors
- Buzzer
- Pushbutton
- Breadboard
- Jumper Wires

## Software Used

- Arduino IDE
- ESP32 Board Package
- Serial Monitor

## Pin Connections

| Component | ESP32 Pin |
|---|---|
| Pulse Sensor Signal | GPIO 34 |
| Pulse Sensor VCC | 3.3V |
| Pulse Sensor GND | GND |
| Green LED | GPIO 13 |
| Yellow LED | GPIO 12 |
| Red LED | GPIO 14 |
| Buzzer | GPIO 18 |
| Pushbutton | GPIO 19 |

## System Flow

Pulse Sensor → ESP32 → Pulse Detection → BPM Estimation → Status Classification → LEDs and Buzzer

## Demonstration Thresholds

- Below 60 BPM – Low-rate critical/demo alert
- 60–100 BPM – Normal/demo
- 101–120 BPM – Warning/demo
- Above 120 BPM – High-rate critical/demo alert

## Project Status

This is an educational prototype for learning and demonstration. The thresholds and pulse-detection method are not clinically validated and the system is not intended for real-patient monitoring or clinical decisions.

## Future Enhancements

- Multiple patient sensor channels
- Central alarm prioritization
- Improved signal filtering and beat detection
- OLED display
- Data logging
- Communication features
