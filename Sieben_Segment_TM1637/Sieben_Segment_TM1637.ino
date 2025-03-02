v#include <TM1637.h>  // TM1637 Bibliothek für Wokwi

// Pins für TM1637 Display
#define CLK 2
#define DIO 3
TM1637 display(CLK, DIO);

// Taster-Pins
#define BUTTON_UP 4
#define BUTTON_DOWN 5

int number = 0;  // Startwert

void setup() {
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  
  display.set(3);  // Helligkeit setzen (0-7)
  display.init();  // Display initialisieren
  updateDisplay();  // Anzeige starten
}

void loop() {
  // Hochzählen, wenn Taster 1 gedrückt wird
  if (digitalRead(BUTTON_UP) == LOW) {  
    number++;  
    if (number > 9999) number = 0;  // Begrenzung auf 0-9999
    updateDisplay();
    delay(200);  // Entprellung
  }

  // Runterzählen, wenn Taster 2 gedrückt wird
  if (digitalRead(BUTTON_DOWN) == LOW) {  
    number--;  
    if (number < 0) number = 9999;  // Begrenzung auf 0-9999
    updateDisplay();
    delay(200);  // Entprellung
  }
}

// Funktion zum Aktualisieren des Displays mit 4-stelliger Anzeige
void updateDisplay() {
  int digits[] = {number / 1000, (number / 100) % 10, (number / 10) % 10, number % 10};
  for (int i = 0; i < 4; i++) {
    display.display(i, digits[i]);  // Alle 4 Stellen befüllen
  }
}
