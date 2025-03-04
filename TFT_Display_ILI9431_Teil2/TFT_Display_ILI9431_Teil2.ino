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

void setup() {
    // Starte die Kommunikation mit dem Display
    tft.begin();
    
    // Setze die Ausrichtung des Displays (0-3 möglich)
    tft.setRotation(0); // Hochformat
    
    // Fülle den Bildschirm mit Schwarz (Hintergrundfarbe)
    tft.fillScreen(ILI9341_BLACK);
    
    // --- Wähle hier die gewünschte Funktion aus, indem du die Kommentarzeichen entfernst ---
    
    //drawLinesAndRectangles(); // Linien und Rechtecke zeichnen
    //drawCirclesAndEllipses(); // Kreise und Ellipsen zeichnen
    //drawTextExamples();       // Text und Schriften anzeigen
    animateMovingCircle();    // Animation: sich bewegender Kreis
}

void loop() {
    // Keine Loop-Funktion nötig, außer für Animationen
}

// --- Funktion: Linien und Rechtecke zeichnen ---
void drawLinesAndRectangles() {
    tft.drawLine(10, 50, 200, 50, ILI9341_WHITE);
    tft.drawRect(50, 100, 100, 50, ILI9341_GREEN);
    tft.fillRect(70, 180, 80, 40, ILI9341_RED);
}

// --- Funktion: Kreise und Ellipsen zeichnen ---
void drawCirclesAndEllipses() {
    tft.drawCircle(120, 120, 40, ILI9341_CYAN);
    tft.fillCircle(180, 220, 50, ILI9341_YELLOW);
    tft.fillCircle(110,110,30, ILI9341_BLUE);
}

// --- Funktion: Text anzeigen ---
void drawTextExamples() {
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(3);
    tft.setCursor(0, 0);
    tft.print("ArduinoNerds!");
}

void animateMovingCircle() {
    int x = 0;
    int oldX = 0;
    unsigned long previousMillis = 0;
    int interval = 10; // Geschwindigkeit der Animation

    while (x < 240) {
        if (millis() - previousMillis >= interval) {
            previousMillis = millis();
            
            tft.fillCircle(oldX, 160, 10, ILI9341_BLACK);
            tft.fillCircle(x, 160, 10, ILI9341_RED);
            oldX = x;
            x += 1;
        }
    }
}
