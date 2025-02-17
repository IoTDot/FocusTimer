#include "display.h"
#include "config.h"
#include "timer.h"
#include "wifi_manager.h" // Potrzebujemy znać stan Wi-Fi
#include "wifi_icon.h"    // Plik z ikoną Wi-Fi

// Załączamy czcionkę dla globalnego licznika
#include <Fonts/FreeMono9pt7b.h>

// Załączamy czcionkę dla napisu "BREAK"
#include <Fonts/FreeSans9pt7b.h>  // Możesz zmienić tę czcionkę, jeśli chcesz

// Definicja dwóch obiektów wyświetlacza (dla I2C i SPI)
Adafruit_SSD1306 displayI2C(128, 64, &Wire, OLED_RESET);
Adafruit_SSD1306 displaySPI(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Wskaźnik do aktywnego wyświetlacza
Adafruit_SSD1306* display = nullptr;

// Funkcja do inicjalizacji wyświetlacza
bool initializeDisplay() {
  Serial.println("Initializing display...");

  // Inicjalizacja komunikacji I2C
  Wire.begin(OLED_SDA, OLED_SCL);

  // Skanowanie urządzeń I2C
  isDisplayI2C = false;
  byte error, address;
  int nDevices = 0;

  Serial.println("Scanning for I2C devices...");
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) Serial.print("0");
      Serial.println(address, HEX);
      nDevices++;
      if (address == 0x3C || address == 0x3D || address == 0x7A || address == 0x7B) {
        isDisplayI2C = true;
      }
    }
  }

  if (nDevices == 0) {
    Serial.println("No I2C devices found");
  } else {
    Serial.println("I2C devices found");
  }

  if (isDisplayI2C) {
    Serial.println("Initializing display over I2C...");
    display = &displayI2C;
    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C) &&
        !display->begin(SSD1306_SWITCHCAPVCC, 0x3D) &&
        !display->begin(SSD1306_SWITCHCAPVCC, 0x7A) &&
        !display->begin(SSD1306_SWITCHCAPVCC, 0x7B)) {
      Serial.println("SSD1306 I2C initialization failed");
      display = nullptr;
    } else {
      Serial.println("SSD1306 I2C initialized successfully");
      return true;
    }
  }

  // Jeśli wyświetlacz I2C nie został znaleziony lub nie udało się go zainicjalizować, spróbuj SPI
  Serial.println("Attempting to initialize display over SPI...");
  SPI.begin();
  display = &displaySPI;
  if (!display->begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 SPI initialization failed");
    display = nullptr;
    return false; // Obie inicjalizacje nie powiodły się
  } else {
    Serial.println("SSD1306 SPI initialized successfully");
    return true;
  }
}

// Funkcja do wyświetlania czasu
void displayTime(unsigned long remainingTime, bool inverted, const char* label) {
  unsigned long minutes = remainingTime / 60000;
  unsigned long seconds = (remainingTime % 60000) / 1000;

  display->invertDisplay(inverted);
  // Nie czyścimy wyświetlacza tutaj

  int16_t xPos = 0;
  int16_t yPos = 0; // Zmienimy tę wartość później

  if (!inverted && !isShortBreakCountdown) {
    display->setTextSize(4);
    display->setFont(); // Używamy domyślnej czcionki

    String timeText = (seconds < 10) ? String(minutes) + ":0" + String(seconds)
                                     : String(minutes) + ":" + String(seconds);

    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(timeText.c_str(), 0, 0, &x1, &y1, &w, &h);

    xPos = (128 - w) / 2;
    yPos = (64 - h) / 2 - y1; // Wyśrodkowanie pionowe
    display->setCursor(xPos, yPos);
    display->print(timeText);
  }

  if (inverted || isShortBreakCountdown) {
    display->setTextSize(5);
    display->setFont(); // Używamy domyślnej czcionki

    String timeText = String(seconds);
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(timeText.c_str(), 0, 0, &x1, &y1, &w, &h);

    xPos = (128 - w) / 2;
    yPos = (64 - h) / 2 - y1; // Wyśrodkowanie pionowe
    display->setCursor(xPos, yPos);
    display->print(timeText);
  }

  if (label[0] != '\0') {
    display->setFont(&FreeSans9pt7b); // Używamy czcionki dla napisu "BREAK"
    display->setTextSize(1);

    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(label, 0, 0, &x1, &y1, &w, &h);

    xPos = (128 - w) / 2;
    int16_t labelYPos = 64 - h - 2; // Pozycja napisu "BREAK" przy dolnej krawędzi
    display->setCursor(xPos, labelYPos - y1);
    display->print(label);

    display->setFont(); // Resetujemy czcionkę do domyślnej
  }
}


// Funkcja do wyświetlania globalnego licznika
void displayGlobalTimer() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - globalStartTime;
  unsigned long hours = elapsedTime / 3600000;
  unsigned long minutes = (elapsedTime % 3600000) / 60000;
  unsigned long seconds = (elapsedTime % 60000) / 1000;

  // Przygotowanie tekstu
  char buffer[12];
  snprintf(buffer, sizeof(buffer), "%lu:%02lu:%02lu", hours, minutes, seconds);

  // Używamy czcionki FreeMono9pt7b dla globalnego licznika
  display->setFont(&FreeMono9pt7b);
  display->setTextSize(1); // Dla niestandardowych czcionek używamy TextSize 1

  // Obliczamy wymiary tekstu
  int16_t x1, y1;
  uint16_t w, h;
  display->getTextBounds(buffer, 0, 0, &x1, &y1, &w, &h);

  // Wyśrodkujmy tekst w poziomie
  int16_t xPos = (128 - w) / 2;
  int16_t yPos = 0 - y1; // Ustawienie pozycji na górze wyświetlacza

  display->setCursor(xPos, yPos);
  display->print(buffer);

  display->setFont(); // Resetujemy czcionkę do domyślnej

  // Nie wywołujemy display->display() tutaj
}

// Funkcja do wyświetlania ikony Wi-Fi
void displayWiFiIcon() {
  static unsigned long lastBlinkTime = 0;
  static bool iconVisible = true;

  if (wifiEnabled) {
    if (wifiConnected) {
      // Połączone z Wi-Fi, wyświetlamy ikonę stale
      display->drawBitmap(128 - WIFI_ICON_WIDTH, 0, wifi_icon_bmp, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, SSD1306_WHITE);
    } else {
      // Nie jesteśmy połączeni, ikona miga
      if (millis() - lastBlinkTime >= 500) { // Częstotliwość migania co 500 ms
        lastBlinkTime = millis();
        iconVisible = !iconVisible;
      }
      if (iconVisible) {
        display->drawBitmap(128 - WIFI_ICON_WIDTH, 0, wifi_icon_bmp, WIFI_ICON_WIDTH, WIFI_ICON_HEIGHT, SSD1306_WHITE);
      }
    }
  }
  // Jeśli Wi-Fi jest wyłączone, nie wyświetlamy ikony
}

// Funkcja do aktualizacji wyświetlacza
void updateDisplay() {
  display->clearDisplay();

  // Wyświetlanie czasu (timerów)
  // Zakładam, że funkcja displayTime() jest wywoływana w odpowiednim miejscu w kodzie, np. w updateTimers()

  // Wyświetlanie globalnego licznika
  displayGlobalTimer();

  // Wyświetlanie ikony Wi-Fi
  displayWiFiIcon();

  // Aktualizacja wyświetlacza
  display->display();
}
