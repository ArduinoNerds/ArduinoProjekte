#include <Servo.h>            // Servo-Bibliothek einbinden
#include <TM1637Display.h>    // Bibliothek für TM1637 7-Segment-Display einbinden

// Pins für das TM1637 7-Segment-Display
#define CLK 6  // Takt-Pin (Clock)
#define DIO 7  // Daten-Pin (Data)

TM1637Display display(CLK, DIO); // Display-Objekt erstellen
Servo meinServo;                 // Servo-Objekt erstellen

//  Pins für das Potentiometer und Servo
int potiPin = A0;  // Potentiometer an Analog-Pin A0
int servoPin = 9;  // Servo an Digital-Pin 9

void setup() {
  meinServo.attach(servoPin);  // Servo mit dem definierten Pin verbinden
  display.setBrightness(7);    // Helligkeit des Displays auf Maximum (0-7)
}

void loop() {
  int potiWert = analogRead(potiPin); //  Poti-Wert von 0 bis 1023 auslesen
  int winkel = map(potiWert, 0, 1023, 0, 180); //  Wert auf 0-180 Grad umrechnen

  meinServo.write(winkel); //  Servo auf berechneten Winkel drehen
  display.showNumberDec(winkel, false); //  Winkel auf dem Display anzeigen

  delay(100); //  100ms Pause für eine flüssige Aktualisierung
}
