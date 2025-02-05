// src/wifi_manager.cpp

#include "wifi_manager.h"

#ifdef ESP32
  #include <BlynkSimpleEsp32.h>
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <BlynkSimpleEsp8266.h>
  #include <ESP8266WiFi.h>
#endif

#include "config.h"
#include "display.h"

extern Blynk::BlynkArduinoClient Blynk;

bool wifiConnected = false;
bool wifiConnecting = false;

#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Remove duplicate includes and ensure correct order.

#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Remove duplicate includes and ensure correct order.

#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Remove duplicate includes and ensure correct order.

#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif

// Remove duplicate includes and ensure correct order.

#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif



unsigned long wifiConnectStartTime = 0;
const unsigned long wifiConnectTimeout = 10000; // Timeout połączenia Wi-Fi (10 sekund)

void startWiFiConnection() {
  Serial.println("Starting WiFi connection...");
  wifiConnected = false;
  wifiConnecting = true;
  wifiConnectStartTime = millis();

  #ifdef ESP32
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #else // ESP8266
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  #endif
}

void stopWiFiConnection() {
  Serial.println("Stopping WiFi connection...");
  wifiConnected = false;
  wifiConnecting = false;

  #ifdef ESP32
    WiFi.disconnect(true, true);
    WiFi.mode(WIFI_OFF);
  #else // ESP8266
    WiFi.disconnect(true);
    WiFi.mode(WIFI_OFF);
  #endif
}

void handleWiFi() {
  if (wifiEnabled) {
    if (wifiConnecting) {
      if (millis() - wifiConnectStartTime > wifiConnectTimeout) {
        Serial.println("WiFi connection timeout.");
        wifiConnecting = false;
        wifiConnected = false;
      } else if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected!");
        wifiConnecting = false;
        wifiConnected = true;

        // Inicjalizacja Blynk
        Blynk.config(BLYNK_AUTH_TOKEN);
        Blynk.connect(); // Skonfiguruj połączenie z Blynk

        // Wysyłamy początkowe wartości statystyk
        if (Blynk.connected()) {
          Blynk.virtualWrite(PIN_TOTAL_STUDY_TIME, totalStudyTime / 60000); // Total Study Time
          Blynk.virtualWrite(PIN_STUDY_SESSIONS, studySessions);          // Study Sessions
          Blynk.virtualWrite(PIN_TOTAL_BREAK_TIME, totalBreakTime / 60000); // Total Break Time
        }
      }
    } else if (wifiConnected) {
      // Obsługa Blynk jest wykonywana w main.cpp

      // Sprawdzenie, czy połączenie Wi-Fi zostało utracone
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost.");
        wifiConnected = false;
        wifiConnecting = true;
        wifiConnectStartTime = millis();
      }
    } else {
      // Jeśli Wi-Fi jest włączone, ale nie jesteśmy połączeni i nie próbujemy się połączyć, rozpocznij próbę połączenia
      if (!wifiConnecting) {
        startWiFiConnection();
      }
    }
  } else {
    // Jeśli Wi-Fi jest wyłączone, upewnij się, że nie jesteśmy połączeni
    if (WiFi.getMode() != WIFI_OFF) {
      stopWiFiConnection();
    }
  }
}
