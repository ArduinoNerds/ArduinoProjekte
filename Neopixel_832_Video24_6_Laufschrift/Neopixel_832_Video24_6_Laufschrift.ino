#include <Adafruit_NeoPixel.h>

#define LED_PIN 6      
#define MATRIX_WIDTH 32  
#define MATRIX_HEIGHT 8  
#define LED_COUNT (MATRIX_WIDTH * MATRIX_HEIGHT)
#define BRIGHTNESS 100    
#define SCROLL_DELAY 50  // Geschwindigkeit der Laufschrift (Millisekunden)

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// **HIER SCHREIBST DU DEINEN TEXT**
String message = "ARDUINONERDS";  // Beliebigen Text hier einfügen

// Alphabet (A-Z), Zahlen (0-9) und Leerzeichen
const char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";

// **Alle Buchstaben als 8x8-Bitmaps**
byte font[][8] = {
  {0b00111100, 0b01000010, 0b01000010, 0b01111110, 0b01000010, 0b01000010, 0b01000010, 0b00000000},  // A
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b00000000},  // B
  {0b00111100, 0b01000010, 0b01000000, 0b01000000, 0b01000000, 0b01000010, 0b00111100, 0b00000000},  // C
  {0b01111000, 0b01000100, 0b01000010, 0b01000010, 0b01000010, 0b01000100, 0b01111000, 0b00000000},  // D
  {0b01111110, 0b01000000, 0b01000000, 0b01111100, 0b01000000, 0b01000000, 0b01111110, 0b00000000},  // E
  {0b01111110, 0b01000000, 0b01000000, 0b01111100, 0b01000000, 0b01000000, 0b01000000, 0b00000000},  // F
  {0b00111100, 0b01000010, 0b01000000, 0b01001110, 0b01000010, 0b01000010, 0b00111100, 0b00000000},  // G
  {0b01000010, 0b01000010, 0b01000010, 0b01111110, 0b01000010, 0b01000010, 0b01000010, 0b00000000},  // H
  {0b00111100, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00111100, 0b00000000},  // I
  {0b00001110, 0b00000100, 0b00000100, 0b00000100, 0b00000100, 0b01000100, 0b00111000, 0b00000000},  // J
  {0b01000010, 0b01000100, 0b01001000, 0b01110000, 0b01001000, 0b01000100, 0b01000010, 0b00000000},  // K
  {0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01000000, 0b01111110, 0b00000000},  // L
  {0b01000010, 0b01100110, 0b01011010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00000000},  // M
  {0b01000010, 0b01100010, 0b01010010, 0b01001010, 0b01000110, 0b01000010, 0b01000010, 0b00000000},  // N
  {0b00111100, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00111100, 0b00000000},  // O
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01000000, 0b01000000, 0b01000000, 0b00000000},  // P
  {0b00111100, 0b01000010, 0b01000010, 0b01000010, 0b01001010, 0b01000110, 0b00111110, 0b00000000},  // Q
  {0b01111100, 0b01000010, 0b01000010, 0b01111100, 0b01000100, 0b01000010, 0b01000010, 0b00000000},  // R
  {0b00111100, 0b01000010, 0b01000000, 0b00111100, 0b00000010, 0b01000010, 0b00111100, 0b00000000},  // S
  {0b01111110, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00000000},  // T
  {0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b00111100, 0b00000000},  // U
  {0b01000010, 0b01000010, 0b01000010, 0b00100100, 0b00100100, 0b00011000, 0b00011000, 0b00000000},  // V
  {0b01000010, 0b01000010, 0b01000010, 0b01000010, 0b01011010, 0b01100110, 0b01000010, 0b00000000},  // W
  {0b01000010, 0b01000010, 0b00100100, 0b00011000, 0b00011000, 0b00100100, 0b01000010, 0b00000000},  // X
  {0b01000010, 0b00100100, 0b00100100, 0b00011000, 0b00001000, 0b00001000, 0b00001000, 0b00000000},  // Y
  {0b01111110, 0b00000010, 0b00000100, 0b00001000, 0b00010000, 0b00100000, 0b01111110, 0b00000000}   // Z
};




//  NEUE ERWEITERUNG: Farbwechsel für Laufschrift hinzugefügt
// Der Text wechselt beim Scrollen langsam die Farbe (Regenbogen-Effekt)

// **Farbwechsel-Variablen**
int hue = 0;  // Farbe ändert sich von 0-255 (Regenbogen-Farbzyklus)

void setup() {
    strip.begin();
    strip.setBrightness(BRIGHTNESS);
    strip.show();
}

void loop() {
    for (int pos = MATRIX_WIDTH; pos >= -(int)message.length() * 8; pos--) {  
        strip.clear();
        
        // Farbton für Regenbogen-Effekt
        hue += 5;  // Farbe wechselt langsam über die Zeit

        for (int i = 0; i < message.length(); i++) {
            int letterIndex = getLetterIndex(message[i]);
            if (letterIndex >= 0) {
                uint32_t letterColor = colorFromHue(hue + (i * 15));  // Jede Buchstabe leicht andere Farbe
                drawLetter(pos + (i * 8), 0, font[letterIndex], letterColor);
            }
        }

        strip.show();
        delay(SCROLL_DELAY);
    }
}

// 🚀 NEUE FUNKTION: HSL-zu-RGB-Funktion für den Farbwechsel
uint32_t colorFromHue(int hue) {
    hue = hue % 256;  // Wertebereich 0-255
    int r, g, b;

    if (hue < 85) {
        r = hue * 3;
        g = 255 - hue * 3;
        b = 0;
    } else if (hue < 170) {
        hue -= 85;
        r = 255 - hue * 3;
        g = 0;
        b = hue * 3;
    } else {
        hue -= 170;
        r = 0;
        g = hue * 3;
        b = 255 - hue * 3;
    }
    return strip.Color(r, g, b);
}

// Zeichnet einen Buchstaben auf das LED-Panel
void drawLetter(int xOffset, int yOffset, byte letter[], uint32_t color) {
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 8; x++) {
            if (letter[y] & (1 << (7 - x))) {
                int pixel = getPixelNumber(x + xOffset, y + yOffset);
                if (pixel >= 0 && pixel < LED_COUNT) {
                    strip.setPixelColor(pixel, color);
                }
            }
        }
    }
}

// Konvertiert einen Buchstaben in einen Index für die Font-Tabelle
int getLetterIndex(char c) {
    for (int i = 0; i < sizeof(alphabet) - 1; i++) {
        if (alphabet[i] == c) return i;
    }
    return -1;  // Falls Buchstabe nicht gefunden wird
}

// Mapping-Funktion für dein 8x32 Panel
int getPixelNumber(int x, int y) {
    if (x % 2 == 0) {
        return (x * MATRIX_HEIGHT) + y;  
    } else {
        return (x * MATRIX_HEIGHT) + (MATRIX_HEIGHT - 1 - y);  
    }
}
