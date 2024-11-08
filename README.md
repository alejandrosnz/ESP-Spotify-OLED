# ESP8266/ESP32 Spotify OLED Display

This project displays current Spotify playback information and weather data on an OLED display using an ESP8266 or ESP32.

## 🎵 Features

- Shows current Spotify track information including:
  - Song title
  - Artist(s)
  - Progress bar
  - Play/pause status
  - Elapsed/total time
- When no music is playing, displays:
  - Current time
  - Current temperature
  - Weather icon

## 📋 Requirements

### Hardware
- ESP8266 or ESP32 (ex.: D1 Mini)
- SH1106 OLED Display (128x64)

### Connections
- OLED SDA -> D1
- OLED SCL -> D2
- VCC -> 3.3V
- GND -> GND

### Arduino Libraries
Install the following libraries from the Arduino Library Manager:

- [`ArduinoJson 6.21.x`](https://arduinojson.org/v6/doc/installation/)
- [`Spotify API Arduino`](https://github.com/witnessmenow/spotify-api-arduino.git)
- [`Adafruit GFX Library 1.10.9+`](https://github.com/adafruit/Adafruit-GFX-Library)
- [`Adafruit SH110X 2.1.10+`](https://github.com/adafruit/Adafruit_SH110X.git)

You can install the libraries using the Arduino IDE Library Manager (Tools > Manage Libraries...) except for `Spotify API Arduino` which you need to install manually, download the [library repository](https://github.com/witnessmenow/spotify-api-arduino.git) as a ZIP and install it as an external library (Sketch > Include Library > Add .ZIP Library...).

## 🔧 How to Compile

1. Install all required libraries as described above

2. In Arduino IDE:
   - Select your board:
     - For ESP8266: Tools > Board > ESP8266 Boards > NodeMCU 1.0
     - For ESP32: Tools > Board > ESP32 Arduino > ESP32 Dev Module

3. Create the required configuration files:
   - Update `secrets.h` with your WiFi, Spotify and Weather API credentials

4. Verify library dependencies:
   ```cpp
   #include <SpotifyArduino.h>
   #include <SpotifyArduinoCert.h>
   #include <Adafruit_SH110X.h>
   ```
   If you see any errors, double-check that all libraries are properly installed

5. Connect your ESP8266/ESP32 to your computer

6. Select the correct port in Arduino IDE:
   - Tools > Port > [Your ESP Port]

7. Click the "Upload" button or use Sketch > Upload

8. Monitor the upload process in the IDE's console
