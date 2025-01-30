#include "button.h"
#include "config.h"
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
    if (buttonPressCount == 2) {
      timerDuration = (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) ? 10 * 60 * 1000 : mainTimerMinutes * 60 * 1000;
    } else if (buttonPressCount == 3) {
      breakTimerDuration = (breakTimerDuration == 2 * 60 * 1000) ? 5 * 60 * 1000 : 2 * 60 * 1000;
      displayTime(breakTimerDuration, false, "BREAK");
      delay(1000);
    } else if (buttonPressCount == 4) {
      if (!isShortBreak && !isLongBreak) {
        isLongBreak = true;
        startTime = millis();
      } else if (isLongBreak) {
        isShortBreak = true;
        isLongBreak = false;
        startTime = millis();
      } else if (isShortBreak) {
        isShortBreak = false;
        startTime = millis();
      }
    }

    if (buttonPressCount != 4) {
      startTime = millis();
      isShortBreak = false;
      isLongBreak = false;
    }
    buttonPressCount = 0;
  }
}
