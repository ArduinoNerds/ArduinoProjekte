void setup() {
  pinMode(13, OUTPUT); // Pin 13 als Ausgang setzen
  pinMode(12, OUTPUT); // Pin 12 als Ausgang setzen
}

void loop() {
  digitalWrite(13, HIGH); // LED an Pin 13 einschalten
  digitalWrite(12, LOW);  // LED an Pin 12 ausschalten
  delay(500); // 500ms warten

  digitalWrite(13, LOW);  // LED an Pin 13 ausschalten
  digitalWrite(12, HIGH); // LED an Pin 12 einschalten
  delay(500); // 500ms warten
}
