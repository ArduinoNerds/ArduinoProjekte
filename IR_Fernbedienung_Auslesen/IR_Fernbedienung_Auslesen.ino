#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <IRremote.h>

#define IR_RECEIVER_PIN 2 // Pin für den IR-Empfänger

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C-Adresse 0x27 für das 16x2 LCD

void setup() {
    Serial.begin(9600);
    IrReceiver.begin(IR_RECEIVER_PIN, ENABLE_LED_FEEDBACK);
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("IR Remote Test");
    delay(2000);
    lcd.clear();
}

void loop() {
    if (IrReceiver.decode()) {
        lcd.clear();
        
        // Hersteller basierend auf dem Protokoll bestimmen
        lcd.setCursor(0, 0);
        switch (IrReceiver.decodedIRData.protocol) {
            case NEC: lcd.print("NEC"); break;
            case SONY: lcd.print("Sony"); break;
            case RC5: lcd.print("Philips RC5"); break;
            case RC6: lcd.print("Philips RC6"); break;
            case SAMSUNG: lcd.print("Samsung"); break;
            case LG: lcd.print("LG"); break;
            default: lcd.print("Unbekannt"); break;
        }
        
        // Hex-Code in der unteren Zeile anzeigen
        lcd.setCursor(0, 1);
        lcd.print("0x");
        lcd.print(IrReceiver.decodedIRData.decodedRawData, HEX);
        
        Serial.print("Empfangener Code: 0x");
        Serial.println(IrReceiver.decodedIRData.decodedRawData, HEX);
        
        IrReceiver.resume(); // Bereit für das nächste Signal
        delay(2000); // Zeit zur Anzeige
    }
}