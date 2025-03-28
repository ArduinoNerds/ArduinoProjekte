#include <FastLED.h>

#define LED_PIN        6
#define MATRIX_WIDTH   32
#define MATRIX_HEIGHT  8
#define LED_COUNT      (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS     100

#define POTI_HUE       A0
#define POTI_SPEED     A1

#define CLK 2
#define DT 3
#define SW 4

CRGB leds[LED_COUNT];

int direction = 1; // 1 = rechts, -1 = links
int frame = 0;

void setup() {
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);

  FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
}

void loop() {
  // --- Potis lesen ---
  int hue = map(analogRead(POTI_HUE), 0, 1023, 0, 255);
  int speed = map(analogRead(POTI_SPEED), 0, 1023, 20, 200);

  // --- Encoder lesen für Richtungsumschaltung ---
  static int lastClk = HIGH;
  int clk = digitalRead(CLK);
  if (clk != lastClk && clk == LOW) {
    int dt = digitalRead(DT);
    if (dt != clk) {
      direction = 1; // rechts
    } else {
      direction = -1; // links
    }
  }
  lastClk = clk;

  // --- LEDs berechnen ---
  FastLED.clear();

  for (int x = 0; x < MATRIX_WIDTH; x++) {
    // Wellenfunktion: sin + Laufzeit
    float wave = sin((x + frame) * 0.3) * 3.5 + 4;  // 0–7
    int y = constrain((int)wave, 0, MATRIX_HEIGHT - 1);
    int pixel = getPixelNumber(x, y);
    leds[pixel] = CHSV(hue, 255, 255);
  }

  FastLED.show();
  frame += direction;
  delay(speed);
}

// --- Zickzack-Mapping ---
int getPixelNumber(int x, int y) {
  if (x % 2 == 0) {
    return x * MATRIX_HEIGHT + y;
  } else {
    return x * MATRIX_HEIGHT + (MATRIX_HEIGHT - 1 - y);
  }
}
