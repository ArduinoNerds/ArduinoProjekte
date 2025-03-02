#include <Servo.h>
#include <TM1637Display.h>

// TM1637 7-Segment-Display Pins
#define CLK 6  // TM1637 Clock-Pin
#define DIO 7  // TM1637 Data-Pin

TM1637Display display(CLK, DIO);
Servo meinServo;

// Servo-Pin
#define SERVO_PIN 9  

// Taster-Pins einzeln definieren
#define TASTER_0   2  // Taster für 0°
#define TASTER_45  3  // Taster für 45°
#define TASTER_90  4  // Taster für 90°
#define TASTER_135 5  // Taster für 135°
#define TASTER_180 8  // Taster für 180°
#define TASTER_PLUS 10  // Taster für +1°
#define TASTER_MINUS 11 // Taster für -1°

int servoWinkel = 90; // Startposition des Servos (Mitte)


void setup() {
  // Servo initialisieren
  meinServo.attach(SERVO_PIN);

  // Display-Helligkeit setzen (0-7, 7 = Maximum)
  display.setBrightness(7);

  // Taster als Eingänge mit internem Pullup-Widerstand
  pinMode(TASTER_0, INPUT_PULLUP);
  pinMode(TASTER_45, INPUT_PULLUP);
  pinMode(TASTER_90, INPUT_PULLUP);
  pinMode(TASTER_135, INPUT_PULLUP);
  pinMode(TASTER_180, INPUT_PULLUP);
  pinMode(TASTER_PLUS, INPUT_PULLUP);
  pinMode(TASTER_MINUS, INPUT_PULLUP);

  // Startposition setzen
  meinServo.write(servoWinkel);
  display.showNumberDec(servoWinkel, true);
}

void loop() {
  // Wenn ein Taster gedrückt wird, neuen Winkel setzen
  if (digitalRead(TASTER_0) == LOW) {
    servoWinkel = 0;   // Servo auf 0°
  }
  if (digitalRead(TASTER_45) == LOW) {
    servoWinkel = 45;  // Servo auf 45°
  }
  if (digitalRead(TASTER_90) == LOW) {
    servoWinkel = 90;  // Servo auf 90°
  }
  if (digitalRead(TASTER_135) == LOW) {
    servoWinkel = 135; // Servo auf 135°
  }
  if (digitalRead(TASTER_180) == LOW) {
    servoWinkel = 180; // Servo auf 180°
  }

  // Feineinstellung: Servo 1° nach vorne oder hinten bewegen
  if (digitalRead(TASTER_PLUS) == LOW && servoWinkel < 180) {
    servoWinkel++;  // 1° vor
  }
  if (digitalRead(TASTER_MINUS) == LOW && servoWinkel > 0) {
    servoWinkel--;  // 1° zurück
  }

  // Servo auf den neuen Winkel setzen
  meinServo.write(servoWinkel);

  // Winkel auf 7-Segment-Display anzeigen
  display.showNumberDec(servoWinkel, true);

  // Kurze Verzögerung für stabile Taster-Erkennung
  delay(100);
}
