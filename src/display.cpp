#include "display.h"
#include "config.h"
#include "timer.h"

// Załączamy niestandardową czcionkę
#include <Fonts/FreeSans9pt7b.h>  // Możesz zmienić na inną czcionkę, jeśli chcesz

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
  int16_t yPos = 16; // Pozycja Y głównego timera

  if (!inverted && !isShortBreakCountdown) {
    display->setTextSize(4);
    display->setFont(); // Używamy domyślnej czcionki

    String timeText = (seconds < 10) ? String(minutes) + ":0" + String(seconds)
                                     : String(minutes) + ":" + String(seconds);

    // Obliczanie szerokości i wysokości tekstu timera
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(timeText.c_str(), 0, 0, &x1, &y1, &w, &h);

    xPos = (128 - w) / 2;
    display->setCursor(xPos, yPos - y1); // Korekta pozycji Y z uwzględnieniem linii bazowej
    display->print(timeText);
  }

  if (inverted || isShortBreakCountdown) {
    display->setTextSize(5);
    display->setFont(); // Używamy domyślnej czcionki

    String timeText = String(seconds);

    // Obliczanie szerokości i wysokości tekstu
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(timeText.c_str(), 0, 0, &x1, &y1, &w, &h);

    xPos = (128 - w) / 2;
    display->setCursor(xPos, yPos - y1); // Korekta pozycji Y z uwzględnieniem linii bazowej
    display->print(timeText);
  }

  if (label[0] != '\0') {
    // Ustawiamy niestandardową czcionkę
    display->setFont(&FreeSans9pt7b);
    display->setTextSize(1); // Dla niestandardowej czcionki używamy TextSize 1

    // Obliczanie szerokości i wysokości napisu
    int16_t x1, y1;
    uint16_t w, h;
    display->getTextBounds(label, 0, 0, &x1, &y1, &w, &h);

    // Wyśrodkowanie napisu w poziomie
    xPos = (128 - w) / 2;

    // Ustawienie pozycji napisu na dole ekranu
    int16_t labelYPos = 64 - h - 2; // 2 piksele marginesu od dołu

    // Korekta pozycji Y z uwzględnieniem linii bazowej
    display->setCursor(xPos, labelYPos - y1);
    display->print(label);

    // Resetujemy czcionkę do domyślnej
    display->setFont();
  }

  // Nie wywołujemy display->display() tutaj
}

// Funkcja do wyświetlania globalnego timera
void displayGlobalTimer() {
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - globalStartTime;
  unsigned long hours = elapsedTime / 3600000;
  unsigned long minutes = (elapsedTime % 3600000) / 60000;
  unsigned long seconds = (elapsedTime % 60000) / 1000;

  // Zwiększony rozmiar bufora dla większych wartości
  char buffer[12];
  snprintf(buffer, sizeof(buffer), "%lu:%02lu:%02lu", hours, minutes, seconds);

  display->setTextSize(1);
  display->setFont(); // Używamy domyślnej czcionki
  display->setCursor(0, 0); // Globalny timer pozostaje na swojej oryginalnej pozycji
  display->print(buffer);

  // Nie wywołujemy display->display() tutaj
}
