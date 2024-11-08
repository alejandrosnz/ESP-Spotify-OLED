#ifndef CONFIG_H
#define CONFIG_H

// Spotify configuration
#define SPOTIFY_MARKET     "ES"
#define SPOTIFY_API_DELAY  1 * 1000  // 1 second

// Time configuration
#define TIME_NTP_SERVER    "europe.pool.ntp.org"
#define TIME_GMT_OFFSET    0      // UCT+1
#define TIME_SUMMER_OFFSET 3600   // UCT+2 in summer

// Weather configuration
#define WEATHER_QUERY      "Madrid,ES,city"
#define WEATHER_API_DELAY  5 * 60 * 1000 // 5 minutes

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