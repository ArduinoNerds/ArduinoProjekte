#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
#define MATRIX_WIDTH 32
#define MATRIX_HEIGHT 8
#define LED_COUNT (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS 50
#define DROP_CHANCE 20     // 0–100: Wahrscheinlichkeit, dass in Spalte ein Tropfen erscheint
#define FALL_DELAY 80      // Zeit zwischen Schritten (ms)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

uint32_t rainColor = strip.Color(0, 0, 255);  // Startfarbe Blau
int hue = 0;  // Für Regenbogen-Color-Modus

void setup() {
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show();
}

void loop() {
  // Alles eine Zeile nach unten verschieben
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    for (int y = MATRIX_HEIGHT - 1; y > 0; y--) {
      int current = getPixelNumber(x, y);
      int above = getPixelNumber(x, y - 1);
      strip.setPixelColor(current, strip.getPixelColor(above));
    }
  }

  // Neue Tropfen ganz oben hinzufügen
  for (int x = 0; x < MATRIX_WIDTH; x++) {
    if (random(100) < DROP_CHANCE) {
      hue += 15;  // Farbe drehen
      rainColor = colorFromHue(hue);
      int topPixel = getPixelNumber(x, 0);
      strip.setPixelColor(topPixel, rainColor);
    } else {
      int topPixel = getPixelNumber(x, 0);
      strip.setPixelColor(topPixel, 0);  // Kein Tropfen
    }
  }

  strip.show();
  delay(FALL_DELAY);
}

// Regenbogen-Farbe aus Hue generieren
uint32_t colorFromHue(int hue) {
  hue = hue % 256;
  int r, g, b;
  if (hue < 85) {
    r = hue * 3;
    g = 255 - hue * 3;
    b = 0;
  } else if (hue < 170) {
    hue -= 85;
    r = 255 - hue * 3;
    g = 0;
    b = hue * 3;
  } else {
    hue -= 170;
    r = 0;
    g = hue * 3;
    b = 255 - hue * 3;
  }
  return strip.Color(r, g, b);
}

// Zickzack-Mapping berücksichtigen
int getPixelNumber(int x, int y) {
  if (x % 2 == 0) {
    return (x * MATRIX_HEIGHT) + y;
  } else {
    return (x * MATRIX_HEIGHT) + (MATRIX_HEIGHT - 1 - y);
  }
}
