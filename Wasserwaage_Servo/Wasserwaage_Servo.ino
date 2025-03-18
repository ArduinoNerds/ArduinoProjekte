#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MPU6050_tockn.h>
#include <Servo.h>

// I2C-Adresse für LCD2004 (0x27 oder 0x3F)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// MPU9250 Sensor
MPU6050 mpu(Wire);

// Servo
Servo myServo;
int servoPin = 9;

// Anpassbarer Nullpunkt
int zeroOffset = 0;

// Angepasster Bereich für den Servo
int minAngle = -60;  // Minimaler Sensorwinkel
int maxAngle = 60;   // Maximaler Sensorwinkel
int servoMin = 0;    // Mindestwert des Servos
int servoMax = 180;  // Maximaler Servo-Winkel

void setup() {
    Wire.begin();
    Serial.begin(9600);
    lcd.begin(20, 4);
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Kalibriere...");

    mpu.begin();
    mpu.calcGyroOffsets(true);

    myServo.attach(servoPin); // Servo an Pin 9 anschließen

    delay(2000);

    // **Automatische Kalibrierung:**
    zeroOffset = (int)mpu.getAngleX(); // Speichert den aktuellen Wert als Nullpunkt

    Serial.print("Kalibrierter Offset: ");
    Serial.println(zeroOffset);

    lcd.clear();
}

void loop() {
    mpu.update();
    int angleX = ((int)mpu.getAngleX() - zeroOffset); // Nullpunkt-Korrektur

    // **Servo-Bewegung mit begrenztem Bereich auf 0 bis 165°**
    int servoAngle = map(angleX, minAngle, maxAngle, servoMin, servoMax);
    servoAngle = constrain(servoAngle, servoMin, servoMax); // Begrenzung auf 0° bis 165°

    myServo.write(servoAngle);

    // LCD-Update
    lcd.setCursor(0, 0);
    lcd.print("X:     ");
    lcd.setCursor(0, 0);
    lcd.print("X: ");
    lcd.print(angleX);
    lcd.print((char)223); // Grad-Symbol

    // Debugging: Ausgabe im Serial Monitor
    Serial.print("X-Winkel: ");
    Serial.print(angleX);
    Serial.print(" | Servo-Winkel: ");
    Serial.println(servoAngle);

    delay(100);
}