#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// Nazwa i hasło sieci Wi-Fi
#define WIFI_SSID "test_2.9"
#define WIFI_PASSWORD "zaq1@WSX"

// Konfiguracja Blynk
#define BLYNK_TEMPLATE_ID "TMPL40mL89b8h"
#define BLYNK_TEMPLATE_NAME "FocusTimer"
#define BLYNK_AUTH_TOKEN "aDI5gWpKXwwmWI5Z-n5IHl35ZVV4zgDa"

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

#elif defined(ESP8266)
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
extern char confirmationLabel[10];

// Deklaracje zmiennych globalnych dla statystyk
extern unsigned long totalStudyTime;
extern unsigned long totalBreakTime;
extern unsigned int studySessions;

// Definicje wirtualnych pinów Blynk
#define PIN_MAIN_TIMER V0
#define PIN_BREAK_TIMER V1
#define PIN_TOTAL_STUDY_TIME V2
#define PIN_STUDY_SESSIONS V3
#define PIN_TOTAL_BREAK_TIME V4

#endif // CONFIG_H
