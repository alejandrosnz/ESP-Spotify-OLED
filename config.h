#ifndef CONFIG_H
#define CONFIG_H

// Spotify configuration
#define SPOTIFY_MARKET     "ES"      // 2-letter country code https://www.iban.com/country-codes
#define SPOTIFY_API_DELAY  1 * 1000  // 1 second delay between API calls

// Time and weather configuration
#define TIME_ZONE          "Europe/Madrid"  // Timezone code https://timezonedb.com/time-zones
#define WEATHER_QUERY      "Madrid,ES,city" // Weather location https://openweathermap.org/current#name
#define WEATHER_API_DELAY  5 * 60 * 1000    // 5 minutes delay between API calls

// Display configuration
#define MAX_CHAR_TITLE_PER_LINE  11
#define MAX_CHAR_ARTIST_PER_LINE 21

// OLED configuration
#define OLED_SDA D1
#define OLED_SCL D2
#define OLED_I2C_ADDR 0x3c
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_RESET -1

// HTTP configuration
#define HTTP_INSECURE 1

#endif 