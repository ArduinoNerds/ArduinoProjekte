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
    
    // Zeichne die Display-Koordinaten zur Orientierung
    drawDisplayGrid();
}

void loop() {
    // Keine Loop-Funktion nötig, da das Bild statisch bleibt
}

// Funktion zum Zeichnen der Display-Koordinaten
void drawDisplayGrid() {
    // Markiere die vier Ecken
    tft.drawPixel(0, 0, ILI9341_RED);       // Oben links
    tft.drawPixel(239, 0, ILI9341_GREEN);  // Oben rechts
    tft.drawPixel(0, 319, ILI9341_BLUE);   // Unten links
    tft.drawPixel(239, 319, ILI9341_YELLOW); // Unten rechts
    
    // Setze die Zahlen an die passenden Positionen
    tft.setTextColor(ILI9341_RED);
    tft.setTextSize(2);
    tft.setCursor(5, 5);
    tft.print("0,0"); // Oben links
    
    tft.setTextColor(ILI9341_GREEN);
    tft.setCursor(180, 5);
    tft.print("239,0"); // Oben rechts
    
    tft.setTextColor(ILI9341_BLUE);
    tft.setCursor(5, 300);
    tft.print("0,319"); // Unten links
    
    tft.setTextColor(ILI9341_YELLOW);
    tft.setCursor(150, 300); 
    tft.print("239,319"); // Unten rechts
    
    // Markiere die Mitte
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(70, 150);
    tft.print("ArduinoNerds");
    tft.drawPixel(120, 160, ILI9341_WHITE); // Mittelpunkt
}
