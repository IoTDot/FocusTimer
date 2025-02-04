// src/timer.cpp
#include "config.h"
#include "timer.h"
#include "display.h"
#include "wifi_manager.h" // Aby znać stan Wi-Fi

void updateTimers() {
  unsigned long currentTime = millis();

  // Sprawdzenie, czy mamy wyświetlić potwierdzenie
  if (isConfirmationDisplay) {
    if (currentTime - confirmationStartTime >= confirmationDuration) {
      isConfirmationDisplay = false;
      // Czyścimy wyświetlacz po zakończeniu wyświetlania potwierdzenia
      display->clearDisplay();
    } else {
      // Czyścimy wyświetlacz i wyświetlamy potwierdzenie
      display->clearDisplay();

      // Wyświetlamy czas potwierdzenia
      displayTime(confirmationValue, false, confirmationLabel);

      // Wyświetlamy ikonę Wi-Fi
      displayWiFiIcon();

      // Aktualizujemy wyświetlacz
      display->display();

      // Wstrzymujemy dalsze przetwarzanie w updateTimers()
      return;
    }
  }

  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime;

  // Czyścimy wyświetlacz na początku (jeśli nie było potwierdzenia)
  display->clearDisplay();

  if (isShortBreakCountdown) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isShortBreakCountdown = false;
      if (isShortBreak) {
        isShortBreak = false;  // Przejście do głównego odliczania po krótkiej przerwie
      } else {
        isShortBreak = true;   // Przejście do przerwy po głównym odliczaniu
      }
      // Synchronizacja startTime z globalnym timerem
      unsigned long elapsedGlobalTime = millis() - globalStartTime;
      unsigned long remainder = elapsedGlobalTime % 1000;
      startTime = millis() - remainder;
    }

    // Rysujemy odliczanie krótkiej przerwy
    displayTime(remainingTime, true, "");

    // Wyświetlamy ikonę Wi-Fi
    displayWiFiIcon();

    // Aktualizujemy wyświetlacz
    display->display();

    // Sterowanie diodą LED
    // Włącz LED z mocą 40%
    #ifdef ESP32
      int pwmValue = 63; // 25% jasności LED (zakres PWM 0 do 255)
      ledcWrite(0, pwmValue);
    #else  // Dla ESP8266
      int pwmValue = 255; // 25% jasności LED (zakres PWM 0 do 1023)
      if (LED_ACTIVE_LOW) {
        analogWrite(LED_BUILTIN, 1023 - pwmValue);
      } else {
        analogWrite(LED_BUILTIN, pwmValue);
      }
    #endif

    // Nie wyświetlamy globalnego licznika podczas 6-sekundowego odliczania

  } else {
    // Wyłączamy LED
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
        // Synchronizacja startTime z globalnym timerem
        unsigned long elapsedGlobalTime = millis() - globalStartTime;
        unsigned long remainder = elapsedGlobalTime % 1000;
        startTime = millis() - remainder;
      }
    } else if (isLongBreak) {
      remainingTime = breakTimerDuration - elapsedTime;
      if (elapsedTime >= breakTimerDuration) {
        isLongBreak = false;
        isShortBreakCountdown = true; // Dodaj krótkie odliczanie po długiej przerwie
        // Synchronizacja startTime z globalnym timerem
        unsigned long elapsedGlobalTime = millis() - globalStartTime;
        unsigned long remainder = elapsedGlobalTime % 1000;
        startTime = millis() - remainder;
      }
    } else if (isShortBreak) {
      remainingTime = breakTimerDuration - elapsedTime;
      if (elapsedTime >= breakTimerDuration) {
        isShortBreak = false;
        isShortBreakCountdown = true; // Dodaj krótkie odliczanie po krótkiej przerwie
        // Synchronizacja startTime z globalnym timerem
        unsigned long elapsedGlobalTime = millis() - globalStartTime;
        unsigned long remainder = elapsedGlobalTime % 1000;
        startTime = millis() - remainder;
      }
    }

    if (isShortBreak || isLongBreak) {
      displayTime(remainingTime, false, "BREAK");
    } else {
      displayTime(remainingTime, false, "");
    }

    // Wyświetlamy globalny licznik czasu
    displayGlobalTimer();

    // Wyświetlamy ikonę Wi-Fi
    displayWiFiIcon();

    // Aktualizujemy wyświetlacz
    display->display();
  }

  delay(100);
}
