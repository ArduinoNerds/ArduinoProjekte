#include <Adafruit_NeoPixel.h>

// **Hardware-Pins definieren**
#define TRIG_PIN   9   // Ultraschallsensor Trig-Pin
#define ECHO_PIN   10  // Ultraschallsensor Echo-Pin
#define LED_PIN    6   // NeoPixel-Ring Steuerungs-Pin
#define BUZZER_PIN 3   // Pin für den Buzzer

#define NUM_LEDS   32  // Anzahl der LEDs im NeoPixel-Ring
#define MAX_DIST   400 // Maximale Entfernung für Anzeige
#define ALERT_DIST 10  // Ab welcher Distanz die LED dauerleuchtet & der Buzzer Dauerton hat

// **Erstelle das NeoPixel-Objekt für den LED-Ring**
Adafruit_NeoPixel ring(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    
    ring.begin();
    ring.show(); // Alle LEDs ausschalten
}

void loop() {
    float distance = measureDistance(); // Entfernung messen

    if (distance == -1) {
        setRingColor(0, 255, 0); // Falls kein Echo → Grün
        noTone(BUZZER_PIN);
    }
    else if (distance > 60) {
        setRingColor(0, 255, 0); // **Grün → Objekt weit entfernt**
        noTone(BUZZER_PIN);
    } 
    else if (distance > 35) {
        setRingColor(255, 255, 0); // **Gelb → Objekt mittlere Entfernung**
        noTone(BUZZER_PIN);
    } 
    else if (distance > ALERT_DIST) { 
        blinkRedWithBuzzer(distance); // **Blinken + pulsierender Piepton zwischen 35 cm und 10 cm**
    }
    else {
        alertRedWithBuzzer(); // **Dauerleuchten & Dauerton unter 10 cm**
    }

    delay(100); // Kurze Wartezeit für Stabilität
}

// **Funktion zur Entfernungsmessung mit Ultraschallsensor**
float measureDistance() {
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);
    
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout nach 30ms (~500 cm)
    
    if (duration == 0) return -1; // Falls kein Echo → Sonderfall

    float distance = duration * 0.034 / 2; // Berechnung der Entfernung in cm
    return (distance > MAX_DIST) ? MAX_DIST : distance; // Max. Distanz begrenzen
}

// **Funktion zur Steuerung der LED-Farbe**
void setRingColor(int r, int g, int b) {
    static int lastR = -1, lastG = -1, lastB = -1;
    if (r == lastR && g == lastG && b == lastB) return; // Nur aktualisieren, wenn nötig

    for (int i = 0; i < NUM_LEDS; i++) {
        ring.setPixelColor(i, ring.Color(r, g, b));
    }
    ring.show();

    lastR = r;
    lastG = g;
    lastB = b;
}

// **Funktion für blinkende LEDs + pulsierenden Buzzer (35 cm bis ALERT_DIST)**
void blinkRedWithBuzzer(float distance) {
    int blinkDelay = map(distance, 35, ALERT_DIST, 500, 150); // Je näher, desto schneller (500ms → 150ms)
    int toneFreq = map(distance, 35, ALERT_DIST, 800, 1500); // Je näher, desto höherer Ton

    setRingColor(255, 0, 0);
    tone(BUZZER_PIN, toneFreq, blinkDelay - 50); 

    delay(blinkDelay);  
    setRingColor(0, 0, 0); 
    noTone(BUZZER_PIN);

    delay(blinkDelay); 
}

// **Funktion für dauerhafte rote LEDs + Dauerton unter ALERT_DIST**
void alertRedWithBuzzer() {
    setRingColor(255, 0, 0); // LEDs dauerhaft rot
    tone(BUZZER_PIN, 2000);  // Dauerton mit hoher Frequenz
}
