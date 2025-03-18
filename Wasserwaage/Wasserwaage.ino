#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050_tockn.h>

// I2C-Adresse für LCD2004 (0x27 oder 0x3F)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// MPU9250 Sensor
MPU6050 mpu(Wire);

int zeroOffsetX = 0; // Dynamischer Nullpunkt X
int zeroOffsetY = 0; // Dynamischer Nullpunkt Y
int zeroOffsetZ = 0; // Dynamischer Nullpunkt Z

void setup() {
    Wire.begin();
    Serial.begin(9600);
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Kalibriere...");

    mpu.begin();
    mpu.calcGyroOffsets(true);

    delay(2000);

    // **Automatische Kalibrierung:**
    zeroOffsetX = (int)mpu.getAngleX();
    zeroOffsetY = (int)mpu.getAngleY();
    zeroOffsetZ = (int)mpu.getAngleZ();

    Serial.println("Kalibrierte Offsets:");
    Serial.print("X: "); Serial.println(zeroOffsetX);
    Serial.print("Y: "); Serial.println(zeroOffsetY);
    Serial.print("Z: "); Serial.println(zeroOffsetZ);

    lcd.clear();
}

void loop() {
    mpu.update();
    int angleX = ((int)mpu.getAngleX() - zeroOffsetX + 360) % 360;
    int angleY = ((int)mpu.getAngleY() - zeroOffsetY + 360) % 360;
    int angleZ = ((int)mpu.getAngleZ() - zeroOffsetZ + 360) % 360;

    // LCD-Anzeige für Winkel in 3 Reihen
    lcd.setCursor(0, 0);
    lcd.print("X-Achse:");
    lcd.setCursor(10, 0);
    lcd.print(angleX);
    lcd.print((char)223);

    lcd.setCursor(0, 1);
    lcd.print("Y-Achse:");
    lcd.setCursor(10, 1);
    lcd.print(angleY);
    lcd.print((char)223);

    lcd.setCursor(0, 2);
    lcd.print("Z-Achse:");
    lcd.setCursor(10, 2);
    lcd.print(angleZ);
    //lcd.print((char)223);

    // Debugging-Ausgabe im Serial Monitor
    Serial.println("--- MPU9250 Werte ---");
    Serial.print("X-Winkel: "); Serial.println(angleX);
    Serial.print("Y-Winkel: "); Serial.println(angleY);
    Serial.print("Z-Winkel: "); Serial.println(angleZ);
    Serial.println("---------------------");

    delay(200);
}
