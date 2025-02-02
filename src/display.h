#ifndef DISPLAY_H
#define DISPLAY_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <SPI.h>

// Deklaracja wskaźnika do wyświetlacza jako zewnętrzna
extern Adafruit_SSD1306* display;

// Funkcja do inicjalizacji wyświetlacza
bool initializeDisplay();

// Funkcja do wyświetlania czasu
void displayTime(unsigned long remainingTime, bool inverted = false, const char* label = "");

// Funkcja do wyświetlania globalnego timera
void displayGlobalTimer();

#endif // DISPLAY_H
