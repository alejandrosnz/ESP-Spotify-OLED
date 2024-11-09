#include "spotify_manager.h"
#include "secrets.h"
#include "config.h"
#include "display_manager.h"
#include "weather_manager.h"

// Definir spotify como variable global
SpotifyArduino spotify(client, SPOTIFY_CLIENT_ID, SPOTIFY_CLIENT_SECRET, SPOTIFY_REFRESH_TOKEN);
CurrentlyPlaying currentlyPlaying;
unsigned long spotify_latest_request = 0;

void setupSpotify() {
  #if defined(ESP8266)
    #ifdef SPOTIFY_FINGERPRINT
      client.setFingerprint(SPOTIFY_FINGERPRINT);
    #else
      client.setInsecure();
    #endif
  #elif defined(ESP32)
    client.setCACert(spotify_server_cert);
  #endif

  Serial.println("Refreshing Access Tokens");
  if (!spotify.refreshAccessToken()) {
    Serial.println("Failed to get access tokens");
  }
}

void handleSpotifyUpdates() {
  if (millis() > spotify_latest_request + SPOTIFY_API_DELAY) {
    int status = spotify.getCurrentlyPlaying(updateSpotifyData, SPOTIFY_MARKET);

    if (status == 200) {
      printCurrentlyPlayingToDisplay(currentlyPlaying);
      Serial.println("Successfully got currently playing");
    } else if (status == 204) {
      Serial.println("Doesn't seem to be anything playing");
      printLocalTime(weather_data);
    } else {
      Serial.print("Error getting currently playing. Status: ");
      Serial.println(status);
    }
    spotify_latest_request = millis();
  }
}

void updateSpotifyData(CurrentlyPlaying _currentlyPlaying) {
  currentlyPlaying = _currentlyPlaying;
} 