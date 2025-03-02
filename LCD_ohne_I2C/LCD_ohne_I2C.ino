#include <LiquidCrystal.h> // LCD-Bibliothek einbinden

// Pin-Zuweisungen für das LCD
#define RS 12
#define E  11
#define D4 5
#define D5 4
#define D6 3
#define D7 2

// Taster-Pins
#define TASTER_TEXT 6    // Taster zum Wechseln des Textes
#define TASTER_LIGHT 7   // Taster für die Beleuchtung

// Pin für Beleuchtung
#define LCD_LIGHT 8      // LCD-Hintergrundbeleuchtung

// LCD-Objekt erstellen
LiquidCrystal lcd(RS, E, D4, D5, D6, D7);

// Liste der Texte (alle in Zeile 2)
String texte[] = {
  "    ArduinoNerds   ",
  "  LCD2004 Ohne I2C ",
  "Taster wechselt Text ",
  "   Viel Spass !!!"
};

// Anzahl der Texte
const int anzahlTexte = sizeof(texte) / sizeof(texte[0]);
int aktuellerText = 0; // Start bei erstem Text
bool lichtAn = true; // Standard: Licht an

void setup() {
    pinMode(TASTER_TEXT, INPUT_PULLUP);  // Taster 1 mit internem Pullup aktivieren
    pinMode(TASTER_LIGHT, INPUT_PULLUP); // Taster 2 mit internem Pullup aktivieren
    pinMode(LCD_LIGHT, OUTPUT);  // Pin für LCD-Hintergrundbeleuchtung als Ausgang setzen
    
    digitalWrite(LCD_LIGHT, HIGH); // Beleuchtung standardmäßig einschalten

    lcd.begin(20, 4);  // LCD starten
    lcd.clear();
    lcd.setCursor(0, 1);  // Text in **zweite Zeile (Zeile 1)**
    lcd.print(texte[aktuellerText]); // Ersten Text anzeigen
}

void loop() {
    // Taster 1: Text wechseln
    if (digitalRead(TASTER_TEXT) == LOW) { // Wenn Taster 1 gedrückt
        delay(200); // Entprellen (Verhindert Doppelklick)
        while (digitalRead(TASTER_TEXT) == LOW); // Warten, bis Taster losgelassen wird
        
        aktuellerText++;  // Zum nächsten Text wechseln
        if (aktuellerText >= anzahlTexte) aktuellerText = 0; // Zurück auf den ersten Text
        
        lcd.clear(); // Display leeren
        lcd.setCursor(0, 1);  // **Text in zweite Zeile setzen**
        lcd.print(texte[aktuellerText]); // Neuen Text anzeigen
    }

    // Taster 2: LCD-Beleuchtung ein/aus schalten
    if (digitalRead(TASTER_LIGHT) == LOW) { // Wenn Taster 2 gedrückt
        delay(200); // Entprellen
        while (digitalRead(TASTER_LIGHT) == LOW); // Warten, bis Taster losgelassen wird
        
        lichtAn = !lichtAn; // Zustand umschalten
        digitalWrite(LCD_LIGHT, lichtAn ? HIGH : LOW); // Licht an oder aus
    }
}
