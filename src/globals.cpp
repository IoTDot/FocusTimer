#include "config.h"

// Define global variables

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

bool buttonLastState = HIGH;
unsigned long debounceTime = 50;

// Initialize the display type indicator
bool isDisplayI2C = false;