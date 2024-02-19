# ESP32 NTP RTC
## Description
This project synchorizes the ESP32 internal RTC with a NTP server. The RTC is used to keep track of time.
With the onboard RTC it is possible to keep track of time even if the ESP32 is not connected to the internet.
With the NTP server the RTC can be synchronized to the current time. For this we need to connect to the internet via WiFi.

After flashing the ESP32 with the code, the ESP32 will wait for a serial connection. The ESP32 will then try to connect to the WiFi network. If the connection is successful, the ESP32 will initialize the RTC. In the main loop the ESP will log the current time of the external rtc and the internal rtc. Then the ESP32 will try to synchronize the external RTC with the internal RTC. After the synchronization the ESP32 will wait for one second and the loop will start again.

## RTC Connection (ESP32)
- SCL -> GPIO 22
- SDA -> GPIO 21
- VCC -> 3.3V
- GND -> GND

## Hardware
- ESP32 development board (*ESP32-WROOM-32* or *ESP32S3*)
- USB cable
- External RTC (Real Time Clock) module

## Software
- VSCode and PlatformIO
- ESP32 Arduino Core
- NTPClient, WiFi, Wire and RTClib libraries

## Configuration
- Set the WiFi SSID and password in the `src/main.cpp` file.
- Set the NPT server and time zone in the `src/main.cpp` file.

**More information about the configuration can be found at the top of the `src/main.cpp` file. There you can also finde a description how to connect the external RTC module to the ESP32.**