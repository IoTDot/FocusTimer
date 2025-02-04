// src/wifi_manager.cpp

#include "wifi_manager.h"
#include "config.h"
#include "display.h"

#ifdef ESP32
  #include <WiFi.h>
#else // ESP8266
  #include <ESP8266WiFi.h>
#endif

// Usunięto definicję zmiennej `wifiConnected`
// bool wifiConnected = false;

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
      // Sprawdzamy, czy upłynął czas oczekiwania na połączenie
      if (millis() - wifiConnectStartTime > wifiConnectTimeout) {
        Serial.println("WiFi connection timeout.");
        wifiConnecting = false;
        wifiConnected = false;
      } else if (WiFi.status() == WL_CONNECTED) {
        Serial.println("WiFi connected!");
        wifiConnecting = false;
        wifiConnected = true;
      }
    } else if (wifiConnected) {
      // **Dodane sprawdzenie stanu połączenia**
      if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi connection lost.");
        wifiConnected = false;
        wifiConnecting = true; // Rozpoczynamy ponowną próbę połączenia
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