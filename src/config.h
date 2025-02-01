#ifndef CONFIG_H
#define CONFIG_H

#ifdef ESP32
  #include <WiFi.h>
  #define BUTTON_BOOT 0

  // I2C pins for ESP32
  #define OLED_SDA 21  // Update to your SDA pin
  #define OLED_SCL 22  // Update to your SCL pin

  // SPI pins for ESP32
  #define OLED_CLK    18
  #define OLED_MOSI   23
  #define OLED_CS     5
  #define OLED_DC     17
  #define OLED_RESET  -1

#else
  #include <ESP8266WiFi.h>
  #define BUTTON_BOOT 0

  // I2C pins for ESP8266
  #define OLED_SDA D2  // Update to your SDA pin
  #define OLED_SCL D1  // Update to your SCL pin

  // SPI pins for ESP8266
  #define OLED_CLK    14 // D5
  #define OLED_MOSI   13 // D7
  #define OLED_CS     15 // D8
  #define OLED_DC     5  // D1
  #define OLED_RESET  -1

#endif

// Declarations of global variables
extern int mainTimerMinutes;
extern int breakTimerMinutes;
extern int shortBreakSeconds;

extern unsigned long timerDuration;
extern unsigned long breakTimerDuration;
extern unsigned long shortBreakDuration;

extern bool isShortBreak;
extern bool isLongBreak;
extern bool isShortBreakCountdown;

extern unsigned long lastButtonPress;
extern int buttonPressCount;
extern unsigned long startTime;

extern bool buttonLastState;
extern unsigned long debounceTime;

// New global variable to indicate display type (if needed)
extern bool isDisplayI2C;

#endif
