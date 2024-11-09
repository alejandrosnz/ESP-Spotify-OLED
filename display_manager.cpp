#include "display_manager.h"
#include "config.h"
#include "weather_icons.h"

Adafruit_SH1106G display = Adafruit_SH1106G(OLED_WIDTH, OLED_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
  Wire.begin(OLED_SDA, OLED_SCL);
  delay(250);
  display.begin(OLED_I2C_ADDR, true);
  display.setTextColor(SH110X_WHITE);
  display.clearDisplay();
  display.println("Connecting...");
  display.display();
}

void printCurrentlyPlayingToDisplay(CurrentlyPlaying currentlyPlaying) {
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

  display.display();
}

void printLocalTime(WeatherData weather_data) {
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

  display.setTextSize(1);
  display.display();
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