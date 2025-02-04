// src/config.h

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Nazwa i hasło sieci Wi-Fi
#define WIFI_SSID "your_wifi_ssid"
#define WIFI_PASSWORD "your_wifi_password"

// Deklaracje zmiennych globalnych związanych z Wi-Fi
extern bool wifiEnabled;
extern bool wifiConnected;
extern bool wifiConnecting;

#ifdef ESP32
  #include <WiFi.h>
  #define BUTTON_BOOT 0

  // I2C pins for ESP32
  #define OLED_SDA 21  // Możesz dostosować do swojego pinu SDA
  #define OLED_SCL 22  // Możesz dostosować do swojego pinu SCL

  // SPI pins for ESP32
  #define OLED_CLK    18
  #define OLED_MOSI   23
  #define OLED_CS     5
  #define OLED_DC     17
  #define OLED_RESET  -1

  // Dioda LED jest aktywna w stanie HIGH na ESP32
  #define LED_ACTIVE_LOW false

#else
  #include <ESP8266WiFi.h>
  #define BUTTON_BOOT 0

  // I2C pins for ESP8266
  #define OLED_SDA D2  // Możesz dostosować do swojego pinu SDA
  #define OLED_SCL D1  // Możesz dostosować do swojego pinu SCL

  // SPI pins for ESP8266
  #define OLED_CLK    14 // D5
  #define OLED_MOSI   13 // D7
  #define OLED_CS     15 // D8
  #define OLED_DC     5  // D1
  #define OLED_RESET  -1

  // Dioda LED jest aktywna w stanie LOW na ESP8266
  #define LED_ACTIVE_LOW true

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

// Zmienna globalna do przechowywania czasu rozpoczęcia ogólnego odliczania
extern unsigned long globalStartTime;

extern bool buttonLastState;
extern unsigned long debounceTime;

// Zmienna globalna do wskazania typu wyświetlacza
extern bool isDisplayI2C;

// Zmienne do zarządzania potwierdzeniem zmiany ustawień
extern bool isConfirmationDisplay;
extern unsigned long confirmationStartTime;
extern unsigned long confirmationDuration;
extern unsigned long confirmationValue;
extern char confirmationLabel[10]; // Przywrócona deklaracja confirmationLabel

#endif // CONFIG_H
