#include <FastLED.h>

#define LED_PIN 6
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define LED_COUNT (MATRIX_WIDTH * MATRIX_HEIGHT)

#define POTI_SPEED A0   // Geschwindigkeit
#define POTI_COLOR A1   // Farbe
#define BRIGHTNESS 100  // Feste Helligkeit

CRGB leds[LED_COUNT];

// **Mapping für dein 8x32 Zickzack-Panel**
int getPixelNumber(int x, int y) {
    if (x % 2 == 0) {
        return (x * MATRIX_HEIGHT) + y;
    } else {
        return (x * MATRIX_HEIGHT) + (MATRIX_HEIGHT - 1 - y);
    }
}

void setup() {
    FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.clear();
}

void loop() {
    for (int i = 0; i < LED_COUNT; i++) {
        // **Poti-Werte in Echtzeit auslesen**
        int potiSpeed = analogRead(POTI_SPEED);
        int delayTime = map(potiSpeed, 0, 1023, 10, 500);  // Geschwindigkeit zwischen 10ms und 500ms

        int potiColor = analogRead(POTI_COLOR);
        int hue = map(potiColor, 0, 1023, 0, 255);  // Farbe zwischen 0-255

        leds[i] = CHSV(hue, 255, 255);  // Setzt die Farbe basierend auf Poti-Wert
        FastLED.show();
        delay(delayTime);

        // **Pixel löschen, damit sie einzeln nacheinander leuchten**
        leds[i] = CRGB::Black;
    }
}
