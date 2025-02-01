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
        isShortBreak = false;  // Transition to main timer after short break
      } else {
        isShortBreak = true;   // Transition to break (BREAK) after main timer
      }
      startTime = millis();
    }
    displayTime(remainingTime, true, ""); // Remove "SHORT" label
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
