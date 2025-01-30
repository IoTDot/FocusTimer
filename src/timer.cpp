#include "timer.h"
#include "config.h"
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
        isShortBreak = false;  // Przechodzi do głównego licznika po krótkiej przerwie
      } else {
        isShortBreak = true;   // Przechodzi do przerwy (BREAK) po głównym liczniku
      }
      startTime = millis();
    }
    displayTime(remainingTime, true, ""); // Usunięcie napisu "SHORT"
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
    displayTime(remainingTime, true, "LONG");
  } else if (isShortBreak) {
    displayTime(remainingTime, false, "BREAK");
  } else {
    displayTime(remainingTime, false, "");
  }

  delay(100);
}
