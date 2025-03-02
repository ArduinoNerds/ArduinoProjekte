#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

// LCD 2004 mit I2C-Adresse 0x27 (20 Zeichen x 4 Zeilen)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Definition des Keypads (4x4 Matrix)
const byte ROWS = 4;  
const byte COLS = 4;  

// Tastenbelegung des Keypads
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Definition der Zeilen- und Spalten-Pins für das Keypad
const byte row1 = 2, row2 = 3, row3 = 4, row4 = 5;  // Zeilen
const byte col1 = 6, col2 = 7, col3 = 8, col4 = 9;  // Spalten
byte rowPins[ROWS] = {row1, row2, row3, row4};  
byte colPins[COLS] = {col1, col2, col3, col4};  

// Erstellen des Keypad-Objekts
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// LED-Pins zur Statusanzeige
const byte greenLED = 10; // Grüne LED = PIN korrekt
const byte redLED = 11;   // Rote LED = PIN falsch

// Servo für das Türschloss
Servo safeServo;
const byte servoPin = 12;  // Servo-Pin

// Variablen zur Speicherung der PIN
String savedPIN = "";  // Gespeicherte PIN
String enteredPIN = "";  // Aktuell eingegebene PIN
bool isNewPIN = true;  // Zeigt an, ob eine neue PIN gesetzt werden muss

void setup() {
  // Initialisierung des LCDs
  lcd.init();
  lcd.backlight(); // Hintergrundbeleuchtung einschalten
  
  // LED-Pins als Ausgänge definieren
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  
  // LEDs beim Start ausschalten
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);

  // Servo anschließen und auf "geschlossen" setzen (0°)
  safeServo.attach(servoPin);
  safeServo.write(0);

  // Aufforderung zur Eingabe einer neuen PIN beim ersten Start
  lcd.setCursor(0, 0);
  lcd.print("Neue PIN setzen:");
}

void loop() {
  char key = keypad.getKey(); // Taste vom Keypad auslesen

  if (key) {
    if (isNewPIN) {
      setNewPIN(key); // Falls eine neue PIN gesetzt werden soll
    } else {
      if (key == 'A') { // Falls "A" gedrückt wird, kann die PIN geändert werden
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Neue PIN eingeben:");
        savedPIN = "";
        isNewPIN = true;
      } 
      else if (key == '#') { // Falls "#" gedrückt wird, PIN überprüfen
        checkPIN();
      } 
      else if (key == '*') { // Falls "*" gedrückt wird, Eingabe zurücksetzen
        enteredPIN = "";
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("PIN eingeben:");
      } 
      else { // Falls eine normale Ziffer gedrückt wird
        if (enteredPIN.length() < 4) {  
          enteredPIN += key;  
          lcd.setCursor(enteredPIN.length() - 1, 1);
          lcd.print('*');  // Eingabe als "*" anzeigen
        }
      }
    }
  }
}

// Funktion zur Eingabe und Speicherung einer neuen PIN
void setNewPIN(char key) {
  if (key == '#') { // Wenn "#" gedrückt wird, PIN speichern
    if (savedPIN.length() == 4) { // Nur speichern, wenn genau 4 Zeichen eingegeben wurden
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PIN gespeichert!");
      delay(2000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("PIN eingeben:");
      isNewPIN = false; // PIN wurde gespeichert
    }
  } 
  else if (key == '*') { // Falls "*" gedrückt wird, Eingabe zurücksetzen
    savedPIN = "";
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Neue PIN setzen:");
  } 
  else { // Falls eine normale Ziffer gedrückt wird
    if (savedPIN.length() < 4) {  
      savedPIN += key;  
      lcd.setCursor(savedPIN.length() - 1, 1);
      lcd.print('*');  // Zeigt die Eingabe als "*"
    }
  }
}

// Funktion zur PIN-Überprüfung
void checkPIN() {
  lcd.clear();
  lcd.setCursor(0, 0);

  if (enteredPIN == savedPIN) { // Wenn die eingegebene PIN korrekt ist
    lcd.print("Safe geoeffnet!");
    digitalWrite(greenLED, HIGH); // Grüne LED an
    safeServo.write(90); // Servo auf 90° drehen (Safe öffnen)
    delay(4000); // Safe für 4 Sekunden offen lassen
    safeServo.write(0); // Safe schließen
    digitalWrite(greenLED, LOW); // Grüne LED aus
  } 
  else { // Falls die PIN falsch ist
    lcd.print("Falsche PIN!");
    digitalWrite(redLED, HIGH); // Rote LED an
    delay(3000); // 3 Sekunden Wartezeit
    digitalWrite(redLED, LOW); // Rote LED aus
  }

  // PIN-Eingabe zurücksetzen
  enteredPIN = "";
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("PIN eingeben:");
}
