#include "config.h"
#include "button.h"
#include "display.h"
#include "timer.h"

void handleButtonPress() {
  unsigned long currentTime = millis();
  bool buttonState = digitalRead(BUTTON_BOOT) == LOW;

  if (buttonState != buttonLastState) {
    if (buttonState && (currentTime - lastButtonPress > debounceTime)) {
      buttonPressCount++;
      lastButtonPress = currentTime;
    }
  }

  buttonLastState = buttonState;

  if (!buttonState && (currentTime - lastButtonPress > 500) && buttonPressCount > 0) {
    if (buttonPressCount == 1) {
      // Reset the timer
      startTime = millis();
      isShortBreak = false;
      isLongBreak = false;
    } else if (buttonPressCount == 2) {
      // Toggle main timer between 5 and 10 minutes
      timerDuration = (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) ? 10 * 60 * 1000 : mainTimerMinutes * 60 * 1000;
    } else if (buttonPressCount == 3) {
      // Toggle break time between 2 and 5 minutes
      breakTimerDuration = (breakTimerDuration == 2 * 60 * 1000) ? 5 * 60 * 1000 : 2 * 60 * 1000;
      displayTime(breakTimerDuration, false, "BREAK");
      delay(1000);
    } else if (buttonPressCount == 4) {
      // Start short break immediately
      isShortBreakCountdown = true;
      startTime = millis();
    }

    buttonPressCount = 0;
  }
}
