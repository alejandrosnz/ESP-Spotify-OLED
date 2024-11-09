#ifndef WEATHER_MANAGER_H
#define WEATHER_MANAGER_H

#include <ArduinoJson.h>
#if defined(ESP8266)
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <HTTPClient.h>
#include <WiFi.h>
#endif

struct WeatherData {
  float temp;
  char icon_code[5];
};

extern WeatherData weather_data;
extern WiFiClientSecure client;

void getWeatherData();
void handleWeatherUpdates();
String GET_Request(const char *server);

#endif 