// Pins für die Segmente A-G + DP
const int segA = 2;
const int segB = 3;
const int segC = 4;
const int segD = 5;
const int segE = 6;
const int segF = 7;
const int segG = 8;
const int segDP = 9;
const int buttonPin = 10;   // Taster für nächstes Segment
const int resetButtonPin = 11;  // Reset-Taster an Pin 11

// Array mit allen Segment-Pins in Reihenfolge
const int segments[] = {segA, segB, segC, segD, segE, segF, segG, segDP};

// Namen für die Segmente (zur Erklärung)
const char* segmentNames[] = {"A (oben)", "B (oben rechts)", "C (unten rechts)", 
                              "D (unten)", "E (unten links)", "F (oben links)", 
                              "G (Mitte)", "DP (Punkt)"};

// Speichert den aktuellen Status der Segmente
int currentSegment = 0;  // Welches Segment gerade aktiv ist
bool segmentState[8] = {0};  // Zustand der Segmente (0 = aus, 1 = an)

// Taster-Zustände speichern
bool lastButtonState = HIGH;
bool lastResetButtonState = HIGH;

void setup() {
  Serial.begin(9600); // Serieller Monitor für Erklärungen

  // Alle Segmente als Output setzen
  for (int i = 0; i < 8; i++) {
    pinMode(segments[i], OUTPUT);
    digitalWrite(segments[i], HIGH); // Start: Alles aus (HIGH für Common Anode)
  }

  pinMode(buttonPin, INPUT_PULLUP); // Taster mit internem Pullup-Widerstand
  pinMode(resetButtonPin, INPUT_PULLUP); // Reset-Taster mit Pullup
}

void loop() {
  bool buttonState = digitalRead(buttonPin); 
  bool resetButtonState = digitalRead(resetButtonPin);

  // Prüfen, ob der normale Taster gedrückt wurde
  if (buttonState == LOW && lastButtonState == HIGH) {
    Serial.print("Schalte Segment um: ");
    Serial.print(segmentNames[currentSegment]); 
    Serial.print(" - ");
    Serial.println(segmentState[currentSegment] ? "AUS" : "AN");

    // Umschalten des aktuellen Segments (LOW = AN, HIGH = AUS für Common Anode)
    segmentState[currentSegment] = !segmentState[currentSegment]; // Zustand umkehren
    digitalWrite(segments[currentSegment], segmentState[currentSegment] ? LOW : HIGH);

    currentSegment++; // Nächstes Segment vorbereiten
    if (currentSegment >= 8) {
      currentSegment = 0; // Zurück zum ersten Segment, wenn alle durch sind
    }

    delay(300); // Kurze Pause für Entprellung
  }

  // Prüfen, ob der Reset-Taster gedrückt wurde
  if (resetButtonState == LOW && lastResetButtonState == HIGH) {
    Serial.println("Reset gedrückt! Alle Segmente aus!");

    // Alle Segmente ausschalten
    for (int i = 0; i < 8; i++) {
      digitalWrite(segments[i], HIGH); // Segmente aus (Common Anode: HIGH = AUS)
      segmentState[i] = 0; // Speichert den neuen Zustand
    }

    currentSegment = 0; // Zurück zum ersten Segment
    delay(300); // Kurze Pause für Entprellung
  }

  lastButtonState = buttonState; // Speichert den Button-Status
  lastResetButtonState = resetButtonState; // Speichert den Reset-Button-Status
}
