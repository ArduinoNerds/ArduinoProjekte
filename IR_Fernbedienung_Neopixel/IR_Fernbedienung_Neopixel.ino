#include <IRremote.h>           // Bibliothek für den IR-Empfänger
#include <Wire.h>               // I2C-Kommunikation für das LCD
#include <LiquidCrystal_I2C.h>  // Bibliothek für das LCD2004 mit I2C
#include <Adafruit_NeoPixel.h>  // Bibliothek für NeoPixel

#define PIN_RECEIVER 2  // IR-Empfänger an Pin D2
#define PIN_NEOPIXEL 6  // NeoPixel an Pin D6
#define NUM_LEDS 32     // Anzahl der LEDs

// LCD 20x4 über I2C mit Adresse 0x27 (falls nötig, 0x3F testen)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Initialisierung der NeoPixel
Adafruit_NeoPixel strip(NUM_LEDS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// IR-Empfänger initialisieren
IRrecv receiver(PIN_RECEIVER);
decode_results results;
int brightness = 255; // Standardhelligkeit
bool runningAnimation = false; // Variable für Lauflicht

void setup() {
    lcd.init();             // LCD initialisieren
    lcd.backlight();        // Hintergrundbeleuchtung aktivieren
    strip.begin();          // NeoPixel starten
    strip.setBrightness(brightness);
    strip.show();           // Alle LEDs ausschalten
    
    lcd.setCursor(0, 0);
    lcd.print("<Taste druecken>");
    receiver.enableIRIn();  // IR-Empfänger aktivieren
}

void loop() {
    if (receiver.decode()) {
        translateIR();       // IR-Code auswerten
        receiver.resume();   // Empfänger für das nächste Signal bereitmachen
    }
    
    if (runningAnimation) {
        runAnimationLoop();
    }
}

void translateIR() {
    unsigned long hexCode = receiver.decodedIRData.decodedRawData; 
    int commandCode = receiver.decodedIRData.command; 

    switch (commandCode) {
        case 162: clearLEDs(hexCode, commandCode); break; // Power -> Alles ausschalten
        case 226: lcdPrint("Menue", hexCode, commandCode); break; // Menü-Taste -> Nur Display
        case 34: blinkAllLeds(hexCode, commandCode); break; // Test -> Alle LEDs blinken in Farben
        case 2: increaseBrightness(hexCode, commandCode); break; // Plus-Taste -> Helligkeit hoch
        case 194: lcdPrint("Zurueck", hexCode, commandCode); break; // Pfeil zurück -> Nur Display
        case 224: lcdPrint("Zurueck spulen", hexCode, commandCode); break; // Zurückspulen -> Nur Display
        case 168: runningAnimation = !runningAnimation; if (!runningAnimation) { clearLEDs(hexCode, commandCode); } lcdPrint("Lauflicht", hexCode, commandCode); break; // Play -> Lauflicht
        case 144: lcdPrint("Vorspulen", hexCode, commandCode); break; // Vorspulen -> Nur Display
        case 104: blinkAllLeds(hexCode, commandCode); break; // 0 -> Alle LEDs leuchten oder blinken
        case 152: decreaseBrightness(hexCode, commandCode); break; // - -> Helligkeit verringern
        case 176: resetAll(hexCode, commandCode); break; // C -> Alles zurücksetzen
        case 48: lightUpLEDs(1, hexCode, commandCode); break; // 1 LED an
        case 24: lightUpLEDs(2, hexCode, commandCode); break; // 2 LEDs an
        case 122: lightUpLEDs(3, hexCode, commandCode); break; // 3 LEDs an
        case 16: lightUpLEDs(4, hexCode, commandCode); break; // 4 LEDs an
        case 56: lightUpLEDs(5, hexCode, commandCode); break; // 5 LEDs an
        case 90: lightUpLEDs(6, hexCode, commandCode); break; // 6 LEDs an
        case 66: lightUpLEDs(7, hexCode, commandCode); break; // 7 LEDs an
        case 74: lightUpLEDs(8, hexCode, commandCode); break; // 8 LEDs an
        case 82: lightUpLEDs(9, hexCode, commandCode); break; // 9 LEDs an
        default: lcdPrint("Unbekannter Code", hexCode, commandCode);
    }
}

void lcdPrint(const char* text, unsigned long hexCode, int commandCode) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Taste gedrueckt:");
    lcd.setCursor(0, 1);
    lcd.print(text);
    lcd.setCursor(0, 2);
    lcd.print("HEX: 0x");
    lcd.print(hexCode, HEX); 
    lcd.setCursor(0, 3);
    lcd.print("CMD: ");
    lcd.print(commandCode);
}

void clearLEDs(unsigned long hexCode, int commandCode) {
    strip.clear();
    strip.show();
    runningAnimation = false;
    lcdPrint("Alle LEDs aus", hexCode, commandCode);
}

void blinkAllLeds(unsigned long hexCode, int commandCode) {
    for (int i = 0; i < 5; i++) {
        strip.fill(strip.Color(random(255), random(255), random(255)));
        strip.show();
        delay(500);
        strip.clear();
        strip.show();
        delay(500);
    }
    lcdPrint("LEDs blinken", hexCode, commandCode);
}

void resetAll(unsigned long hexCode, int commandCode) {
    strip.clear();
    strip.setBrightness(150);
    strip.show();
    runningAnimation = false;
    lcdPrint("Reset", hexCode, commandCode);
}

void increaseBrightness(unsigned long hexCode, int commandCode) {
    brightness = min(brightness + 25, 255);
    strip.setBrightness(brightness);
    strip.show();
    lcdPrint("Helligkeit +", hexCode, commandCode);
}

void decreaseBrightness(unsigned long hexCode, int commandCode) {
    brightness = max(brightness - 25, 0);
    strip.setBrightness(brightness);
    strip.show();
    lcdPrint("Helligkeit -", hexCode, commandCode);
}

void runAnimationLoop() {
    for (int i = 0; i < NUM_LEDS; i++) {
        strip.clear();
        strip.setPixelColor(i, strip.Color(0, 255, 0)); // Grün leuchtend
        strip.show();
        delay(100); // Verzögerung für bessere Sichtbarkeit
    }
    
    delay(300); // Kurze Pause am Ende der Runde
    clearLEDs(0, 0); // LEDs ausschalten
    runningAnimation = false; // Nach einer Runde stoppen
}



void lightUpLEDs(int numLeds, unsigned long hexCode, int commandCode) {
    strip.clear();
    for (int i = 0; i < numLeds; i++) {
        strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
    lcdPrint("LEDs an", hexCode, commandCode);
}
