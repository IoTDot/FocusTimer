#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_SSD1306.h>

// Deklaracja zmiennej display jako zewnętrznej
extern Adafruit_SSD1306 display;

void displayTime(unsigned long remainingTime, bool inverted = false, const char* label = "");

#endif