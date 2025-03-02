v#include <Adafruit_NeoPixel.h>

#define CLK1 2   // Erster Encoder: Clock-Pin
#define DT1 3    // Erster Encoder: Data-Pin
#define SW1 4    // Erster Encoder: Taster-Pin

#define CLK2 5   // Zweiter Encoder: Clock-Pin (Farbauswahl)
#define DT2 6    // Zweiter Encoder: Data-Pin
#define SW2 7    // Zweiter Encoder: Taster-Pin (Farbe bestätigen)

#define LED_PIN 9  // NeoPixel-Ring Daten-Pin
#define NUM_LEDS 32  // Anzahl der LEDs im NeoPixel-Ring
#define DEBOUNCE_TIME 5  // Entprellzeit für die Encoder

Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentPos = 0;   // Aktuelle LED-Position
int colorIndex = 0;   // Aktuelle Farbe (Index)
unsigned long lastButtonPress1 = 0;
unsigned long lastButtonPress2 = 0;
unsigned long lastEncoderTurn = 0;

int colors[][3] = { // **Farbpalette
    {255, 0, 0},   // Rot
    {0, 255, 0},   // Grün
    {0, 0, 255},   // Blau
    {255, 255, 0}, // Gelb
    {0, 255, 255}, // Cyan
    {255, 0, 255}, // Magenta
    {255, 165, 0}, // Orange
    {255, 255, 255} // Weiß
};

int savedLEDs[NUM_LEDS][3] = {{0}}; // Speichert Farben für jede LED

void setup() {
    pinMode(CLK1, INPUT);
    pinMode(DT1, INPUT);
    pinMode(SW1, INPUT_PULLUP);

    pinMode(CLK2, INPUT);
    pinMode(DT2, INPUT);
    pinMode(SW2, INPUT_PULLUP);

    ring.begin();
    ring.show(); // Alle LEDs ausschalten
}

void loop() {
    static int lastStateCLK1 = digitalRead(CLK1);
    static int lastStateCLK2 = digitalRead(CLK2);

    // **Erster Encoder: LED-Position ändern**
    int currentStateCLK1 = digitalRead(CLK1);
    if (lastStateCLK1 == HIGH && currentStateCLK1 == LOW) {
        if (digitalRead(DT1) == HIGH) {
            currentPos++;  // Rechts drehen → nächste LED
        } else {
            currentPos--;  // Links drehen → vorherige LED
        }
        currentPos = (currentPos + NUM_LEDS) % NUM_LEDS; // Begrenzung 0-31
        updateRing();
    }
    lastStateCLK1 = currentStateCLK1;

    // **Zweiter Encoder: Farbe ändern**
    int currentStateCLK2 = digitalRead(CLK2);
    if (lastStateCLK2 == HIGH && currentStateCLK2 == LOW) {
        if (digitalRead(DT2) == HIGH) {
            colorIndex = (colorIndex + 1) % (sizeof(colors) / sizeof(colors[0]));  // Nächste Farbe
        } else {
            colorIndex = (colorIndex - 1 + (sizeof(colors) / sizeof(colors[0]))) % (sizeof(colors) / sizeof(colors[0]));  // Vorherige Farbe
        }
        updateRing();
    }
    lastStateCLK2 = currentStateCLK2;

    // **Button auf Encoder 1 → LED ein/aus**
    if (digitalRead(SW1) == LOW && millis() - lastButtonPress1 > 300) {
        if (savedLEDs[currentPos][0] == 0 && savedLEDs[currentPos][1] == 0 && savedLEDs[currentPos][2] == 0) {
            savedLEDs[currentPos][0] = colors[colorIndex][0];
            savedLEDs[currentPos][1] = colors[colorIndex][1];
            savedLEDs[currentPos][2] = colors[colorIndex][2];
        } else {
            savedLEDs[currentPos][0] = 0;
            savedLEDs[currentPos][1] = 0;
            savedLEDs[currentPos][2] = 0;
        }
        updateRing();
        lastButtonPress1 = millis();
    }

    // **Button auf Encoder 2 → Farbe für LED speichern**
    if (digitalRead(SW2) == LOW && millis() - lastButtonPress2 > 300) {
        savedLEDs[currentPos][0] = colors[colorIndex][0];
        savedLEDs[currentPos][1] = colors[colorIndex][1];
        savedLEDs[currentPos][2] = colors[colorIndex][2];
        updateRing();
        lastButtonPress2 = millis();
    }

    delay(1);
}

// **LED-Ring aktualisieren**
void updateRing() {
    ring.clear();  // Alle LEDs ausschalten

    // **Gespeicherte LEDs anzeigen**
    for (int i = 0; i < NUM_LEDS; i++) {
        if (savedLEDs[i][0] != 0 || savedLEDs[i][1] != 0 || savedLEDs[i][2] != 0) {
            ring.setPixelColor(i, ring.Color(savedLEDs[i][0], savedLEDs[i][1], savedLEDs[i][2]));
        }
    }

    // **Aktuelle LED als Vorschau in der neuen Farbe anzeigen**
    ring.setPixelColor(currentPos, ring.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]));
    ring.show();
}
