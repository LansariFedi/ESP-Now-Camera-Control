# ESP-Now Camera Control

## Overview

This project demonstrates how to use ESP-NOW communication to control an ESP32-CAM module with another ESP32 device. The master ESP32 sends commands to the ESP32-CAM to capture images, which are then served via a web server hosted on the ESP32-CAM.

## Features

- **ESP-NOW Communication**: Efficient and low-latency communication between ESP32 devices.
- **Web Server**: The ESP32-CAM hosts a web server to display captured images.
- **Real-Time Image Capture**: Capture images on demand via commands sent from the master ESP32.
- **Broadcast and Peer Registration**: Dynamically register new ESP-NOW peers for communication.

## Hardware Requirements

- 1 x ESP32 (Master)
- 1 x ESP32-CAM (Slave)
- USB cables for programming and power
- Wi-Fi network for the ESP32-CAM web server

## Software Requirements

- Arduino IDE with ESP32 board support
- Required libraries:
  - `esp_camera`
  - `WiFi`
  - `WebServer`
  - `ESP32_NOW`

## Project Structure

```
ESP-Now-Camera-Control/
├── esp32/
│   └── esp32.ino          # Master ESP32 code
├── espcam/
│   ├── cam.cpp            # Camera functionality
│   ├── cam.h              # Camera header file
│   ├── espcam.ino         # ESP32-CAM code
│   ├── web.cpp            # Web server functionality
│   ├── web.h              # Web server header file
├── README.md              # Project documentation
```

## Setup Instructions

### 1. Master ESP32 Setup

1. Open `esp32/esp32.ino` in the Arduino IDE.
2. Configure the Wi-Fi channel by modifying the `#define ESPNOW_WIFI_CHANNEL` value if needed.
3. Upload the code to the ESP32.

### 2. ESP32-CAM Setup

1. Open `espcam/espcam.ino` in the Arduino IDE.
2. Update the Wi-Fi credentials in the `ssid` and `password` variables.
3. Configure the camera pins in `cam.cpp` if using a custom ESP32-CAM module.
4. Upload the code to the ESP32-CAM.

### 3. Hardware Connections

- Connect the ESP32 and ESP32-CAM to your computer for programming.
- Power the devices using USB or an external power source.

## Usage

1. Power on both the ESP32 and ESP32-CAM.
2. The master ESP32 will broadcast commands via ESP-NOW.
3. Use the Serial Monitor on the master ESP32 to send the `capture` command.
4. The ESP32-CAM will capture an image and host it on its web server.
5. Access the ESP32-CAM's IP address in a web browser to view the captured image.

## Web Interface

- The web server provides a simple interface to view the latest captured image.
- The image is refreshed every 5 seconds automatically.

## Troubleshooting

- Ensure both devices are on the same Wi-Fi channel.
- Verify the ESP32-CAM is connected to the Wi-Fi network.
- Check the Serial Monitor for error messages during setup.

## License

This project is open-source and available under the MIT License.

## Acknowledgments

- Inspired by the ESP-NOW and ESP32-CAM examples provided by Espressif.
- Special thanks to the open-source community for their contributions.

---

Enjoy building and experimenting with this project!
