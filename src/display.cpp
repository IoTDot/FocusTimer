#include "display.h"
#include "config.h"

// Definicja zmiennej display
Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

void displayTime(unsigned long remainingTime, bool inverted, const char* label) {
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
