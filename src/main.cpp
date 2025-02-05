// src/main.cpp

#define BLYNK_PRINT Serial

// Definiujemy BLYNK_TEMPLATE_ID i BLYNK_DEVICE_NAME
#define BLYNK_TEMPLATE_ID "Your_Template_ID"
#define BLYNK_TEMPLATE_NAME "Your_Template_Name"

#include "config.h"
#include "display.h"
#include "button.h"
#include "timer.h"
#include "wifi_manager.h"

#ifdef ESP32
  #include <WiFi.h>
  #include <BlynkSimpleEsp32.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <BlynkSimpleEsp8266.h>
#endif

// Obiekt Blynk jest zdefiniowany tylko tutaj
// Upewnij się, że biblioteka Blynk jest dołączona tylko w tym pliku

#ifndef LED_BUILTIN
  #ifdef ESP32
    #define LED_BUILTIN 2  // LED na ESP32
  #else
    #define LED_BUILTIN 2  // LED na ESP8266
  #endif
#endif

void setup() {
  Serial.begin(115200); // Inicjalizacja komunikacji szeregowej do debugowania

  // Wyłączamy Wi-Fi na starcie
  #ifdef ESP32
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
  #else // ESP8266
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
  #endif

  #ifdef ESP32
    ledcSetup(0, 5000, 8); // Kanał PWM 0, 5 kHz, 8-bitowa rozdzielczość
    ledcAttachPin(LED_BUILTIN, 0); // Przypisanie pinu LED_BUILTIN do kanału PWM 0
  #else   // Dla ESP8266
    delay(1);
    pinMode(LED_BUILTIN, OUTPUT); // Ustawienie pinu LED_BUILTIN jako wyjście
    analogWriteRange(1023);       // Ustawienie zakresu PWM 0-1023
    analogWriteFreq(1000);        // Ustawienie częstotliwości PWM na 1kHz
  #endif
  delay(5);

  pinMode(BUTTON_BOOT, INPUT_PULLUP);

  if (!initializeDisplay()) {
    Serial.println("Failed to initialize display");
    for (;;); // Zatrzymaj wykonywanie lub obsłuż błąd inicjalizacji w razie potrzeby
  }

  display->clearDisplay();
  display->setTextSize(1);
  display->setTextColor(SSD1306_WHITE);
  display->setCursor(0, 0);
  display->println("IoTDot Timer");
  display->display();

  // Synchronizacja startTime i globalStartTime z pełnymi sekundami
  unsigned long currentMillis = millis();
  unsigned long remainder = currentMillis % 1000;
  globalStartTime = currentMillis - remainder; // Ustawienie globalnego czasu startu
  startTime = globalStartTime; // Ustawienie startTime w synchronizacji z globalnym timerem
}

void loop() {
  handleButtonPress();
  updateTimers();
  handleWiFi(); // Dodajemy obsługę Wi-Fi

  // Obsługa Blynk (wywoływane tylko, gdy Wi-Fi jest połączone)
  if (wifiConnected) {
    Blynk.run();
  }
}
