#include <Adafruit_NeoPixel.h>

const int magnetPin = D1;
const int ledPin = D2;

#define NUMPIXELS 8

Adafruit_NeoPixel strip(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

void setup() {

  strip.begin();
  strip.show();

  pinMode(magnetPin, OUTPUT);

}

void loop() {

  stage1();
  stage2();
  stage3();

  analogWrite(magnetPin, 0);
  strip.clear();
  strip.show();

  delay(10000);
}


// ---------- STAGE 1 ----------

void stage1() {

  analogWrite(magnetPin, 1023);

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


// ---------- STAGE 2 ----------

void stage2() {

  analogWrite(magnetPin, 512);

  unsigned long startTime = millis();

  while (millis() - startTime < 10000) {

    strip.clear();
    strip.show();

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


// ---------- STAGE 3 ----------

void stage3() {

  analogWrite(magnetPin, 0);

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