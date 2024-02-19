/*
+-------------------------------------------------------------------------------------------+
| ESP32 NTP RTC                                                                             |
|                                                                                           |
| This code is in the public domain.                                                        |
| Written by Friedjof Noweck - 2024                                                         |
+-------------------------------------------------------------------------------------------+ 
| With this code you can sync the time of an external RTC with the time of an NTP server.   |
| Get ready to sync time with NTP server:                                                   |
| 1. Change the SSID and password to match your WiFi network                                |
| 2. You might need to change the NTP server to match your region                           |
|   - Find your NTP server: https://www.pool.ntp.org/zone/                                  |
|   - Replace "pool.ntp.org" with your NTP server                                           |
|   - set your TZ example: "CET-1CEST,M3.5.0,M10.5.0/3" (Central European Time)             |
|     - Find your TZ: https://remotemonitoringsystems.ca/time-zone-abbreviations.php        |
|     - Replace "CET-1CEST,M3.5.0,M10.5.0/3" with your TZ                                   |
| 3. Upload the sketch to your ESP32                                                        |   
| 4. Open the serial monitor and wait for the ESP32 to connect to your WiFi network         |
| 5. Wait for the ESP32 to sync time with the NTP server                                    |
+-------------------------------------------------------------------------------------------+ 
| Connect the external RTC to your ESP32:                                                   |
| - SDA RTC -> pin 21 ESP32                                                                 |
| - SCL RTC -> pin 22 ESP32                                                                 |
| - VCC RTC -> 3.3V   ESP32                                                                 |
| - GND RTC -> GND    ESP32                                                                 |
+-------------------------------------------------------------------------------------------+ 
| Sync time with NTP server:                                                                |
| 1. Call syncTime() to sync time with NTP server                                           |
| 2. Call logRTC() to print the time of the external RTC                                    |
| 3. Call logNTP() to print the time of the NTP server                                      |
+-------------------------------------------------------------------------------------------+
*/

#include <Arduino.h>
#include <Wire.h>

#ifdef ESP32DEV
#include <WiFi.h>
#else
#include <ESP8266WiFi.h>
#endif

#include <SPI.h>
#include "time.h"
#include "RTClib.h"

#include "ConfigManager.h"


// This section is deprecated. You can now use the config.json file to store your credentials

//#define WIFI_SSID "<your wifi ssid>"
//#define WIFI_PASSWORD "<your wifi password>"
// Replace with your NTP server
//#define NTP_SERVER "pool.ntp.org"
//#define TZ "GMT-1"

// Offset in seconds to match your timezone
#define GMT_OFFSET_SEC 0
#define DAYLIGHT_OFFSET_SEC 3600
#define SERIAL_BAUD 115200

// Function prototypes
void syncTime();
void logRTC();
void logNTP();

// Init external RTC
RTC_DS3231 rtc;
ConfigManager configManager;

void setup () {
  // Init serial connection
  Serial.begin(SERIAL_BAUD);
  while (!Serial);

  // connect to wifi
  WiFi.begin(configManager.get_wifi_ssid(), configManager.get_wifi_password());
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi..");
  }
  
  // Sync time with NTP server
  configTime(GMT_OFFSET_SEC, DAYLIGHT_OFFSET_SEC, configManager.get_ntp_server());

  // Set timezone
  setenv("TZ", configManager.get_ntp_tz(), 1);

  // Init external RTC
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    abort();
  }
}

void loop() {
  // Print RTC and time from NTP server
  logRTC();
  // Print onboarding time
  logNTP();

  // Sync time with NTP server
  syncTime();

  Serial.println("---------------------------------------");

  delay(1000); // Warte eine Sekunde, bevor du erneut synchronisierst
}

void syncTime() {
  // Sync time with NTP server with RTC as time source
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  // Set RTC time
  DateTime dt = DateTime(
    timeinfo.tm_year + 1900,
    timeinfo.tm_mon + 1,
    timeinfo.tm_mday,
    timeinfo.tm_hour,
    timeinfo.tm_min,
    timeinfo.tm_sec
  );

  rtc.adjust(dt);
}

void logRTC() {
  // Print RTC time
  Serial.print("External RTC: ");
  time_t now = rtc.now().unixtime();
  Serial.print(ctime(&now));
}

void logNTP() {
  // Print time from NTP server
  Serial.print("Internal RTC: ");
  time_t now;
  time(&now);
  Serial.print(ctime(&now));
}