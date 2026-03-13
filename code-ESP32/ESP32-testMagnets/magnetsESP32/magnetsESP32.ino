#include <Adafruit_NeoPixel.h>

const int magnetPin = 13;
const int ledPin = 27;

#define NUMPIXELS 8

Adafruit_NeoPixel strip(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();

  ledcAttach(magnetPin, 5000, 8);
}

void loop() {
  stage1();
  stage2();
  stage3();

  // after stage 3, everything stays off for 10 seconds
  ledcWrite(magnetPin, 0);
  strip.clear();
  strip.show();
  delay(10000);
}

// ==================== STAGE 1 ====================
// Full power magnets
// White wave pattern
void stage1() {
  ledcWrite(magnetPin, 255);

  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.clear();
      strip.setPixelColor(i, strip.Color(255, 255, 255));
      strip.show();
      delay(120);

      if (millis() - startTime >= 10000) break;
    }
  }
}

// ==================== STAGE 2 ====================
// Half power magnets
// LEDs from 1,8 to middle in orange
void stage2() {
  ledcWrite(magnetPin, 128);

  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {
    strip.clear();
    strip.show();
    delay(150);

    strip.setPixelColor(0, strip.Color(255, 80, 0));
    strip.setPixelColor(7, strip.Color(255, 80, 0));
    strip.show();
    delay(250);

    strip.setPixelColor(1, strip.Color(255, 80, 0));
    strip.setPixelColor(6, strip.Color(255, 80, 0));
    strip.show();
    delay(250);

    strip.setPixelColor(2, strip.Color(255, 80, 0));
    strip.setPixelColor(5, strip.Color(255, 80, 0));
    strip.show();
    delay(250);

    strip.setPixelColor(3, strip.Color(255, 80, 0));
    strip.setPixelColor(4, strip.Color(255, 80, 0));
    strip.show();
    delay(400);

    strip.clear();
    strip.show();
    delay(200);
  }
}

// ==================== STAGE 3 ====================
// Zero power magnets
// LEDs blinking red for 5 sec
void stage3() {
  ledcWrite(magnetPin, 0);

  unsigned long startTime = millis();

  while (millis() - startTime < 5000) {
    for (int i = 0; i < NUMPIXELS; i++) {
      strip.setPixelColor(i, strip.Color(255, 0, 0));
    }
    strip.show();
    delay(300);

    strip.clear();
    strip.show();
    delay(300);
  }
}