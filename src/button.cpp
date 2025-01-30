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
    if (buttonPressCount == 1) {
      // Resetowanie licznika
      startTime = millis();
      isShortBreak = false;
      isLongBreak = false;
    } else if (buttonPressCount == 2) {
      // Zmiana głównego odliczania między 5 a 10 minut
      timerDuration = (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) ? 10 * 60 * 1000 : mainTimerMinutes * 60 * 1000;
    } else if (buttonPressCount == 3) {
      // Zmiana czasu przerwy między 2 a 5 minut
      breakTimerDuration = (breakTimerDuration == 2 * 60 * 1000) ? 5 * 60 * 1000 : 2 * 60 * 1000;
      displayTime(breakTimerDuration, false, "BREAK");
      delay(1000);
    } else if (buttonPressCount == 4) {
      // Natychmiastowe rozpoczęcie przerwy
      isShortBreakCountdown = true;
      startTime = millis();
    }

    buttonPressCount = 0;
  }
}
