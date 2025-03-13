#include <Wire.h>

void setup() {
  Serial.begin(9600);
  Serial.println("Suche nach I2C-Geräten...");
  Wire.begin();
}

void loop() {
  for (byte address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    if (Wire.endTransmission() == 0) {
      Serial.print("I2C-Gerät gefunden bei Adresse: 0x");
      Serial.println(address, HEX);
      delay(500);
    }
  }
  Serial.println("Scan abgeschlossen!");
  delay(5000); // 5 Sekunden warten und dann erneut scannen
}
