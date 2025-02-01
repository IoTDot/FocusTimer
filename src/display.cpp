#include "display.h"
#include "config.h"
#include "timer.h"

// Define two display objects (for I2C and SPI)
Adafruit_SSD1306 displayI2C(128, 64, &Wire, OLED_RESET);
Adafruit_SSD1306 displaySPI(128, 64, &SPI, OLED_DC, OLED_RESET, OLED_CS);

// Pointer to the active display
Adafruit_SSD1306* display = nullptr;

// Function to initialize the display
bool initializeDisplay() {
  Serial.println("Initializing display...");

  // Initialize I2C communication
  Wire.begin(OLED_SDA, OLED_SCL);

  // Scan for I2C devices
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
      if (address == 0x3C || address == 0x3D) {
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
    if (!display->begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
      Serial.println("SSD1306 I2C initialization failed");
      display = nullptr;
    } else {
      Serial.println("SSD1306 I2C initialized successfully");
      return true;
    }
  }

  // If I2C display not found or failed to initialize, try SPI
  Serial.println("Attempting to initialize display over SPI...");
  SPI.begin();
  display = &displaySPI;
  if (!display->begin(SSD1306_SWITCHCAPVCC)) {
    Serial.println("SSD1306 SPI initialization failed");
    display = nullptr;
    return false; // Both initializations failed
  } else {
    Serial.println("SSD1306 SPI initialized successfully");
    return true;
  }
}

void displayTime(unsigned long remainingTime, bool inverted, const char* label) {
  unsigned long minutes = remainingTime / 60000;
  unsigned long seconds = (remainingTime % 60000) / 1000;

  display->invertDisplay(inverted);
  display->clearDisplay();

  int16_t xPos = 0;
  int16_t yPos = 10;

  if (!inverted && !isShortBreakCountdown) {
    display->setTextSize(4);

    String timeText = (seconds < 10) ? String(minutes) + ":0" + String(seconds) : String(minutes) + ":" + String(seconds);
    int16_t textWidth = timeText.length() * 24; // Approximate text width
    xPos = (128 - textWidth) / 2;
    display->setCursor(xPos, yPos);
    display->print(timeText);
  }

  if (inverted || isShortBreakCountdown) {
    display->setTextSize(5);
    String timeText = String(seconds);
    int16_t textWidth = (seconds < 10) ? 30 : 60; // Approximate text width
    xPos = (128 - textWidth) / 2;
    display->setCursor(xPos, yPos);
    display->print(timeText);
  }

  if (label[0] != '\0') {
    display->setTextSize(2);
    int16_t labelWidth = 12 * strlen(label); // Approximate label width
    xPos = (128 - labelWidth) / 2;
    display->setCursor(xPos, 50);
    display->print(label);
  }

  display->display();
}
