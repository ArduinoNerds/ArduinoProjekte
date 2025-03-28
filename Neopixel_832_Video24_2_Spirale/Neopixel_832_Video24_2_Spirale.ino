#include <FastLED.h>

#define LED_PIN        6
#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define LED_COUNT      (MATRIX_WIDTH * MATRIX_HEIGHT)

#define POTI_SPEED     A0
#define POTI_COLOR     A1
#define BRIGHTNESS     100

CRGB leds[LED_COUNT];

// --- Zickzack-Mapping ---
int getPixelNumber(int x, int y) {
  if (x % 2 == 0) {
    return (x * MATRIX_HEIGHT) + y;
  } else {
    return (x * MATRIX_HEIGHT) + (MATRIX_HEIGHT - 1 - y);
  }
}

// --- Spiral-Koordinaten-Generator ---
void generateSpiral(int coords[][2]) {
  int left = 0, right = MATRIX_WIDTH - 1;
  int top = 0, bottom = MATRIX_HEIGHT - 1;
  int index = 0;

  while (left <= right && top <= bottom) {
    // oben → rechts
    for (int x = left; x <= right; x++) {
      coords[index][0] = x;
      coords[index][1] = top;
      index++;
    }
    top++;

    // rechts runter
    for (int y = top; y <= bottom; y++) {
      coords[index][0] = right;
      coords[index][1] = y;
      index++;
    }
    right--;

    // unten ← links
    for (int x = right; x >= left; x--) {
      coords[index][0] = x;
      coords[index][1] = bottom;
      index++;
    }
    bottom--;

    // links hoch ↑
    for (int y = bottom; y >= top; y--) {
      coords[index][0] = left;
      coords[index][1] = y;
      index++;
    }
    left++;
  }
}

void setup() {
  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  int coords[LED_COUNT][2];
  generateSpiral(coords);  // einmal spiralreihenfolge berechnen

  // LEDs spiralförmig EINschalten
  for (int i = 0; i < LED_COUNT; i++) {
    int x = coords[i][0];
    int y = coords[i][1];
    int pixel = getPixelNumber(x, y);

    int potiHue = analogRead(POTI_COLOR);
    int hue = map(potiHue, 0, 1023, 0, 255);

    leds[pixel] = CHSV(hue, 255, 255);
    FastLED.show();

    int potiSpeed = analogRead(POTI_SPEED);
    int delayTime = map(potiSpeed, 0, 1023, 5, 150);
    delay(delayTime);
  }

  delay(1000);

  // LEDs spiralförmig wieder AUSschalten
  for (int i = 0; i < LED_COUNT; i++) {
    int x = coords[i][0];
    int y = coords[i][1];
    int pixel = getPixelNumber(x, y);

    leds[pixel] = CRGB::Black;
    FastLED.show();

    int potiSpeed = analogRead(POTI_SPEED);
    int delayTime = map(potiSpeed, 0, 1023, 5, 150);
    delay(delayTime);
  }

  delay(1000);
}
