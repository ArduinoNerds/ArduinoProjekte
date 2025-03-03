// 2025 by ArduinoNerds
#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define NUM_LEDS 16
#define BUZZER_PIN 10

// Pins für die NeoPixel-Ringe
#define RED_RING 6
#define GREEN_RING 7
#define BLUE_RING 8
#define YELLOW_RING 9

// Taster-Pins
#define BUTTON_RED 2
#define BUTTON_GREEN 3
#define BUTTON_BLUE 4
#define BUTTON_YELLOW 5
#define START_BUTTON 11 

// Farben für das Simon-Spiel
const uint32_t COLORS[] = {
  0xFF0000, // Rot
  0x00FF00, // Grün
  0x0000FF, // Blau
  0xFFFF00  // Gelb
};

// Töne für jede Farbe
const int TONES[] = {262, 330, 392, 523};

// Objekte für die NeoPixel-Ringe
Adafruit_NeoPixel redRing(NUM_LEDS, RED_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel greenRing(NUM_LEDS, GREEN_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel blueRing(NUM_LEDS, BLUE_RING, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel yellowRing(NUM_LEDS, YELLOW_RING, NEO_GRB + NEO_KHZ800);

// LCD-Display zur Anzeige von Score und Highscore
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Spielvariablen
int sequence[100]; // Speichert die zufällige Reihenfolge
int playerInput[100]; // Speichert die Eingaben des Spielers
int sequenceLength = 0; // Aktuelle Länge der Sequenz
int inputIndex = 0; // Position der aktuellen Eingabe
int score = 0; // Aktueller Punktestand
int highscore = 0; // Höchster erreichter Score
int delayTime = 800; // Startgeschwindigkeit der Sequenz

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Druecke Start"); // Aufforderung zum Start
  lcd.setCursor(0, 1);
  lcd.print("Highscore: "); // Highscore-Anzeige
  lcd.setCursor(10, 1);
  lcd.print(highscore);
  
  // Taster-Pins als Eingänge mit Pullup-Widerstand
  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(START_BUTTON, INPUT_PULLUP); // Start-Taster
  pinMode(BUZZER_PIN, OUTPUT);
  
  // Initialisieren der NeoPixel-Ringe
  redRing.begin();
  greenRing.begin();
  blueRing.begin();
  yellowRing.begin();
  
  // Auf Start-Taster warten
  waitForStart();
}

// Funktion wartet auf das Drücken des Start-Tasters
void waitForStart() {
  while (digitalRead(START_BUTTON) == HIGH);
  startNewGame();
}

// Funktion zum Starten eines neuen Spiels
void startNewGame() {
  sequenceLength = 0;
  score = 0;
  delayTime = 800; // Geschwindigkeit zurücksetzen
  lcd.setCursor(0, 0);
  lcd.print("Score: 0        "); // Score zurücksetzen
  addToSequence();
  playSequence();
}

// Fügt eine neue zufällige Farbe zur Sequenz hinzu
void addToSequence() {
  sequence[sequenceLength] = random(0, 4);
  sequenceLength++;
  if (delayTime > 300) delayTime -= 50; // Erhöht die Geschwindigkeit mit jedem Level
}

// Gibt die gespeicherte Sequenz aus
void playSequence() {
  delay(1000);
  for (int i = 0; i < sequenceLength; i++) {
    lightUp(sequence[i]);
    delay(delayTime);
    turnOffAll();
    delay(200);
  }
  inputIndex = 0;
}

void loop() {
  // Überprüfung der Taster-Eingaben
  if (digitalRead(BUTTON_RED) == LOW) checkInput(0);
  if (digitalRead(BUTTON_GREEN) == LOW) checkInput(1);
  if (digitalRead(BUTTON_BLUE) == LOW) checkInput(2);
  if (digitalRead(BUTTON_YELLOW) == LOW) checkInput(3);
}

// Überprüft die Eingabe des Spielers
void checkInput(int color) {
  lightUp(color);
  delay(300);
  turnOffAll();
  
  if (sequence[inputIndex] == color) {
    tone(BUZZER_PIN, TONES[color], 300);
    playerInput[inputIndex] = color;
    inputIndex++;
    
    if (inputIndex == sequenceLength) {
      score++;
      if (score > highscore) {
        highscore = score;
        lcd.setCursor(10, 1);
        lcd.print(highscore); // Highscore aktualisieren
      }
      lcd.setCursor(0, 0);
      lcd.print("Score: ");
      lcd.setCursor(7, 0);
      lcd.print(score);
      delay(1000);
      addToSequence();
      playSequence();
    }
  } else {
    wrongInput(sequence[inputIndex]);
    waitForStart();
  }
}

// Lässt den entsprechenden NeoPixel-Ring aufleuchten
void lightUp(int color) {
  switch (color) {
    case 0: redRing.fill(COLORS[color], 0, NUM_LEDS); redRing.show(); break;
    case 1: greenRing.fill(COLORS[color], 0, NUM_LEDS); greenRing.show(); break;
    case 2: blueRing.fill(COLORS[color], 0, NUM_LEDS); blueRing.show(); break;
    case 3: yellowRing.fill(COLORS[color], 0, NUM_LEDS); yellowRing.show(); break;
  }
  tone(BUZZER_PIN, TONES[color], 300);
}

// Schaltet alle LEDs aus
void turnOffAll() {
  redRing.clear();
  greenRing.clear();
  blueRing.clear();
  yellowRing.clear();
  redRing.show();
  greenRing.show();
  blueRing.show();
  yellowRing.show();
  noTone(BUZZER_PIN);
}

// Blinkt die richtige LED als Fehlermeldung
void wrongInput(int correctColor) {
  for (int i = 0; i < 3; i++) {
    lightUp(correctColor);
    delay(200);
    turnOffAll();
    delay(200);
  }
  tone(BUZZER_PIN, 100, 1000);
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Game Over!");
  delay(2000);
  lcd.clear();
  lcd.setCursor(0, 1);
  lcd.print("Highscore: ");
  lcd.setCursor(10, 1);
  lcd.print(highscore);
}
