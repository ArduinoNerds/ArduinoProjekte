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

// Drehencoder Pins
#define ENCODER_CLK 2  // Drehencoder Clock-Pin
#define ENCODER_DT 3   // Drehencoder Data-Pin
#define ENCODER_SW 4   // Drehencoder Taster-Pin

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

int menuIndex = 0;  // Menüpunkt-Index
const int menuItems = 5;  // Anzahl der Menüpunkte
String menuOptions[menuItems] = {"Option 1", "Option 2", "Option 3", "Option 4", "Option 5"};

void setup() {
    // Starte die Kommunikation mit dem Display
    tft.begin();
    tft.setRotation(0); // Hochformat
    tft.fillScreen(ILI9341_BLACK);
    
    // Drehencoder Pins als Eingänge setzen
    pinMode(ENCODER_CLK, INPUT_PULLUP);
    pinMode(ENCODER_DT, INPUT_PULLUP);
    pinMode(ENCODER_SW, INPUT_PULLUP);
    
    drawMenu(); // Menü zeichnen
}

void loop() {
    handleEncoder(); // Menü mit Drehencoder steuern
}

// --- Funktion: Menü zeichnen ---
void drawMenu() {
    tft.fillScreen(ILI9341_BLACK); // Bildschirm leeren
    for (int i = 0; i < menuItems; i++) {
        if (i == menuIndex) {
            tft.setTextColor(ILI9341_YELLOW, ILI9341_BLUE); // Markierte Option
        } else {
            tft.setTextColor(ILI9341_WHITE, ILI9341_BLACK);
        }
        tft.setCursor(50, 50 + (i * 30));
        tft.setTextSize(2);
        tft.print(menuOptions[i]);
    }
}

// --- Funktion: Drehencoder-Eingabe behandeln ---
void handleEncoder() {
    static int lastState = HIGH;
    int currentState = digitalRead(ENCODER_CLK);
    if (currentState != lastState && currentState == LOW) {
        if (digitalRead(ENCODER_DT) != currentState) {
            menuIndex++;
            if (menuIndex >= menuItems) menuIndex = 0;
        } else {
            menuIndex--;
            if (menuIndex < 0) menuIndex = menuItems - 1;
        }
        drawMenu();
    }
    lastState = currentState;

    // Wenn der Drehencoder-Knopf gedrückt wird
    if (digitalRead(ENCODER_SW) == LOW) {
        executeMenuOption(menuIndex);
        delay(200); // Verhindert mehrfaches Auslösen
    }
}

// --- Funktion: Aktion für ausgewähltes Menüelement ---
void executeMenuOption(int index) {
    tft.fillScreen(ILI9341_BLACK); // Bildschirm löschen
    
    // Erste Zeile
    tft.setCursor(50, 80);
    tft.setTextColor(ILI9341_GREEN);
    tft.setTextSize(2);
    tft.print("Ausgewaehlt:");
    
    // Zweite Zeile mit der ausgewählten Option
    tft.setCursor(50, 120); 
    tft.setTextColor(ILI9341_CYAN);
    tft.setTextSize(3);
    tft.print(menuOptions[index]); 
    
    delay(2000); // Kurz anzeigen
    drawMenu(); // Zurück zum Menü
}

