#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

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

Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

int mainTimerMinutes = 5;
int breakTimerMinutes = 2;
int shortBreakSeconds = 6;

unsigned long timerDuration;
unsigned long breakTimerDuration;
unsigned long shortBreakDuration;

bool isShortBreak = false;
bool isLongBreak = false;
unsigned long lastButtonPress = 0;
int buttonPressCount = 0;
bool isShortBreakCountdown = false;

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

  timerDuration = mainTimerMinutes * 60 * 1000;
  breakTimerDuration = breakTimerMinutes * 60 * 1000;
  shortBreakDuration = shortBreakSeconds * 1000;

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

void displayTime(unsigned long remainingTime, bool inverted = false, const char* label = "") {
  unsigned long minutes = remainingTime / 60000;
  unsigned long seconds = (remainingTime % 60000) / 1000;

  display.invertDisplay(inverted);
  display.clearDisplay();
  int16_t xPos = 0;
  int16_t yPos = 10;

  if (!inverted && !isShortBreakCountdown) {
    display.setTextSize(4);
    String timeText = (seconds < 10) ? String(minutes) + ":0" + String(seconds) : String(minutes) + ":" + String(seconds);
    int16_t textWidth = timeText.length() * 24;
    xPos = (128 - textWidth) / 2;
    display.setCursor(xPos, yPos);
    display.print(timeText);
  }

  if (inverted || isShortBreakCountdown) {
    display.setTextSize(5);
    String timeText = String(seconds);
    int16_t textWidth = seconds < 10 ? 30 : 60;
    xPos = (128 - textWidth) / 2;
    display.setCursor(xPos, yPos);
    display.print(timeText);
  }

  if (label[0] != '\0') {
    display.setTextSize(2);
    int16_t labelWidth = 12 * strlen(label);
    xPos = (128 - labelWidth) / 2;
    display.setCursor(xPos, 50);
    display.print(label);
  }

  display.display();
}

void handleButtonPress() {
  unsigned long currentTime = millis();
  bool buttonState = digitalRead(BUTTON_BOOT) == LOW;

  if (buttonState != buttonLastState) {
    if (buttonState && (currentTime - lastButtonPress > debounceTime)) {
      buttonPressCount++;
      lastButtonPress = currentTime;
    }
  }

  buttonLastState = buttonState;

  if (!buttonState && (currentTime - lastButtonPress > 500) && buttonPressCount > 0) {
    if (buttonPressCount == 2) {
      timerDuration = (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) ? 10 * 60 * 1000 : mainTimerMinutes * 60 * 1000;
    } else if (buttonPressCount == 3) {
      breakTimerDuration = (breakTimerDuration == 2 * 60 * 1000) ? 5 * 60 * 1000 : 2 * 60 * 1000;
      displayTime(breakTimerDuration, false, "BREAK");
      delay(1000);
    } else if (buttonPressCount == 4) {
      if (!isShortBreak && !isLongBreak) {
        isLongBreak = true;
        startTime = millis();
      } else if (isLongBreak) {
        isShortBreak = true;
        isLongBreak = false;
        startTime = millis();
      } else if (isShortBreak) {
        isShortBreak = false;
        startTime = millis();
      }
    }

    if (buttonPressCount != 4) {
      startTime = millis();
      isShortBreak = false;
      isLongBreak = false;
    }
    buttonPressCount = 0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime;

  handleButtonPress();

  if (isShortBreakCountdown) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isShortBreakCountdown = false;
      startTime = millis();
    }
    displayTime(remainingTime, true);
    return;
  }

  if (!isShortBreak && !isLongBreak) {
    remainingTime = timerDuration - elapsedTime;
    if (elapsedTime >= timerDuration) {
      isLongBreak = true;
      startTime = millis();
    }
  } else if (isLongBreak) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isLongBreak = false;
      isShortBreak = true;
      startTime = millis();
    }
  } else if (isShortBreak) {
    remainingTime = breakTimerDuration - elapsedTime;
    if (elapsedTime >= breakTimerDuration) {
      isShortBreak = false;
      startTime = millis();
    }
  }

  if (isLongBreak) {
    displayTime(remainingTime, true);
  } else if (isShortBreak) {
    displayTime(remainingTime, false, "BREAK");
  } else {
    displayTime(remainingTime);
  }

  delay(100);
}
