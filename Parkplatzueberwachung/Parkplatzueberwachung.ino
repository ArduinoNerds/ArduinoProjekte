v// **Ultraschallsensoren (HC-SR04)**
#define TRIG_1 22
#define ECHO_1 23
#define TRIG_2 24
#define ECHO_2 25
#define TRIG_3 26
#define ECHO_3 27
#define TRIG_4 28
#define ECHO_4 29
#define TRIG_5 30
#define ECHO_5 31

// **RGB-LED Pins für 5 Parkplätze**
#define LED1_R 42
#define LED1_G 43
#define LED2_R 44
#define LED2_G 45
#define LED3_R 46
#define LED3_G 47
#define LED4_R 48
#define LED4_G 49
#define LED5_R 50
#define LED5_G 51

// **Status-LED für Gesamtanzeige**
#define STATUS_LED_R 52  // Rot: Alle Parkplätze belegt
#define STATUS_LED_G 53  // Grün: Noch Parkplätze frei

// **Schwellenwert für belegten Parkplatz (in cm)**
#define PARKING_DISTANCE 30  

void setup() {
    // **Sensoren als Ein-/Ausgänge setzen**
    pinMode(TRIG_1, OUTPUT); pinMode(ECHO_1, INPUT);
    pinMode(TRIG_2, OUTPUT); pinMode(ECHO_2, INPUT);
    pinMode(TRIG_3, OUTPUT); pinMode(ECHO_3, INPUT);
    pinMode(TRIG_4, OUTPUT); pinMode(ECHO_4, INPUT);
    pinMode(TRIG_5, OUTPUT); pinMode(ECHO_5, INPUT);

    // **RGB-LEDs für Parkplätze als Ausgang setzen**
    pinMode(LED1_R, OUTPUT); pinMode(LED1_G, OUTPUT);
    pinMode(LED2_R, OUTPUT); pinMode(LED2_G, OUTPUT);
    pinMode(LED3_R, OUTPUT); pinMode(LED3_G, OUTPUT);
    pinMode(LED4_R, OUTPUT); pinMode(LED4_G, OUTPUT);
    pinMode(LED5_R, OUTPUT); pinMode(LED5_G, OUTPUT);

    // **Status-LED als Ausgang setzen**
    pinMode(STATUS_LED_R, OUTPUT);
    pinMode(STATUS_LED_G, OUTPUT);
}

void loop() {
    int occupiedSpots = 0; // Zählt belegte Parkplätze

    // **Überprüfe belegte Parkplätze und steuere LEDs**
    occupiedSpots += checkParkingSpot(TRIG_1, ECHO_1, LED1_R, LED1_G);
    occupiedSpots += checkParkingSpot(TRIG_2, ECHO_2, LED2_R, LED2_G);
    occupiedSpots += checkParkingSpot(TRIG_3, ECHO_3, LED3_R, LED3_G);
    occupiedSpots += checkParkingSpot(TRIG_4, ECHO_4, LED4_R, LED4_G);
    occupiedSpots += checkParkingSpot(TRIG_5, ECHO_5, LED5_R, LED5_G);

    // **Gesamtstatus-LED steuern**
    if (occupiedSpots == 5) {
        digitalWrite(STATUS_LED_R, HIGH);  // Alle belegt -> Rot an
        digitalWrite(STATUS_LED_G, LOW);   // Grün aus
    } else {
        digitalWrite(STATUS_LED_R, LOW);   // Rot aus
        digitalWrite(STATUS_LED_G, HIGH);  // Noch frei -> Grün an
    }

    delay(500);  // Aktualisierung alle 500 ms
}

// **Funktion zur Distanzmessung & LED-Steuerung**
int checkParkingSpot(int trigPin, int echoPin, int ledR, int ledG) {
    float distance = measureDistance(trigPin, echoPin);
    
    if (distance < PARKING_DISTANCE) {
        digitalWrite(ledR, HIGH);  // Rot an
        digitalWrite(ledG, LOW);   // Grün aus
        return 1; // Parkplatz belegt
    } else {
        digitalWrite(ledR, LOW);   // Rot aus
        digitalWrite(ledG, HIGH);  // Grün an
        return 0; // Parkplatz frei
    }
}

// **Funktion zur Distanzmessung mit HC-SR04**
float measureDistance(int trigPin, int echoPin) {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    
    long duration = pulseIn(echoPin, HIGH, 50000);  // Timeout auf 50ms setzen
    if (duration == 0) return 999;  // Kein Echo = kein Objekt

    float distance = duration * 0.034 / 2;  // Entfernung berechnen
    return (distance > 200) ? 200 : distance;  // Maximale Entfernung auf 200 cm begrenzen
}
