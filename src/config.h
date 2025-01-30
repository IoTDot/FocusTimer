#ifndef CONFIG_H
#define CONFIG_H

#ifdef ESP32
  #include <WiFi.h>
  #define OLED_CLK    18
  #define OLED_MOSI   23
  #define OLED_CS     5
  #define OLED_DC     17
  #define BUTTON_BOOT 0
#else
  #include <ESP8266WiFi.h>
  #define OLED_CLK    14
  #define OLED_MOSI   13
  #define OLED_CS     15
  #define OLED_DC     5
  #define BUTTON_BOOT 0
#endif

#define OLED_RESET  -1

// Deklaracje zmiennych globalnych
extern int mainTimerMinutes;
extern int breakTimerMinutes;
extern int shortBreakSeconds;
extern unsigned long timerDuration;
extern unsigned long breakTimerDuration;
extern unsigned long shortBreakDuration;
extern bool isShortBreak;
extern bool isLongBreak;
extern unsigned long lastButtonPress;
extern int buttonPressCount;
extern bool isShortBreakCountdown;
extern unsigned long startTime;
extern bool buttonLastState;
extern unsigned long debounceTime;

#endif
