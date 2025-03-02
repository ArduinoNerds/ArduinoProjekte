#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

// LCD2004 Display mit I2C-Adresse 0x27 (manchmal auch 0x3F)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// DHT22 Sensor an Pin 2 für Temperatur und Luftfeuchtigkeit
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LDR Sensor an A0
#define LDR_PIN A0

// LED für hohe Luftfeuchtigkeit an Pin 4
#define LED_PIN 4

void setup() {
    // LCD initialisieren
    lcd.init();
    lcd.backlight(); // Hintergrundbeleuchtung einschalten
    lcd.setCursor(0, 0);
    lcd.print("DHT22 Sensor Start"); // Startmeldung
    delay(2000);
    lcd.clear(); // Display leeren
    
    // DHT22 Sensor starten
    dht.begin();

    // LED-Pin als Ausgang setzen
    pinMode(LED_PIN, OUTPUT);
}

void loop() {
    // Temperatur in °C auslesen
    float temperature = dht.readTemperature();
    // Luftfeuchtigkeit in % auslesen
    float humidity = dht.readHumidity();
    // Helligkeit aus dem LDR-Sensor lesen (0 - 1023) und invertiert in Prozent umwandeln
    int ldrValue = analogRead(LDR_PIN);
    int brightnessPercent = map(ldrValue, 0, 1023, 100, 0); // Werte umdrehen
    
    // Temperatur auf dem Display anzeigen
    lcd.setCursor(0, 0);
    lcd.print("Temperatur: ");
    lcd.print(temperature);
    lcd.print(" C   ");
    
    // Luftfeuchtigkeit auf dem Display anzeigen
    lcd.setCursor(0, 1);
    lcd.print("Luftfeucht.: ");
    lcd.print(humidity);
    lcd.print(" %   ");
    
    // Helligkeit in Prozent (invertiert) in Zeile 3 anzeigen
    lcd.setCursor(0, 2);
    lcd.print("Helligkeit: ");
    lcd.print(brightnessPercent);
    lcd.print(" %   ");
    
    // LED blinken lassen, wenn Luftfeuchtigkeit über 60% liegt
    if (humidity > 60) {
        digitalWrite(LED_PIN, HIGH);
        delay(200);
        digitalWrite(LED_PIN, LOW);
        delay(200);
    } else {
        digitalWrite(LED_PIN, LOW); // LED aus, wenn unter 60%
    }

    delay(2000); // Werte alle 2 Sekunden aktualisieren
}
