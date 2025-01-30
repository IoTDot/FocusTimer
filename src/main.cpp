#include "config.h"
#include "display.h"
#include "button.h"
#include "timer.h"

int mainTimerMinutes = 5;
int breakTimerMinutes = 2;
int shortBreakSeconds = 6;

unsigned long timerDuration = mainTimerMinutes * 60 * 1000;
unsigned long breakTimerDuration = breakTimerMinutes * 60 * 1000;
unsigned long shortBreakDuration = shortBreakSeconds * 1000;

bool isShortBreak = false;
bool isLongBreak = false;
bool isShortBreakCountdown = false;
unsigned long lastButtonPress = 0;
int buttonPressCount = 0;
unsigned long startTime;
bool buttonLastState = HIGH;
unsigned long debounceTime = 50;

void setup() {
  #ifdef ESP32
    WiFi.mode(WIFI_OFF);
  #else
    WiFi.mode(WIFI_OFF);
    WiFi.forceSleepBegin();
  #endif
  delay(5);

  pinMode(BUTTON_BOOT, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Minutnik");
  display.display();

  startTime = millis();
}

void loop() {
  handleButtonPress();
  updateTimers();
}
