#include "secrets.h"
#include "config.h"
#include "display_manager.h"
#include "spotify_manager.h"
#include "weather_manager.h"
#include "wifi_manager.h"

// Definición global del cliente WiFi seguro
WiFiClientSecure client;

void setup() {
  Serial.begin(SERIAL_BAUDRATE);
  
  setupDisplay();
  setupWiFi();
  setupSpotify();
  setupTime();
  
  // Get first weather data
  getWeatherData();
}

void loop() {
  handleSpotifyUpdates();
  handleWeatherUpdates();
  events();
}
