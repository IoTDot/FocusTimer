#include "config.h"
#include "button.h"
#include "display.h"
#include "timer.h"
#include <string.h> // Dołączenie biblioteki dla strcpy

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
      // Reset timera i synchronizacja z globalnym zegarem
      unsigned long elapsedGlobalTime = millis() - globalStartTime;
      unsigned long remainder = elapsedGlobalTime % 1000;
      startTime = millis() - remainder; // Synchronizacja z pełną sekundą
      isShortBreak = false;
      isLongBreak = false;
    } else if (buttonPressCount == 2) {
      // Przełączanie głównego timera między 5 a 10 minut
      timerDuration = (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) ? 10 * 60 * 1000 : mainTimerMinutes * 60 * 1000;
      
      // Ustawienie informacji o potwierdzeniu dla głównego timera
      isConfirmationDisplay = true;
      confirmationStartTime = millis();
      confirmationValue = timerDuration;
      confirmationLabel[0] = '\0'; // Ustawienie pustego napisu, aby nie wyświetlać 'TIMER'
    } else if (buttonPressCount == 3) {
      // Przełączanie czasu przerwy między 2 a 5 minut
      breakTimerDuration = (breakTimerDuration == 2 * 60 * 1000) ? 5 * 60 * 1000 : 2 * 60 * 1000;

      // Ustawienie informacji o potwierdzeniu dla przerwy
      isConfirmationDisplay = true;
      confirmationStartTime = millis();
      confirmationValue = breakTimerDuration;
      strcpy(confirmationLabel, "BREAK");
    } else if (buttonPressCount == 4) {
      // Natychmiastowe rozpoczęcie krótkiej przerwy i synchronizacja z globalnym zegarem
      unsigned long elapsedGlobalTime = millis() - globalStartTime;
      unsigned long remainder = elapsedGlobalTime % 1000;
      startTime = millis() - remainder; // Synchronizacja z pełną sekundą
      isShortBreakCountdown = true;
    }

    buttonPressCount = 0;
  }
}
