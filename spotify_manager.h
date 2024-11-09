#ifndef SPOTIFY_MANAGER_H
#define SPOTIFY_MANAGER_H

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#elif defined(ESP32)
#include <WiFi.h>
#endif
#include <SpotifyArduino.h>

extern WiFiClientSecure client;
extern SpotifyArduino spotify;
extern CurrentlyPlaying currentlyPlaying;

void setupSpotify();
void handleSpotifyUpdates();
void updateSpotifyData(CurrentlyPlaying currentlyPlaying);

#endif 