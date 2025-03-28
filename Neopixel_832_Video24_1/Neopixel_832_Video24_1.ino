#include <Adafruit_NeoPixel.h>

#define CLK 2   // Dreh-Encoder: Clock-Pin
#define DT 3    // Dreh-Encoder: Data-Pin
#define SW 4    // Dreh-Encoder: Taster-Pin
#define LED_PIN 6  // NeoPixel Daten-Pin
#define BRIGHTNESS     100

#define NUM_LEDS 256  // Anzahl der LEDs im NeoPixel-Ring
#define DEBOUNCE_TIME 5  // Entprellzeit für den Encoder (in Millisekunden)

Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

int currentPos = 0;   // Aktuelle LED-Position
bool savedLEDs[NUM_LEDS] = {false};  // Array für gespeicherte LEDs
int lastStateCLK;  
unsigned long lastButtonPress = 0;  // Zeitstempel für Entprellung
unsigned long lastEncoderTurn = 0;  // Entprellzeit für den Encoder

void setup() {
    pinMode(CLK, INPUT);
    pinMode(DT, INPUT);
    pinMode(SW, INPUT_PULLUP);  // Interner Pullup für den Taster

    ring.begin();
    ring.show();  // Alle LEDs ausschalten

    lastStateCLK = digitalRead(CLK);  // Startzustand des Encoders speichern
}

void loop() {
    int currentStateCLK = digitalRead(CLK);
    
    // **Drehung nur erkennen, wenn CLK von HIGH → LOW wechselt**
    if (lastStateCLK == HIGH && currentStateCLK == LOW) {
        // **Drehrichtung prüfen und Korrektur vornehmen**
        if (digitalRead(DT) == HIGH) { 
            currentPos++;  // Drehung nach rechts → nächste LED
        } else {
            currentPos--;  // Drehung nach links → vorherige LED
        }

        // Begrenzung: Immer im Bereich 0–31 bleiben
        currentPos = (currentPos + NUM_LEDS) % NUM_LEDS;

        updateRing();
    }

    lastStateCLK = currentStateCLK; // Speichern des letzten Zustands

    // **Taster gedrückt?**
    if (digitalRead(SW) == LOW) {
        if (millis() - lastButtonPress > 300) {  // Entprellung: min. 300ms zwischen den Klicks
            savedLEDs[currentPos] = !savedLEDs[currentPos];  // **LED umschalten (AN/AUS)**
            
            updateRing(); // LED-Status aktualisieren
            lastButtonPress = millis();
        }
    }

    delay(1); // Sehr kurze Wartezeit für flüssige Bewegung
}

// **LED-Ring aktualisieren**
void updateRing() {
    ring.clear();  // Alle LEDs ausschalten

    // **Gespeicherte LEDs aktivieren**
    for (int i = 0; i < NUM_LEDS; i++) {
        if (savedLEDs[i]) {
            ring.setPixelColor(i, ring.Color(0, 255, 0));  // **Gespeicherte LEDs grün**
        }
    }

    // **Aktuelle Position als rote LED**
    ring.setPixelColor(currentPos, ring.Color(0, 0, 255));
    ring.show();
}
