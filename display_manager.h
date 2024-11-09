#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <SpotifyArduino.h>
#include <ezTime.h>
#include "weather_manager.h"

extern Adafruit_SH1106G display;
extern Timezone timezone;

void setupDisplay();
void printCurrentlyPlayingToDisplay(CurrentlyPlaying currentlyPlaying);
void printLocalTime(WeatherData weather_data);
void replaceSpecialCharacters(String &str);

#endif 