#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <MD_Parola.h>
#include <MD_MAX72XX.h>
#include <SPI.h>
#include <Encoder.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// LCD2004
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Dot-Matrix
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define DATA_PIN 13 // D7
#define CLK_PIN 14  // D5
#define CS_PIN 15   // D8

MD_Parola matrix = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

// Encoder
Encoder myEnc(12, 0);  // D6, D3
long lastPosition = -1;

// WLAN
const char* ssid = "EUER WLAN NAME";
const char* password = "EUER WLAN PASSWORT";
ESP8266WebServer server(80);

// Systemdaten
String values[] = {
  "Warte auf Daten...", "0 MHz", "0%", "0 GB 0%", "0%",
  "0MB", "0MB", "0 Min", "?"
};
const char* labels[] = {
  "CPU Name", "CPU Takt", "CPU Last", "Arbeitsspeicher", "Cpu Kerne",
  "Netz TX", "Netz RX", "Uptime", "Betriebssystem"
};
int numItems = sizeof(labels) / sizeof(labels[0]);
int currentIndex = 0;

// Begrüßung
bool welcomeShown = false;

void handleUpdate() {
  if (server.hasArg("cpu_name")) values[0] = server.arg("cpu_name");
  if (server.hasArg("cpu_freq")) values[1] = server.arg("cpu_freq") + " MHz";
  if (server.hasArg("cpu_usage")) values[2] = server.arg("cpu_usage") + "%";
  if (server.hasArg("ram_total") && server.hasArg("ram_usage"))
    values[3] = server.arg("ram_total") + " GB " + server.arg("ram_usage") + "%";
  if (server.hasArg("cpu_cores")) values[4] = server.arg("cpu_cores") + " Kerne";
  if (server.hasArg("net_sent")) values[5] = server.arg("net_sent") + "MB";
  if (server.hasArg("net_recv")) values[6] = server.arg("net_recv") + "MB";
  if (server.hasArg("uptime")) values[7] = server.arg("uptime") + " Min";
  if (server.hasArg("os_name")) values[8] = server.arg("os_name");

  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);

  // LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Auswahl:");
  lcd.setCursor(0, 1);
  lcd.print(labels[currentIndex]);

  // Matrix
  matrix.begin();
  matrix.setIntensity(5);
  matrix.displayClear();
  matrix.displayText("Herzlich Willkommen bei Arduino fuer Anfaenger", PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);

  // WLAN
  WiFi.begin(ssid, password);
  Serial.print("Verbindet mit WLAN...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nVerbunden!");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());

  server.on("/update", handleUpdate);
  server.begin();
  Serial.println("Webserver läuft!");
}

void loop() {
  server.handleClient();

  // Begrüßung abspielen
  if (!welcomeShown) {
    if (matrix.displayAnimate()) {
      // Begrüßung ist fertig
      matrix.displayText(values[currentIndex].c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
      welcomeShown = true;
    }
    return;
  }

  // Encoder auslesen
  long newPosition = myEnc.read() / 4;
  if (newPosition != lastPosition) {
    lastPosition = newPosition;
    currentIndex = newPosition % numItems;
    if (currentIndex < 0) currentIndex += numItems;

    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(labels[currentIndex]);

    matrix.displayClear();
    matrix.displayText(values[currentIndex].c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }

  // Wichtig: Text erneut setzen, wenn durchgelaufen
  if (matrix.displayAnimate()) {
    matrix.displayText(values[currentIndex].c_str(), PA_LEFT, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }

  delay(50);
}

