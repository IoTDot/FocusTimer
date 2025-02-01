#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

// Declare the display pointer as external
extern Adafruit_SSD1306* display;

// Function to initialize the display
bool initializeDisplay();

// Function to display time
void displayTime(unsigned long remainingTime, bool inverted = false, const char* label = "");

#endif