#include "wifi_manager.h"
#include "secrets.h"
#include "config.h"
#include "display_manager.h"

Timezone timezone;

void setupWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to WiFi! IP: ");
  Serial.println(WiFi.localIP());
  display.clearDisplay();
  display.println("Connected to WiFi!");
  display.display();
}

void setupTime() {
  waitForSync();
  timezone.setLocation(TIME_ZONE);
} 