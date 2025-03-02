#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// LCD2004 I2C-Initialisierung (Adresse 0x27)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Anzahl der Zeilen und Spalten des Keypads
const byte ROWS = 4;  
const byte COLS = 4;  

// Keypad-Tastenbelegung (4 Zeilen x 4 Spalten)
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Einzelne Definition der Zeilen-Pins
const byte row1 = 2;  // Zeile 1 des Keypads an Pin 2
const byte row2 = 3;  // Zeile 2 des Keypads an Pin 3
const byte row3 = 4;  // Zeile 3 des Keypads an Pin 4
const byte row4 = 5;  // Zeile 4 des Keypads an Pin 5

// Einzelne Definition der Spalten-Pins
const byte col1 = 6;  // Spalte 1 des Keypads an Pin 6
const byte col2 = 7;  // Spalte 2 des Keypads an Pin 7
const byte col3 = 8;  // Spalte 3 des Keypads an Pin 8
const byte col4 = 9;  // Spalte 4 des Keypads an Pin 9

// Array für die Pins
byte rowPins[ROWS] = {row1, row2, row3, row4};  
byte colPins[COLS] = {col1, col2, col3, col4};  

// Erstellen des Keypad-Objekts
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // LCD initialisieren
  lcd.init();  
  lcd.backlight();  
  lcd.setCursor(0, 0);
  lcd.print("Keypad Test");
}

void loop() {
  // Prüfen, ob eine Taste gedrückt wurde
  char key = keypad.getKey();

  // Falls eine Taste gedrückt wurde, diese auf dem LCD anzeigen
  if (key) {
    lcd.clear();  
    lcd.setCursor(0, 0);
    lcd.print("Taste: ");
    lcd.print(key);
    delay(1000);  

    // Setzt das Display nach 1000 ms zurück
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Keypad Test");
  }
}
