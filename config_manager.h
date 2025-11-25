#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <ArduinoJson.h>
#include "config.h"

#if defined(ESP32)
  #include <SPIFFS.h>
  #define FileFS SPIFFS
#elif defined(ESP8266)
  #include <LittleFS.h>
  #define FileFS LittleFS
#endif

#define CONFIG_FILE "/config.json"

// Load configuration from filesystem
bool loadConfig(char* timezone, char* weather_query, char* spotify_client_id, char* spotify_client_secret, char* spotify_refresh_token, char* weather_api_key) {
  if (!FileFS.exists(CONFIG_FILE)) {
    Serial.println("Config file not found, using defaults from secrets.h");
    strcpy(timezone, TIME_ZONE);
    strcpy(weather_query, WEATHER_QUERY);
    // API keys will use values from secrets.h if they exist
    return false;
  }

  File configFile = FileFS.open(CONFIG_FILE, "r");
  if (!configFile) {
    Serial.println("Failed to open config file");
    strcpy(timezone, TIME_ZONE);
    strcpy(weather_query, WEATHER_QUERY);
    return false;
  }

  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);
  configFile.close();

  StaticJsonDocument<1024> doc;
  DeserializationError error = deserializeJson(doc, buf.get());
  
  if (error) {
    Serial.println("Failed to parse config file");
    strcpy(timezone, TIME_ZONE);
    strcpy(weather_query, WEATHER_QUERY);
    return false;
  }

  strcpy(timezone, doc["timezone"] | TIME_ZONE);
  strcpy(weather_query, doc["weather_query"] | WEATHER_QUERY);
  
  // Load API credentials if they exist in config
  if (doc.containsKey("spotify_client_id")) {
    strcpy(spotify_client_id, doc["spotify_client_id"]);
  }
  if (doc.containsKey("spotify_client_secret")) {
    strcpy(spotify_client_secret, doc["spotify_client_secret"]);
  }
  if (doc.containsKey("spotify_refresh_token")) {
    strcpy(spotify_refresh_token, doc["spotify_refresh_token"]);
  }
  if (doc.containsKey("weather_api_key")) {
    strcpy(weather_api_key, doc["weather_api_key"]);
  }
  
  Serial.println("Config loaded successfully");
  Serial.print("  Timezone: ");
  Serial.println(timezone);
  Serial.print("  Weather: ");
  Serial.println(weather_query);
  
  return true;
}

// Save configuration to filesystem
bool saveConfig(const char* timezone, const char* weather_query, const char* spotify_client_id, const char* spotify_client_secret, const char* spotify_refresh_token, const char* weather_api_key) {
  Serial.println("Saving config...");
  
  StaticJsonDocument<1024> doc;
  doc["timezone"] = timezone;
  doc["weather_query"] = weather_query;
  doc["spotify_client_id"] = spotify_client_id;
  doc["spotify_client_secret"] = spotify_client_secret;
  doc["spotify_refresh_token"] = spotify_refresh_token;
  doc["weather_api_key"] = weather_api_key;

  File configFile = FileFS.open(CONFIG_FILE, "w");
  if (!configFile) {
    Serial.println("Failed to open config file for writing");
    return false;
  }

  if (serializeJson(doc, configFile) == 0) {
    Serial.println("Failed to write config file");
    configFile.close();
    return false;
  }

  configFile.close();
  Serial.println("Config saved successfully");
  return true;
}

#endif
