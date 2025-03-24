#include <TM1637Display.h>

// === Pinbelegung ===
#define MIC_PIN A0       // Mikrofoneingang (KY-038 Mikrofonmodul)
#define LED1 2           // LED 1 (für Code mit 2 Klopfern)
#define LED2 3           // LED 2 (für Code mit 3 Klopfern)
#define LED3 4           // LED 3 (für Code mit 4 Klopfern)
#define LED_RED 5        // Rote LED für Fehleranzeige
#define CLK 6            // CLK-Pin des TM1637-Displays
#define DIO 7            // DIO-Pin des TM1637-Displays

TM1637Display display(CLK, DIO);  // Display-Objekt erstellen

// === Einstellungen ===
const int threshold = 150;        // Schwelle für Klopferkennung (je nach Mikro anpassen)
const int knockTimeout = 2000;    // Zeitfenster zur Codeeingabe (in Millisekunden)

unsigned long lastKnockTime = 0;  // Zeit des letzten erkannten Klopfs
int knockCount = 0;               // Anzahl der erkannten Klopfer
bool knockDetected = false;       // Wird auf true gesetzt, wenn ein Klopf erkannt wurde

void setup() {
  Serial.begin(9600);             // Serielle Schnittstelle zur Debug-Ausgabe aktivieren

  // LEDs als Ausgänge definieren
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  display.setBrightness(5);       // Helligkeit des TM1637-Displays (0–7)
  display.showNumberDec(0);       // Startanzeige: 0 Klopfer
}

void loop() {
  int val = analogRead(MIC_PIN);  // Mikrofonwert einlesen

  // Klopf erkannt (über der Schwelle) und noch nicht als erkannt markiert
  if (val > threshold && !knockDetected) {
    knockCount++;                         // Klopfanzahl erhöhen
    lastKnockTime = millis();            // Zeit speichern
    knockDetected = true;                // Klopf als erkannt markieren
    display.showNumberDec(knockCount);   // Klopfanzahl auf Display anzeigen
    Serial.print("Klopf erkannt! Count: ");
    Serial.println(knockCount);
    delay(200);                          // Entprellungspause
  }

  // Mikro wieder unter Schwelle – bereit für neuen Klopf
  if (val < threshold - 50) {
    knockDetected = false;
  }

  // Wenn innerhalb des Zeitfensters keine weiteren Klopfer folgen
  if (knockCount > 0 && millis() - lastKnockTime > knockTimeout) {
    auswerten(knockCount);        // Anzahl auswerten
    knockCount = 0;               // Zähler zurücksetzen
    delay(1500);                  // Anzeige kurz anzeigen lassen
    display.showNumberDec(0);     // Reset auf 0
  }
}

// === Auswertung des Klopf-Codes ===
void auswerten(int count) {
  // Alle LEDs ausschalten
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  digitalWrite(LED_RED, LOW);

  switch (count) {
    case 2:
      digitalWrite(LED1, HIGH);     // LED 1 einschalten
      anzeigeText("L 1");           // Anzeige: L 1
      break;
    case 3:
      digitalWrite(LED2, HIGH);     // LED 2 einschalten
      anzeigeText("L 2");           // Anzeige: L 2
      break;
    case 4:
      digitalWrite(LED3, HIGH);     // LED 3 einschalten
      anzeigeText("L 3");           // Anzeige: L 3
      break;
    case 5:
      // Mastercode: alle drei LEDs gleichzeitig einschalten
      digitalWrite(LED1, HIGH);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, HIGH);
      anzeigeText("ALL");           // Anzeige: ALL
      break;
    default:
      // Ungültiger oder unbekannter Code: rote LED blinkt 3x
      for (int i = 0; i < 3; i++) {
        digitalWrite(LED_RED, HIGH);
        delay(200);
        digitalWrite(LED_RED, LOW);
        delay(200);
      }
      anzeigeText("Err");           // Anzeige: Err
      return;                       // Keine weiteren Aktionen
  }

  delay(2000); // LEDs 2 Sekunden eingeschaltet lassen

  // Danach alle LEDs ausschalten (gilt für erfolgreiche Codes)
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
}

// === Funktion zur Umwandlung von Zeichen in Segment-Codes ===
uint8_t charToSegments(char c) {
  switch (toupper(c)) {
    case 'A': return 0x77;
    case 'L': return 0x38;
    case 'E': return 0x79;
    case 'R': return 0x50;
    case '0': return 0x3F;
    case '1': return 0x06;
    case '2': return 0x5B;
    case '3': return 0x4F;
    case '4': return 0x66;
    case '5': return 0x6D;
    case '6': return 0x7D;
    case '7': return 0x07;
    case '8': return 0x7F;
    case '9': return 0x6F;
    case ' ': return 0x00;          // Leerzeichen
    default:  return 0x00;          // Unbekannte Zeichen bleiben aus
  }
}

// === Funktion zum Anzeigen von Text auf dem TM1637-Display ===
// Unterstützt nur maximal 4 Zeichen und nur darstellbare Buchstaben/Zahlen
void anzeigeText(const char* txt) {
  uint8_t seg[4] = {0};             // Puffer für Segmentdaten
  for (int i = 0; i < 4 && txt[i] != '\0'; i++) {
    seg[i] = charToSegments(txt[i]);
  }
  display.setSegments(seg);        // Zeichen auf das Display schreiben
}
