#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

// LCD2004 für Innenwerte (I2C-Adresse 0x27)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Dot-Matrix MAX7219 für Außenwerte (SPI-Pins: DIN=11, CLK=13, CS=10)
#define MAX_DEVICES 4   // Anzahl der Dot-Matrix-Module
#define DATA_PIN 11     // DIN (Data In)
#define CLK_PIN 13      // CLK (Clock)
#define CS_PIN 10       // Chip-Select
MD_Parola matrix = MD_Parola(MD_MAX72XX::PAROLA_HW, CS_PIN, MAX_DEVICES);


// DHT22 Sensor für Innen (Pin 2)
#define DHTPIN_INNEN 2
DHT dht_innen(DHTPIN_INNEN, DHT22);

// DHT22 Sensor für Außen (Pin 3)
#define DHTPIN_AUSSEN 3
DHT dht_aussen(DHTPIN_AUSSEN, DHT22);

// LDR Sensoren
#define LDR_INNEN A0
#define LDR_AUSSEN A1

// LED-Pins
#define LED_PIN_INNEN 4
#define LED_PIN_AUSSEN 5

// Timer-Variablen
unsigned long lastBlinkTime = 0;
bool ledStateInnen = false;
bool ledStateAussen = false;

void setup() {
    // LCD initialisieren
    lcd.init();
    lcd.backlight();
    
    // Dot-Matrix initialisieren
    matrix.begin();
    matrix.setIntensity(5);  // Helligkeit 0-15 (5 ist Mittelwert)
    matrix.displayClear();
    
    // DHT22 Sensoren starten
    dht_innen.begin();
    dht_aussen.begin();

    // LEDs als Ausgang setzen
    pinMode(LED_PIN_INNEN, OUTPUT);
    pinMode(LED_PIN_AUSSEN, OUTPUT);
}

void loop() {
    // Sensorwerte für Innen auslesen
    float temp_innen = dht_innen.readTemperature();
    float hum_innen = dht_innen.readHumidity();
    int ldr_innen = map(analogRead(LDR_INNEN), 0, 1023, 100, 0);

    // Sensorwerte für Außen auslesen
    float temp_aussen = dht_aussen.readTemperature();
    float hum_aussen = dht_aussen.readHumidity();
    int ldr_aussen = map(analogRead(LDR_AUSSEN), 0, 1023, 100, 0);

    // LCD-Anzeige für Innen
    lcd.setCursor(0, 0);
    lcd.print("Temp Innen: ");
    lcd.print(temp_innen);
    lcd.print(" C   ");

    lcd.setCursor(0, 1);
    lcd.print("Luftf.Innen: ");
    lcd.print(hum_innen);
    lcd.print(" %   ");

    lcd.setCursor(0, 2);
    lcd.print("Licht Innen: ");
    lcd.print(ldr_innen);
    lcd.print(" %   ");

    // Dot-Matrix Laufschrift für Außen
    String message = "Temp: " + String(temp_aussen) + "C | ";
    message += "Luft: " + String(hum_aussen) + "% | ";
    message += "Licht: " + String(ldr_aussen) + "%";

    // Start der Laufschrift
    matrix.displayText(message.c_str(), PA_CENTER, 100, 2000, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

    // **Wichtige Änderung**: Nutze eine **Schleife mit Timer**, damit die LEDs während der Laufschrift blinken
    unsigned long startTime = millis();
    while (!matrix.displayAnimate()) {  // Animation läuft im Hintergrund
        if (millis() - lastBlinkTime >= 500) { // LED blinkt alle 500ms
            lastBlinkTime = millis();
            ledStateInnen = !ledStateInnen;  // Zustand umschalten
            ledStateAussen = !ledStateAussen;
            digitalWrite(LED_PIN_INNEN, ledStateInnen);
            digitalWrite(LED_PIN_AUSSEN, ledStateAussen);
        }
    }

    delay(2000); // Werte alle 2 Sekunden aktualisieren
}
