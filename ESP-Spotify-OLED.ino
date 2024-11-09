#include "secrets.h"
#include "config.h"

// ------------------
// Standard Libraries
// ------------------

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <WiFi.h>
#include <WiFiClientSecure.h>
#endif

#include <ArduinoJson.h>

// ----------------------------
// Spotify API lib
// github.com/witnessmenow/spotify-api-arduino
// ----------------------------

#include <SpotifyArduino.h>
#include <SpotifyArduinoCert.h>

// ----------------------------
// OLED Screen lib
// github.com/adafruit/Adafruit_SH110x
// ----------------------------

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeMonoBold9pt7b.h>

#define OLED_SDA D1
#define OLED_SCL D2
#define OLED_I2C_ADDR 0x3c
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display = Adafruit_SH1106G(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

// ----------------------------
// EzTime lib
// https://github.com/ropg/ezTime
// ----------------------------

#include <ezTime.h>
Timezone timezone;

// ----------------------------
// Wheather icons and data
// ----------------------------

#include "weather_icons.h"
struct WeatherData {
  float temp;
  char icon_code[5];
};
WeatherData weather_data;


WiFiClientSecure client;
SpotifyArduino spotify(client, SPOTIFY_CLIENT_ID, SPOTIFY_CLIENT_SECRET, SPOTIFY_REFRESH_TOKEN);
CurrentlyPlaying currentlyPlaying;
unsigned long spotify_latest_request = 0;
unsigned long weather_latest_request = 0;

void setup() {
  Serial.begin(115200);

  // OLED setup
  Wire.begin(OLED_SDA, OLED_SCL);
  delay(250);
  display.begin(OLED_I2C_ADDR, true);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.println("Connecting...");
  display.display();

  // Connect to Wifi
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

  // Client Secure configuration
#if defined(ESP8266)
  client.setFingerprint(SPOTIFY_FINGERPRINT);  // These expire every few months
#elif defined(ESP32)
  client.setCACert(spotify_server_cert);
#endif
#if defined(HTTP_INSECURE)
  client.setInsecure();
#endif

  // Time configuration
  waitForSync();
  timezone.setLocation(TIME_ZONE);

  // Get Spotify auth token
  Serial.println("Refreshing Access Tokens");
  if (!spotify.refreshAccessToken()) {
    Serial.println("Failed to get access tokens");
  }

  // Get first weather data
  getWeatherData();
}

void updateSpotifyData(CurrentlyPlaying _currentlyPlaying) {
  currentlyPlaying = _currentlyPlaying;
}

void printCurrentlyPlayingToDisplay() {
  Serial.println("--------- Currently Playing ---------");
  display.clearDisplay();

  // Print TRACK name
  String trackName = String(currentlyPlaying.trackName);
  Serial.println("Track: " + trackName);
  display.setCursor(0, 4);
  display.setFont(&FreeMonoBold9pt7b);
  replaceSpecialCharacters(trackName);
  if (trackName.length() <= MAX_CHAR_TITLE_PER_LINE) {
    display.setCursor(0, 20);
    display.print(trackName);
  } else {
    display.print(trackName.substring(0, MAX_CHAR_TITLE_PER_LINE));
    display.setCursor(0, 24);
    if (trackName.charAt(MAX_CHAR_TITLE_PER_LINE) == ' ') {
      trackName.remove(MAX_CHAR_TITLE_PER_LINE, 1);
    }
    display.print(trackName.substring(MAX_CHAR_TITLE_PER_LINE, MAX_CHAR_TITLE_PER_LINE * 2));
  }

  // Print ARTIST list
  String allArtistNames = "";
  for (int i = 0; i < currentlyPlaying.numArtists; i++) {
    allArtistNames += currentlyPlaying.artists[i].artistName;
    if (i < currentlyPlaying.numArtists - 1) {
      allArtistNames += ", ";
    }
  }
  Serial.println("Artists: " + allArtistNames);
  display.setCursor(0, 36);
  display.setTextSize(1);
  display.setFont();
  replaceSpecialCharacters(allArtistNames);
  display.println(allArtistNames.substring(0, MAX_CHAR_ARTIST_PER_LINE));
  if (allArtistNames.charAt(MAX_CHAR_ARTIST_PER_LINE) == ' ') {
    allArtistNames.remove(MAX_CHAR_ARTIST_PER_LINE, 1);
  }
  display.print(allArtistNames.substring(MAX_CHAR_ARTIST_PER_LINE, MAX_CHAR_ARTIST_PER_LINE * 2));

  // Print progress bar
  long progress = currentlyPlaying.progressMs;
  long duration = currentlyPlaying.durationMs;
  int progress_pixel = map(progress, 0, duration, 0, 126);

  display.drawRect(0, 50, 128, 6, SH110X_WHITE);
  display.fillRect(0, 50, progress_pixel, 6, SH110X_WHITE);

  // Print play or stop button
  display.setCursor(0, 57);
  if (currentlyPlaying.isPlaying) {
    display.write(0x10);  // Play
  } else {
    display.write(0xFD);  // Stop
  }

  // Print elapsed and total time
  char buffer[15];
  sprintf(buffer, "%02lu:%02lu/%02lu:%02lu", progress / 1000 / 60, progress / 1000 % 60, duration / 1000 / 60, duration / 1000 % 60);
  display.setCursor(60, 57);
  display.print(buffer);

  // Refresh display
  display.display();
}

void loop() {
  if (millis() > spotify_latest_request + SPOTIFY_API_DELAY) {
    int status = spotify.getCurrentlyPlaying(updateSpotifyData, SPOTIFY_MARKET);

    if (status == 200) {
      printCurrentlyPlayingToDisplay();
      Serial.println("Successfully got currently playing");
    } else if (status == 204) {
      Serial.println("Doesn't seem to be anything playing");
      printLocalTime();
    } else {
      Serial.print("ERROR! Status " + status);
    }
    spotify_latest_request = millis();
  }

  if (millis() > weather_latest_request + WEATHER_API_DELAY) {
    getWeatherData();
  }
}

void replaceSpecialCharacters(String &str) {
  // Replace lower-case spanish chars
  str.replace("á", "a");
  str.replace("é", "e");
  str.replace("í", "i");
  str.replace("ó", "o");
  str.replace("ú", "u");
  str.replace("ñ", "n");
  str.replace("ü", "u");

  // Replace upper-case spanish chars
  str.replace("Á", "A");
  str.replace("É", "E");
  str.replace("Í", "I");
  str.replace("Ó", "O");
  str.replace("Ú", "U");
  str.replace("Ñ", "N");
  str.replace("Ü", "U");

  // Replace other special chars
  str.replace("Ø", "O");
}


void printLocalTime() {
  // Format and print current time
  display.clearDisplay();
  display.setCursor(20, 5);
  display.setTextSize(3);
  display.print(timezone.dateTime("H:i"));

  // Print weather data
  Serial.println("WEATHER DATA: " + String(weather_data.temp) + "C - " + weather_data.icon_code);
  display.drawBitmap(23, 32, getWeatherIcon(weather_data.icon_code), WI_ICON_WIDTH, WI_ICON_HEIGHT, SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(60, 40);
  char buffer[10];
  snprintf(buffer, sizeof(buffer), "%d", (int)round(weather_data.temp));
  display.print(buffer);
  display.drawBitmap(60 + 2 + strlen(buffer) * 12, 40, degree_bmp, 8, 10, SH110X_WHITE);
  display.setCursor(60 + 2 + 9 + strlen(buffer) * 12, 40);
  display.print("C");

  // Refresh the display  
  display.setTextSize(1);
  display.display();
}

void getWeatherData() {
  String server = "https://api.openweathermap.org/data/2.5/weather?units=metric&q=" + String(WEATHER_QUERY) + "&APPID=" + String(WEATHER_API_KEY);
  String json_array = GET_Request(server.c_str());

  StaticJsonDocument<32> filter;
  filter["weather"] = true;
  filter["main"] = true;
  StaticJsonDocument<384> doc;

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
