#include "weather_manager.h"
#include "secrets.h"
#include "config.h"

WeatherData weather_data;
unsigned long weather_latest_request = 0;

void handleWeatherUpdates() {
  if (millis() > weather_latest_request + WEATHER_API_DELAY) {
    getWeatherData();
  }
}

void getWeatherData() {
  String server = "https://api.openweathermap.org/data/2.5/weather?units=metric&q=" + String(WEATHER_QUERY) + "&APPID=" + String(WEATHER_API_KEY);
  String json_array = GET_Request(server.c_str());

  StaticJsonDocument<80> filter;
  filter["weather"][0]["icon"] = true;
  filter["main"]["temp"] = true;
  StaticJsonDocument<128> doc;

  DeserializationError error = deserializeJson(doc, json_array, DeserializationOption::Filter(filter));

  if (error) {
    Serial.print(F("Error while parsing weather data"));
    Serial.println(error.f_str());
    return;
  }

  JsonObject weather = doc["weather"][0];
  JsonObject main = doc["main"];

  strcpy(weather_data.icon_code, weather["icon"]);
  weather_data.temp = main["temp"];

  weather_latest_request = millis();
}

String GET_Request(const char *server) {
  HTTPClient http;
  http.begin(client, server);
  int httpResponseCode = http.GET();
  String payload = "{}";

  if (httpResponseCode == 200) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
  return payload;
} 