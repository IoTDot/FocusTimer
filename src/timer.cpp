#include "config.h"
#include "timer.h"
#include "display.h"

void updateTimers() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime;

  if (isShortBreakCountdown) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isShortBreakCountdown = false;
      if (isShortBreak) {
        isShortBreak = false;  // Przejście do głównego odliczania po krótkiej przerwie
      } else {
        isShortBreak = true;   // Przejście do przerwy (BREAK) po głównym odliczaniu
      }
      startTime = millis();
    }
    displayTime(remainingTime, true, "");

    // Włącz LED z mocą 40%
    #ifdef ESP32
      int pwmValue = 63; // 25% jasności LED (zakres pwm 0 do 255)
      ledcWrite(0, pwmValue);
    #else  // Dla ESP8266
      int pwmValue = 255; // 25% jasności LED (zakres 0 do 1023)
      if (LED_ACTIVE_LOW) {
        analogWrite(LED_BUILTIN, 1023 - pwmValue);
      } else {
        analogWrite(LED_BUILTIN, pwmValue);
      }
    #endif

    return;
  }

  // Wyłącz LED
  #ifdef ESP32
    ledcWrite(0, 0); // Wyłącz LED
  #else  // Dla ESP8266
    if (LED_ACTIVE_LOW) {
      analogWrite(LED_BUILTIN, 1023); // Wyłącz LED
    } else {
      analogWrite(LED_BUILTIN, 0); // Wyłącz LED
    }
  #endif

  if (!isShortBreak && !isLongBreak) {
    remainingTime = timerDuration - elapsedTime;
    if (elapsedTime >= timerDuration) {
      isLongBreak = true;
      isShortBreakCountdown = true; // Dodaj krótkie odliczanie po głównym odliczaniu
      startTime = millis();
    }
  } else if (isLongBreak) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isLongBreak = false;
      isShortBreakCountdown = true; // Dodaj krótkie odliczanie po długiej przerwie
      startTime = millis();
    }
  } else if (isShortBreak) {
    remainingTime = breakTimerDuration - elapsedTime;
    if (elapsedTime >= breakTimerDuration) {
      isShortBreak = false;
      isShortBreakCountdown = true; // Dodaj krótkie odliczanie po krótkiej przerwie
      startTime = millis();
    }
  }

  if (isShortBreak) {
    displayTime(remainingTime, false, "BREAK");
  } else {
    displayTime(remainingTime, false, "");
  }

  delay(100);
}
