#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD-Objekt mit Adresse 0x27 (oder 0x3F, falls nötig)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Taster-Pins
#define TASTER_TEXT 6    // Taster für den Textwechsel
#define TASTER_LIGHT 7   // Taster für die Beleuchtung

// LCD-Hintergrundbeleuchtung (über Software)
bool lichtAn = true;

// Texte für das LCD
String texte[] = {
  "    ArduinoNerds   ",
  "  LCD2004 mit I2C  ",
  "Taster wechselt Text ",
  "   Viel Spass !!!"
};

const int anzahlTexte = sizeof(texte) / sizeof(texte[0]);
int aktuellerText = 0; // Start bei erstem Text

void setup() {
    pinMode(TASTER_TEXT, INPUT_PULLUP);
    pinMode(TASTER_LIGHT, INPUT_PULLUP);

    lcd.init();          // LCD starten
    lcd.backlight();     // Hintergrundbeleuchtung aktivieren
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print(texte[aktuellerText]); // Ersten Text anzeigen
}

void loop() {
    // Taster 1: Text wechseln
    if (digitalRead(TASTER_TEXT) == LOW) {
        delay(200);
        while (digitalRead(TASTER_TEXT) == LOW); 
        
        aktuellerText++;
        if (aktuellerText >= anzahlTexte) aktuellerText = 0;
        
        lcd.clear();
        lcd.setCursor(0, 1);
        lcd.print(texte[aktuellerText]);
    }

    // Taster 2: Hintergrundbeleuchtung ein/aus
    if (digitalRead(TASTER_LIGHT) == LOW) {
        delay(200);
        while (digitalRead(TASTER_LIGHT) == LOW);

        lichtAn = !lichtAn;
        if (lichtAn) {
            lcd.backlight();
        } else {
            lcd.noBacklight();
        }
    }
}
