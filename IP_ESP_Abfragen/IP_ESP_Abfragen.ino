#include <ESP8266WiFi.h>

// Zugangsdaten
const char* ssid = "DEIN-WLAN-NAME";
const char* password = "DEIN-WLAN-PASSWORT";

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println("Starte WLAN-Verbindung...");

  WiFi.begin(ssid, password);

  // Auf Verbindung warten
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Verbunden!
  Serial.println();
  Serial.println("WLAN verbunden!");
  Serial.print("IP-Adresse des ESP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // nichts
}
