#include <FastLED.h>

#define LED_PIN        6
#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define LED_COUNT      (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS     100

#define POTI_LEVEL     A0  // Für Dummy-„Lautstärke“

CRGB leds[LED_COUNT];

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  randomSeed(analogRead(A1)); // ANAlog Eingang z.b. Audiosignal
}

void loop() {
  FastLED.clear();

  int levelFactor = map(analogRead(POTI_LEVEL), 0, 1023, 2, MATRIX_HEIGHT);  // wie laut darf's werden?

  for (int x = 0; x < MATRIX_WIDTH; x++) {
    int height = random(levelFactor, MATRIX_HEIGHT + 1);  // Pegel für Spalte x

    for (int y = 0; y < height; y++) {
      int pixel = getPixelNumber(x, y);
      leds[pixel] = CHSV(map(y, 0, MATRIX_HEIGHT - 1, 96, 0), 255, 255);  // grün → rot
    }
  }

  FastLED.show();
  delay(80);
}

// Zickzack-Mapping
int getPixelNumber(int x, int y) {
  // Flip X und Y für 180° Drehung
  x = MATRIX_WIDTH - 1 - x;
  y = MATRIX_HEIGHT - 1 - y;

  if (x % 2 == 0) {
    return x * MATRIX_HEIGHT + y;
  } else {
    return x * MATRIX_HEIGHT + (MATRIX_HEIGHT - 1 - y);
  }
}

