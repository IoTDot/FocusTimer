#include "config.h"
#include "display.h"
#include "button.h"
#include "timer.h"
#include "wifi_manager.h"

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
    WiFi.mode(WIFI_OFF);
    ledcSetup(0, 5000, 8); // Kanał PWM 0, 5 kHz, 8-bitowa rozdzielczość
    ledcAttachPin(LED_BUILTIN, 0); // Przypisanie pinu LED_BUILTIN do kanału PWM 0
  #else   // Dla ESP8266
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
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
}
