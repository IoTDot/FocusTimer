#include "config.h"
#include "display.h"
#include "button.h"
#include "timer.h"

#ifndef LED_BUILTIN
  #ifdef ESP32
    #define LED_BUILTIN 2  // LED na ESP32
  #else
    #define LED_BUILTIN 2  // LED na ESP8266
  #endif
#endif

void setup() {
  #ifdef ESP32
    WiFi.mode(WIFI_OFF);
    ledcSetup(0, 5000, 8); // Kanał PWM 0, 5 kHz, 8-bitowa rozdzielczość
    ledcAttachPin(LED_BUILTIN, 0); // Przypisanie pinu LED_BUILTIN do kanału PWM 0
  #else
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
    pinMode(LED_BUILTIN, OUTPUT); // Ustawienie pinu LED_BUILTIN jako wyjście
  #endif
  delay(5);

  pinMode(BUTTON_BOOT, INPUT_PULLUP);

  Serial.begin(115200); // Inicjalizacja komunikacji szeregowej do debugowania

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

  startTime = millis();
}

void loop() {
  handleButtonPress();
  updateTimers();
}