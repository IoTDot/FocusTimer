#include "config.h"
#include "display.h"
#include "button.h"
#include "timer.h"

void setup() {
  #ifdef ESP32
    WiFi.mode(WIFI_OFF);
  #else
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
  #endif
  delay(5);

  pinMode(BUTTON_BOOT, INPUT_PULLUP);

  Serial.begin(115200); // Initialize serial communication for debugging

  if (!initializeDisplay()) {
    Serial.println("Failed to initialize display");
    for (;;); // Stop execution or handle initialization failure as desired
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
