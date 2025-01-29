#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

// Definicja pinów dla wyświetlacza OLED
#define OLED_CLK    14  // D5 (GPIO14) – CLK
#define OLED_MOSI   13  // D7 (GPIO13) – MOSI
#define OLED_CS     15  // D8 (GPIO15) – CS
#define OLED_DC     5   // D1 (GPIO5) – D/C
#define OLED_RESET  -1  // Nie używamy resetu przez pin

Adafruit_SSD1306 display(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Definicja przycisku Boot (GPIO0)
#define BUTTON_BOOT 0

// Ustawienia czasów – łatwa edycja
int mainTimerMinutes = 5;  // Wpisz tutaj czas głównego odliczania w minutach (np. 5)
int breakTimerMinutes = 2; // Wpisz tutaj czas przerwy w minutach (np. 2)
int shortBreakSeconds = 6; // Wpisz tutaj czas przerwy pomiędzy odliczaniami w sekundach (np. 6)

unsigned long timerDuration;    // Główne odliczanie w milisekundach
unsigned long breakTimerDuration;  // Przerwa w milisekundach
unsigned long shortBreakDuration;  // Czas przerwy pomiędzy odliczaniami

bool isShortBreak = false;
bool isLongBreak = false;
unsigned long lastButtonPress = 0;
int buttonPressCount = 0;
bool isShortBreakCountdown = false;

unsigned long timerStart;

void setup() {
  // Wyłączenie WiFi
  WiFi.mode(WIFI_OFF);  // Wyłączenie Wi-Fi
  WiFi.forceSleepBegin();  // Wprowadzenie Wi-Fi w tryb głębokiego snu
  delay(5);

  // Inicjalizacja przycisku Boot
  pinMode(BUTTON_BOOT, INPUT_PULLUP);

  // Ustawienie czasów w milisekundach
  timerDuration = mainTimerMinutes * 60 * 1000;  // Główne odliczanie
  breakTimerDuration = breakTimerMinutes * 60 * 1000;  // Przerwa
  shortBreakDuration = shortBreakSeconds * 1000;  // Przerwa pomiędzy odliczaniami
  
  // Inicjalizacja wyświetlacza OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);  // Zatrzymaj program, jeśli wyświetlacz nie działa
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Minutnik");
  display.display();

  // Uruchomienie odliczania
  timerStart = millis();
}

void displayTime(unsigned long remainingTime, bool inverted = false, const char* label = "") {
  unsigned long minutes = remainingTime / 60000;
  unsigned long seconds = (remainingTime % 60000) / 1000;

  // Ustawienia dla odwróconych kolorów
  if (inverted) {
    display.invertDisplay(true);
  } else {
    display.invertDisplay(false);
  }

  display.clearDisplay();
  int16_t xPos = 0;  // Pozycja X do wyśrodkowania
  int16_t yPos = 10; // Pozycja Y (ustalona dla obu trybów)

  // Wyświetlanie odliczania głównego (MM:SS)
  if (!inverted && !isShortBreakCountdown) {
    display.setTextSize(4);  // Większy rozmiar tekstu
    String timeText = (seconds < 10) ? String(minutes) + ":0" + String(seconds) : String(minutes) + ":" + String(seconds);
    int16_t textWidth = timeText.length() * 24; // Przybliżona szerokość (24 piksele na znak w rozmiarze 4)
    xPos = (128 - textWidth) / 2; // Wyśrodkowanie
    display.setCursor(xPos, yPos);
    display.print(timeText);
  }

  // Wyświetlanie krótkiej przerwy (sekundy: 6, 5, 4, ...)
  if (inverted || isShortBreakCountdown) {
    display.setTextSize(5);  // Większy rozmiar cyfr
    String timeText = String(seconds);
    int16_t textWidth = seconds < 10 ? 30 : 60; // Przybliżona szerokość (30 pikseli dla 1 cyfry, 60 dla 2 cyfr w rozmiarze 5)
    xPos = (128 - textWidth) / 2; // Wyśrodkowanie
    display.setCursor(xPos, yPos);
    display.print(timeText);
  }

  // Wyświetlanie napisu BREAK (na stałe, bez znikania)
  if (label[0] != '\0') {
    display.setTextSize(2);
    int16_t labelWidth = 12 * strlen(label); // Przybliżona szerokość napisu
    xPos = (128 - labelWidth) / 2; // Wyśrodkowanie napisu BREAK
    display.setCursor(xPos, 50);  // Dolna część ekranu
    display.print(label);
  }

  display.display();
}

void handleButtonPress() {
  unsigned long currentTime = millis();

  if (digitalRead(BUTTON_BOOT) == LOW) {
    if (currentTime - lastButtonPress > 200) {  // Debounce
      buttonPressCount++;
      lastButtonPress = currentTime;
    }
  } else if (currentTime - lastButtonPress > 500 && buttonPressCount > 0) {
    // Obsługa liczby kliknięć
    if (buttonPressCount == 2) {
      if (timerDuration == (unsigned long)(mainTimerMinutes) * 60 * 1000) {
        timerDuration = 10 * 60 * 1000;
      } else {
        timerDuration = mainTimerMinutes * 60 * 1000;
      }
    } else if (buttonPressCount == 3) {
      if (breakTimerDuration == 2 * 60 * 1000) {
        breakTimerDuration = 5 * 60 * 1000;
      } else {
        breakTimerDuration = 2 * 60 * 1000;
      }
      displayTime(breakTimerDuration, false, "BREAK");
      delay(1000);  // Wyświetl na chwilę
    } else if (buttonPressCount == 4) {
      // Przełączamy pomiędzy głównym odliczaniem a przerwą
      if (!isShortBreak && !isLongBreak) {
        isLongBreak = true;  // Rozpoczynamy przerwę
        timerStart = millis();  // Resetujemy czas
      } else if (isLongBreak) {
        isShortBreak = true;  // Rozpoczynamy krótką przerwę
        isLongBreak = false;
        timerStart = millis();  // Resetujemy czas
      } else if (isShortBreak) {
        isShortBreak = false;  // Kończymy przerwę, wracamy do głównego odliczania
        timerStart = millis();
      }
    }

    // Zresetuj odliczanie, jeśli nie jest to 4 kliknięcia
    if (buttonPressCount != 4) {
      timerStart = millis();
      isShortBreak = false;
      isLongBreak = false;
    }
    buttonPressCount = 0;
  }
}

void loop() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - timerStart;
  unsigned long remainingTime;

  handleButtonPress();

  // Logika odliczania czasu
  if (isShortBreakCountdown) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isShortBreakCountdown = false;
      timerStart = millis();
    }
    displayTime(remainingTime, true);  // Odwrócone kolory dla krótkiego odliczania
    return;
  }

  if (!isShortBreak && !isLongBreak) {
    remainingTime = timerDuration - elapsedTime;
    if (elapsedTime >= timerDuration) {
      isLongBreak = true;
      timerStart = millis();
    }
  } else if (isLongBreak) {
    remainingTime = shortBreakDuration - elapsedTime;
    if (elapsedTime >= shortBreakDuration) {
      isLongBreak = false;
      isShortBreak = true;
      timerStart = millis();
    }
  } else if (isShortBreak) {
    remainingTime = breakTimerDuration - elapsedTime;
    if (elapsedTime >= breakTimerDuration) {
      isShortBreak = false;
      timerStart = millis();
    }
  }

  // Wyświetlanie czasu na ekranie OLED
  if (isLongBreak) {
    displayTime(remainingTime, true);  // Odwrócone kolory dla przerwy
  } else if (isShortBreak) {
    displayTime(remainingTime, false, "BREAK");
  } else {
    displayTime(remainingTime);       // Normalne kolory dla odliczania
  }

  delay(100);
}