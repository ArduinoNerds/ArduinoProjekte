#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// MAX7219-Display-Typ (ändern, falls Schrift verdreht: MD_MAX72XX::FC16_HW)
#define HARDWARE_TYPE MD_MAX72XX::PAROLA_HW  
#define MAX_DEVICES 8  // 8 Module (64x8 Pixel)

// Pin-Zuweisungen
#define DATA_PIN   11
#define CLK_PIN    13
#define CS_PIN     10
#define TASTER_PIN 2  // Taster an Pin 2

// Display-Objekt für Laufschrift
MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// Anzuzeigender Text
const char text[] = "ARDUINO NERDS !";

// Liste von Effekten für die Animation
const textEffect_t effects[] = {
    PA_SCROLL_LEFT, PA_SCROLL_RIGHT, PA_BLINDS, PA_WIPE, 
    PA_GROW_UP, PA_GROW_DOWN, PA_FADE, PA_OPENING, PA_SLICE, PA_MESH
};
const int effectCount = sizeof(effects) / sizeof(effects[0]);  
int currentEffect = 0;  // Start-Effekt

void setup() {
    display.begin();
    display.setIntensity(5);  // Helligkeit (0-15)
    display.displayClear();
    display.setSpeed(50);  // Geschwindigkeit der Animation
    display.setPause(500); // Pause zwischen Wiederholungen

    pinMode(TASTER_PIN, INPUT_PULLUP);  // Taster als Eingang mit Pullup

    // Startet die Animation mit dem ersten Effekt
    display.displayText(text, PA_CENTER, 50, 500, effects[currentEffect], effects[currentEffect]);
}

void loop() {
    // Falls Animation abgeschlossen, neustarten
    if (display.displayAnimate()) {
        display.displayReset();
    }

    // Taster gedrückt? -> Nächsten Effekt wählen
    if (digitalRead(TASTER_PIN) == LOW) {
        delay(200);  // Entprellung
        currentEffect = (currentEffect + 1) % effectCount;
        display.displayText(text, PA_CENTER, 50, 500, effects[currentEffect], effects[currentEffect]);
        display.displayReset();
    }
}
