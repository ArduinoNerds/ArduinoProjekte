#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

#define IR_RECEIVER_PIN 2  // IR-Empfänger an Pin 2
#define LED_PIN 4          // LED an Pin 4

LiquidCrystal_I2C lcd(0x27, 16, 2);  // I2C LCD mit Adresse 0x27

uint32_t gespeicherterCode = 0;  // Hier speichern wir den letzten Code
bool ledStatus = false;  // Status der LED (AN oder AUS)

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVER_PIN, DISABLE_LED_FEEDBACK);
    lcd.init();
    lcd.backlight();
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    lcd.setCursor(0, 0);
    lcd.print("Warte auf Code...");
}

void loop() {
    if (IrReceiver.decode()) {
        uint32_t code = IrReceiver.decodedIRData.decodedRawData;
        
        // Zeige den Code auf dem LCD an
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Empf. Code:");
        lcd.setCursor(0, 1);
        lcd.print("0x");
        lcd.print(code, HEX);

        Serial.print("Empfangener Code: 0x");
        Serial.println(code, HEX);

        // Falls der Code bereits gespeichert ist, LED umschalten
        if (code == gespeicherterCode) {
            ledStatus = !ledStatus;  // LED-Zustand umkehren
            digitalWrite(LED_PIN, ledStatus ? HIGH : LOW);
            Serial.println(ledStatus ? "LED AN" : "LED AUS");
        } else {
            gespeicherterCode = code;  // Neuen Code speichern
            Serial.println("Neuer Code gespeichert!");
        }

        IrReceiver.resume();  // Bereit für das nächste Signal
        delay(500);  // Kleiner Delay, um Mehrfachauslösungen zu vermeiden
    }
}
