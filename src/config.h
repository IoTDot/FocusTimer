#ifndef CONFIG_H
#define CONFIG_H

#ifdef ESP32
  #include <WiFi.h>
  #define BUTTON_BOOT 0

  // I2C pins for ESP32
  #define OLED_SDA 21  // Aktualizuj do swojego pinu SDA
  #define OLED_SCL 22  // Aktualizuj do swojego pinu SCL

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
  #define OLED_SDA D2  // Aktualizuj do swojego pinu SDA
  #define OLED_SCL D1  // Aktualizuj do swojego pinu SCL

  // SPI pins for ESP8266
  #define OLED_CLK    14 // D5
  #define OLED_MOSI   13 // D7
  #define OLED_CS     15 // D8
  #define OLED_DC     5  // D1
  #define OLED_RESET  -1

#endif

// Deklaracje zmiennych globalnych
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

// Nowa zmienna globalna do wskazania typu wyświetlacza (jeśli potrzebna)
extern bool isDisplayI2C;

#endif