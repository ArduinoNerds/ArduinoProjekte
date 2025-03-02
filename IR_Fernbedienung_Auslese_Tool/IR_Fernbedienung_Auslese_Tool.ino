#include <IRremote.h>           // IR-Empfänger Bibliothek
#include <Wire.h>               // I2C-Kommunikation für LCD
#include <LiquidCrystal_I2C.h>  // LCD2004 Bibliothek

#define PIN_RECEIVER 2  // IR-Empfänger an Pin D2

// LCD 20x4 I2C (Adresse 0x27)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// IR-Empfänger initialisieren
IRrecv receiver(PIN_RECEIVER);

void setup() {
    lcd.init();              
    lcd.backlight();         

    // Startnachricht auf LCD anzeigen
    lcd.setCursor(0, 0);
    lcd.print("IR-Auslese-Tool");
    lcd.setCursor(0, 1);
    lcd.print("Taste druecken!");

    receiver.enableIRIn();  
}

void loop() {
    // Prüfen, ob ein IR-Signal empfangen wurde
    if (receiver.decode()) {
        showIRCode();       
        receiver.resume();  
    }
}

// Funktion zeigt HEX und Command-Code an
void showIRCode() {
    unsigned long hexCode = receiver.decodedIRData.decodedRawData;
    int commandCode = receiver.decodedIRData.command;

    lcd.clear();

    // Erste Zeile: Info
    lcd.setCursor(0, 0);
    lcd.print("IR Code empfangen:");

    // Zweite Zeile: HEX-Code
    lcd.setCursor(0, 1);
    lcd.print("HEX: 0x");
    lcd.print(hexCode, HEX);

    // Dritte Zeile: Command-Code (dezimal)
    lcd.setCursor(0, 2);
    lcd.print("Command: ");
    lcd.print(commandCode);

    // Vierte Zeile: Hinweis
    lcd.setCursor(0, 3);
    lcd.print("Taste erneut druecken");
}
