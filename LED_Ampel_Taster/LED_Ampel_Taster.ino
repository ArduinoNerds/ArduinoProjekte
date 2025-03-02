// Definiere die Pins für die LEDs der Autoampel
const int ledRot = 13;        
const int ledGelb = 12;       
const int ledGruen = 11;      

// Definiere die Pins für die LEDs der Fußgängerampel
const int ledFussgaengerRot = 10;  
const int ledFussgaengerGruen = 9; 

// Definiere den Pin für den Taster
const int taster = 2;        

// Variable für den Tasterstatus
bool anforderung = false;    

void setup() {
  pinMode(ledRot, OUTPUT);
  pinMode(ledGelb, OUTPUT);
  pinMode(ledGruen, OUTPUT);
  pinMode(ledFussgaengerRot, OUTPUT);
  pinMode(ledFussgaengerGruen, OUTPUT);

  pinMode(taster, INPUT_PULLUP); // Interner Pull-Up aktiviert

  // Anfangszustand der Ampel
  digitalWrite(ledRot, LOW);  
  digitalWrite(ledGelb, LOW); 
  digitalWrite(ledGruen, HIGH);
  digitalWrite(ledFussgaengerRot, HIGH); 
  digitalWrite(ledFussgaengerGruen, LOW);
}

void loop() {
  // Prüfen, ob der Taster gedrückt wurde
  if (digitalRead(taster) == LOW) {  
    anforderung = true;  
  }

  if (anforderung) {
    // Autos bekommen Gelb
    delay(1000);
    digitalWrite(ledGruen, LOW);
    digitalWrite(ledGelb, HIGH);
    delay(2000);

    // Autos bekommen Rot
    digitalWrite(ledGelb, LOW);
    digitalWrite(ledRot, HIGH);
    delay(2000);

    // Fußgänger haben Grün
    digitalWrite(ledFussgaengerRot, LOW);
    digitalWrite(ledFussgaengerGruen, HIGH);
    delay(5000);

    // Blinksignal für Fußgänger
    for (int i = 0; i < 3; i++) {
      digitalWrite(ledFussgaengerGruen, LOW);
      delay(500);
      digitalWrite(ledFussgaengerGruen, HIGH);
      delay(500);
    }

    // Fußgänger wieder Rot
    digitalWrite(ledFussgaengerGruen, LOW);
    digitalWrite(ledFussgaengerRot, HIGH);
    delay(2000);

    // Autos bekommen wieder Grün
    digitalWrite(ledGelb, HIGH);
    delay(2000);
    digitalWrite(ledRot, LOW);
    digitalWrite(ledGelb,LOW);
    digitalWrite(ledGruen, HIGH);

    // Anforderung zurücksetzen
    anforderung = false;
  }
}
