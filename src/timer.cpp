// src/timer.cpp

#include "config.h"
#include "timer.h"
#include "display.h"
#include "wifi_manager.h"

#ifdef ESP32
  #include <BlynkSimpleEsp32.h>
#elif defined(ESP8266)
  #include <BlynkSimpleEsp8266.h>
#endif

// Deklaracja obiektu Blynk jako extern
extern BlynkWifi Blynk;

void updateTimers() {
  unsigned long currentTime = millis();

  // Obsługa Wi-Fi i Blynk
  handleWiFi();

  // Sprawdzenie, czy mamy wyświetlić potwierdzenie
  if (isConfirmationDisplay) {
    if (currentTime - confirmationStartTime >= confirmationDuration) {
      isConfirmationDisplay = false;
      display->clearDisplay();
    } else {
      display->clearDisplay();
      displayTime(confirmationValue, false, confirmationLabel);
      displayWiFiIcon();
      display->display();
      return;
    }
  }

  unsigned long elapsedTime = currentTime - startTime;
  unsigned long remainingTime;

  display->clearDisplay();

  if (isShortBreakCountdown) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isShortBreakCountdown = false;
      if (isShortBreak) {
        isShortBreak = false;  // Przejście do głównego odliczania po krótkiej przerwie
      } else {
        isShortBreak = true;   // Przejście do przerwy po głównym odliczaniu

        // Zliczanie sesji nauki po zakończeniu głównego timera
        studySessions++;
        if (wifiConnected) {
          Blynk.virtualWrite(PIN_STUDY_SESSIONS, studySessions);
        }
      }
      unsigned long elapsedGlobalTime = millis() - globalStartTime;
      unsigned long remainder = elapsedGlobalTime % 1000;
      startTime = millis() - remainder;
    }

    displayTime(remainingTime, true, "");
    displayWiFiIcon();
    display->display();

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
        // Dodajemy czas do totalStudyTime
        totalStudyTime += timerDuration;
        if (wifiConnected) {
          Blynk.virtualWrite(PIN_TOTAL_STUDY_TIME, totalStudyTime / 60000); // W minutach
        }

        isLongBreak = true;
        isShortBreakCountdown = true; // Dodaj krótkie odliczanie po głównym odliczaniu
        unsigned long elapsedGlobalTime = millis() - globalStartTime;
        unsigned long remainder = elapsedGlobalTime % 1000;
        startTime = millis() - remainder;
      }
    } else if (isLongBreak || isShortBreak) {
      remainingTime = breakTimerDuration - elapsedTime;
      if (elapsedTime >= breakTimerDuration) {
        // Dodajemy czas do totalBreakTime
        totalBreakTime += breakTimerDuration;
        if (wifiConnected) {
          Blynk.virtualWrite(PIN_TOTAL_BREAK_TIME, totalBreakTime / 60000); // W minutach
        }

        isLongBreak = false;
        isShortBreakCountdown = true; // Dodaj krótkie odliczanie po przerwie
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

    displayGlobalTimer();
    displayWiFiIcon();
    display->display();
  }

  delay(100);
}

// Funkcje Blynk do obsługi wirtualnych pinów
BLYNK_WRITE(PIN_MAIN_TIMER) {
  int mainTimerValue = param.asInt(); // Odczytujemy wartość z aplikacji
  mainTimerMinutes = mainTimerValue;
  timerDuration = mainTimerMinutes * 60 * 1000;
  Serial.print("Main Timer set to: ");
  Serial.print(mainTimerMinutes);
  Serial.println(" minutes");
}

BLYNK_WRITE(PIN_BREAK_TIMER) {
  int breakTimerValue = param.asInt(); // Odczytujemy wartość z aplikacji
  breakTimerMinutes = breakTimerValue;
  breakTimerDuration = breakTimerMinutes * 60 * 1000;
  Serial.print("Break Timer set to: ");
  Serial.print(breakTimerMinutes);
  Serial.println(" minutes");
}
