#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

#include <ezTime.h>

extern Timezone timezone;

void setupWiFi();
void setupTime();

#endif 