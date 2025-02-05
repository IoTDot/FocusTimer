// src/globals.cpp

#include "config.h"

// Definicje zmiennych globalnych związanych z Wi-Fi
bool wifiEnabled = false;
bool wifiConnected = false;
bool wifiConnecting = false;

// Definicje zmiennych globalnych

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
unsigned long startTime = 0;

// Zmienna globalna do przechowywania czasu rozpoczęcia ogólnego odliczania
unsigned long globalStartTime = 0;

bool buttonLastState = HIGH;
unsigned long debounceTime = 50;

// Inicjalizacja wskaźnika typu wyświetlacza
bool isDisplayI2C = false;

// Zmienne do zarządzania potwierdzeniem zmiany ustawień
bool isConfirmationDisplay = false;
unsigned long confirmationStartTime = 0;
unsigned long confirmationDuration = 1000; // Czas trwania potwierdzenia w milisekundach
unsigned long confirmationValue = 0;
char confirmationLabel[10] = "";

// Definicje zmiennych globalnych dla statystyk
unsigned long totalStudyTime = 0;
unsigned long totalBreakTime = 0;
unsigned int studySessions = 0;
