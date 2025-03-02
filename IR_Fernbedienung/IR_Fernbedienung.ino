#include <IRremote.h>           // Bibliothek für den IR-Empfänger
#include <Wire.h>               // I2C-Kommunikation für das LCD
#include <LiquidCrystal_I2C.h>  // Bibliothek für das LCD2004 mit I2C

#define PIN_RECEIVER 2  // IR-Empfänger an Pin D2

// LCD 16x2 über I2C mit Adresse 0x27 (falls nötig, 0x3F testen)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// IR-Empfänger initialisieren
IRrecv receiver(PIN_RECEIVER);

void setup()
{
    lcd.init();             // LCD initialisieren
    lcd.backlight();        // Hintergrundbeleuchtung aktivieren

    // Startnachricht auf LCD anzeigen
    lcd.setCursor(0, 0);
    lcd.print("<Taste druecken>");
    
    receiver.enableIRIn();  // IR-Empfänger aktivieren
}

void loop()
{
    // Überprüfen, ob ein IR-Signal empfangen wurde
    if (receiver.decode()) {
        translateIR();       // IR-Code auswerten
        receiver.resume();   // Empfänger für das nächste Signal bereitmachen
    }
}

// Funktion zur Anzeige der erkannten Taste mit beiden Codes auf dem LCD
void lcdPrint(const char* text, unsigned long hexCode, int commandCode)
{
    lcd.clear();
    
    // Zeile 1: Taste gedrückt
    lcd.setCursor(0, 0);
    lcd.print("Taste gedrueckt:");
    
    // Zeile 2: Tastenname
    lcd.setCursor(0, 1);
    lcd.print(text);

    // Zeile 3: HEX-Code der Taste (Original-Signal)
    lcd.setCursor(0, 2);
    lcd.print("HEX: 0x");
    lcd.print(hexCode, HEX); 

    // Zeile 4: Command-Code (vereinfachter Code)
    lcd.setCursor(0, 3);
    lcd.print("CMD: ");
    lcd.print(commandCode);
}

// Funktion zur Zuordnung der IR-Codes zu den Tasten
void translateIR()
{
    unsigned long hexCode = receiver.decodedIRData.decodedRawData; // Empfangener echter HEX-Code
    int commandCode = receiver.decodedIRData.command; // Vereinfachter "Command"-Code

    switch (commandCode) {
        case 162: lcdPrint("POWER", hexCode, commandCode); break;
        case 226: lcdPrint("MENU", hexCode, commandCode); break;
        case 34:  lcdPrint("TEST", hexCode, commandCode); break;
        case 2:   lcdPrint("PLUS", hexCode, commandCode); break;
        case 194: lcdPrint("BACK", hexCode, commandCode); break;
        case 224: lcdPrint("PREV.", hexCode, commandCode); break;
        case 168: lcdPrint("PLAY", hexCode, commandCode); break;
        case 144: lcdPrint("NEXT", hexCode, commandCode); break;
        case 104: lcdPrint("0", hexCode, commandCode); break;
        case 152: lcdPrint("MINUS", hexCode, commandCode); break;
        case 176: lcdPrint("C", hexCode, commandCode); break;
        case 48:  lcdPrint("1", hexCode, commandCode); break;
        case 24:  lcdPrint("2", hexCode, commandCode); break;
        case 122: lcdPrint("3", hexCode, commandCode); break;
        case 16:  lcdPrint("4", hexCode, commandCode); break;
        case 56:  lcdPrint("5", hexCode, commandCode); break;
        case 90:  lcdPrint("6", hexCode, commandCode); break;
        case 66:  lcdPrint("7", hexCode, commandCode); break;
        case 74:  lcdPrint("8", hexCode, commandCode); break;
        case 82:  lcdPrint("9", hexCode, commandCode); break;
        default:
            lcd.clear();
            lcd.setCursor(0, 0);
            lcd.print("Unbekannter Code:");
            lcd.setCursor(0, 1);
            lcd.print("HEX: 0x");
            lcd.print(hexCode, HEX);
            lcd.setCursor(0, 2);
            lcd.print("CMD: ");
            lcd.print(commandCode);
    }
}
