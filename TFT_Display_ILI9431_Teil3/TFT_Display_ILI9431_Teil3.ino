#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>

// Das ILI9341-Display verwendet den SPI-Bus des Arduino.
// Die SPI-Pins (MOSI, MISO, SCK) sind hardwarebedingt festgelegt:
// - MOSI (D11 bei Arduino UNO) -> Datenleitung zum Display
// - MISO (D12 bei Arduino UNO) -> Nicht benötigt für das Display
// - SCK (D13 bei Arduino UNO) -> Taktsignal
//
// Zusätzlich benötigen wir drei frei wählbare Pins für Steuerleitungen:
#define TFT_CS     10  // Chip Select (aktiviert das Display für die Kommunikation)
#define TFT_RST    9   // Reset (zurücksetzen des Displays, kann auch an 3,3V angeschlossen werden)
#define TFT_DC     8   // Data/Command (Steuert, ob Daten oder Befehle gesendet werden)

// Initialisiere das Display-Objekt mit den definierten Pins
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

#define BUTTON_PIN 2   // Pin für den Taster
#define POT_PIN A0     // Pin für das Potentiometer

int currentColor = 0; // Variable zur Speicherung der aktuellen Farbe

void setup() {
    // Starte die Kommunikation mit dem Display
    tft.begin();
    
    // Setze die Ausrichtung des Displays (0-3 möglich)
    tft.setRotation(0); // Hochformat
    
    // Setze den Pin für den Taster als Eingang
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    
    // Fülle den Bildschirm mit Schwarz (Hintergrundfarbe)
    tft.fillScreen(ILI9341_BLACK);
    
    // --- Wähle hier die gewünschte Funktion aus, indem du die Kommentarzeichen entfernst ---
    
     //changeBackgroundWithButton(); // Taster verändert die Hintergrundfarbe
     changeCircleSizeWithPot();    // Potentiometer steuert die Größe eines Kreises
}

void loop() {
    // Keine Loop-Funktion nötig, außer für Animationen oder Interaktionen
}

// --- Funktion: Taster verändert die Hintergrundfarbe ---
void changeBackgroundWithButton() {
    while (true) {
        if (digitalRead(BUTTON_PIN) == LOW) { // Taster gedrückt
            currentColor++;
            if (currentColor > 5) {
                currentColor = 0; // Zurücksetzen nach der letzten Farbe
            }
            
            // Wähle Farbe basierend auf currentColor
            switch (currentColor) {
                case 0: tft.fillScreen(ILI9341_BLACK); break;
                case 1: tft.fillScreen(ILI9341_RED); break;
                case 2: tft.fillScreen(ILI9341_GREEN); break;
                case 3: tft.fillScreen(ILI9341_BLUE); break;
                case 4: tft.fillScreen(ILI9341_YELLOW); break;
                case 5: tft.fillScreen(ILI9341_CYAN); break;
            }
            delay(300); // Kleiner Delay, um mehrfaches Auslösen zu verhindern
        }
    }
}

// --- Funktion: Potentiometer steuert die Größe eines Kreises ---
void changeCircleSizeWithPot() {
    while (true) {
        int potValue = analogRead(POT_PIN); // Lese den Wert des Potentiometers
        int radius = map(potValue, 0, 1023, 10, 100); // Skaliere Wert auf 10-100
        
        // Lösche alten Kreis
        tft.fillScreen(ILI9341_BLACK);
        
        // Zeichne neuen Kreis
        tft.fillCircle(120, 160, radius, ILI9341_WHITE);
        
        delay(1000); // Kurze Pause für stabilere Anzeige
    }
}
