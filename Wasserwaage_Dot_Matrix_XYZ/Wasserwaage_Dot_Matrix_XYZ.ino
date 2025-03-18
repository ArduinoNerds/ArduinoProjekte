#include <Wire.h>
#include <MPU6050_tockn.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <LiquidCrystal_I2C.h>

// Definiere die Pins für die MAX7219-Matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4  // Anzahl der Module
#define CS_PIN 10      // Chip Select für MAX7219

// Initialisiere die Matrix mit MD_MAX72XX
MD_MAX72XX matrix = MD_MAX72XX(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

// LCD 2004 I2C
LiquidCrystal_I2C lcd(0x27, 20, 4);

// MPU9250 Sensor
MPU6050 mpu(Wire);

int zeroOffsetX = 0; // Dynamischer Nullpunkt X
int zeroOffsetY = 0; // Dynamischer Nullpunkt Y
int matrixWidth = MAX_DEVICES * 8; // Breite der gesamten Matrix
int matrixHeight = 8; // Höhe der Matrix
int bubbleX = matrixWidth / 2; // Startposition der Blase in X
int bubbleY = matrixHeight / 2; // Startposition der Blase in Y

void setup() {
    Wire.begin();
    Serial.begin(9600);

    matrix.begin();
    matrix.control(MD_MAX72XX::INTENSITY, 5);  // Helligkeit (0-15)
    matrix.clear();

    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 1);
    lcd.print("ArduinoNerds!");

    Serial.println("Kalibriere MPU...");
    mpu.begin();
    mpu.calcGyroOffsets(true);

    delay(2000);

    // Nullpunkt-Kalibrierung
    zeroOffsetX = (int)mpu.getAngleX(); // Speichert den aktuellen Wert als Nullpunkt X
    zeroOffsetY = (int)mpu.getAngleY(); // Speichert den aktuellen Wert als Nullpunkt Y

    Serial.print("Kalibrierter Offset X: ");
    Serial.println(zeroOffsetX);
    Serial.print("Kalibrierter Offset Y: ");
    Serial.println(zeroOffsetY);
}

void loop() {
    mpu.update();
    int angleX = ((int)mpu.getAngleX() - zeroOffsetX); // Nullpunkt-Korrektur X
    int angleY = ((int)mpu.getAngleY() - zeroOffsetY); // Nullpunkt-Korrektur Y

    // **Blase auf der Dot-Matrix bewegen in X und Y**
    bubbleX = map(angleX, -45, 45, 0, matrixWidth - 2); // X-Position berechnen
    bubbleY = map(angleY, -45, 45, 0, matrixHeight - 2); // Y-Position berechnen
    bubbleX = constrain(bubbleX, 0, matrixWidth - 2); // Begrenzung für 2x2 Blase in X
    bubbleY = constrain(bubbleY, 0, matrixHeight - 2); // Begrenzung für 2x2 Blase in Y

    matrix.clear();

    // **Blase als 2x2 Quadrat darstellen**
    matrix.setPoint(bubbleY, bubbleX, true);
    matrix.setPoint(bubbleY, bubbleX + 1, true);
    matrix.setPoint(bubbleY + 1, bubbleX, true);
    matrix.setPoint(bubbleY + 1, bubbleX + 1, true);

    matrix.update(); // Matrix aktualisieren

    // **Lauftext auf LCD 2004**
    static int scrollPos = 0;
    String text = "   ArduinoNerds!   ";
    lcd.setCursor(0, 3);
    lcd.print(text.substring(scrollPos, scrollPos + 20));
    scrollPos++;
    if (scrollPos > text.length() -20) scrollPos = 0;

    Serial.print("X-Winkel: ");
    Serial.print(angleX);
    Serial.print(" | Y-Winkel: ");
    Serial.print(angleY);
    Serial.print(" | Blasen-Position: X=");
    Serial.print(bubbleX);
    Serial.print(" Y=");
    Serial.println(bubbleY);

    delay(300);
}
