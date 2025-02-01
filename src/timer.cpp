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
    displayTime(remainingTime, true, ""); // Usunięcie etykiety "SHORT"

    // Włącz LED z mocą 40%
    #ifdef ESP32
      ledcWrite(0, 102); // 102 = 40% z 255 (8-bitowa rozdzielczość)
    #else
      analogWrite(LED_BUILTIN, 102); // 102 = 40% z 255 (8-bitowa rozdzielczość)
    #endif

    return;
  }

  // Wyłącz LED
  #ifdef ESP32
    ledcWrite(0, 0); // Wyłącz LED
  #else
    analogWrite(LED_BUILTIN, 0); // Wyłącz LED
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
      isShortBreakCountdown = true; // Dodaj krótkie odliczanie po głównym odliczaniu
      startTime = millis();
    }
  } else if (isShortBreak) {
    remainingTime = breakTimerDuration - elapsedTime;
    if (elapsedTime >= breakTimerDuration) {
      isShortBreak = false;
      isShortBreakCountdown = true; // Dodaj krótkie odliczanie po przerwie
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