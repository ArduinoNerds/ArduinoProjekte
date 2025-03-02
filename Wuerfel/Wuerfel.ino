vconst int buttonPin = 2; // Taster an Pin 2
const int statusLed = 9; // Status-LED zeigt Bereitschaft an
const int leds[] = {3, 4, 5, 6, 7, 8}; // LED-Pins für 1-6

void setup() {
  pinMode(buttonPin, INPUT_PULLUP); // Taster mit internem Pull-Up-Widerstand
  pinMode(statusLed, OUTPUT); // Status-LED
  for (int i = 0; i < 6; i++) {
    pinMode(leds[i], OUTPUT);
  }

  digitalWrite(statusLed, HIGH); // Würfel ist bereit
}

void loop() {
  if (digitalRead(buttonPin) == LOW) { // Wenn Taster gedrückt
  delay(200); // Kurze Pause um mehrfach Auslösung zu verhindern
    digitalWrite(statusLed, LOW); // Bereitschaft aus, da Würfeln beginnt
    lauflicht(); // Lauflicht startet
    int wurfelZahl = random(1, 7); // Zufallszahl zwischen 1 und 6 generieren
    zeigeWurf(wurfelZahl); // Zeige das Ergebnis
    delay(5000); // 5 Sekunden Pause nach dem Wurf
    blinken(wurfelZahl); // Gewürfelte Zahl blinkt 3x
    digitalWrite(statusLed, HIGH); // Würfel ist wieder bereit
  }
}

void lauflicht() {
  for (int j = 0; j < 10; j++) { // 10 Durchläufe für Lauflicht
    for (int i = 0; i < 6; i++) {
      digitalWrite(leds[i], HIGH);
      delay(50); // Schnelles Lauflicht
      digitalWrite(leds[i], LOW);
    }
  }
}

void zeigeWurf(int zahl) {
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[i], LOW); // Erst alle LEDs ausschalten
  }
  
  // Die entsprechende Anzahl LEDs anschalten
  for (int i = 0; i < zahl; i++) {
    digitalWrite(leds[i], HIGH);
  }
}

void blinken(int zahl) {
  for (int k = 0; k < 3; k++) { // 3x blinken
    for (int i = 0; i < 6; i++) {
      digitalWrite(leds[i], LOW); // Alle LEDs aus
    }
    delay(500);
    for (int i = 0; i < zahl; i++) {
      digitalWrite(leds[i], HIGH); // Erneut die gewürfelte Zahl leuchten lassen
    }
    delay(500);
  }

  // Nach dem Blinken LEDs ausschalten
  for (int i = 0; i < 6; i++) {
    digitalWrite(leds[i], LOW);
  }
}
