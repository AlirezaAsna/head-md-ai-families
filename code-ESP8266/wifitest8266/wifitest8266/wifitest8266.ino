#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Adafruit_NeoPixel.h>
#include <math.h>

const char* ssid = "Media_Design_Network";
const char* password = "mdnetwork";

const int magnetPin = D1;
const int ledPin = D2;
#define NUMPIXELS 8

ESP8266WebServer server(80);
Adafruit_NeoPixel strip(NUMPIXELS, ledPin, NEO_GRB + NEO_KHZ800);

enum Mode {
  MODE_STAGE1,
  MODE_STAGE2,
  MODE_STAGE3,
  MODE_REST
};

Mode currentMode = MODE_REST;

// ---------- WEB PAGE ----------
const char webpage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>Magnet Control</title>
  <style>
    body { font-family: Arial; text-align: center; background: #111; color: #eee; margin-top: 40px; }
    button {
      width: 220px; height: 56px; margin: 10px; font-size: 18px;
      border: none; border-radius: 12px; cursor: pointer;
    }
    .b1 { background: #ffffff; color: #111; }
    .b2 { background: orange; color: #111; }
    .b3 { background: #ff3b30; color: white; }
    .b4 { background: #4da3ff; color: white; }
  </style>
</head>
<body>
  <h1>Wemos Control</h1>
  <p>Select a stage:</p>
  <p><a href="/stage1"><button class="b1">Playing together</button></a></p>
  <p><a href="/stage2"><button class="b2">It's been a while you haven't meet</button></a></p>
  <p><a href="/stage3"><button class="b3">You need to meet him!</button></a></p>
  <p><a href="/rest"><button class="b4">Take a moment to think about him</button></a></p>
</body>
</html>
)rawliteral";

// ---------- HELPERS ----------
void setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(r, g, b));
  }
  strip.show();
}

void clearStrip() {
  strip.clear();
  strip.show();
}

void handleRoot() {
  server.send_P(200, "text/html", webpage);
}

void handleStage1() {
  currentMode = MODE_STAGE1;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStage2() {
  currentMode = MODE_STAGE2;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleStage3() {
  currentMode = MODE_STAGE3;
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleRest() {
  currentMode = MODE_REST;
  server.sendHeader("Location", "/");
  server.send(303);
}

// ---------- ANIMATION STATE ----------
unsigned long lastAnim = 0;
int waveIndex = 0;
int inwardStep = 0;
bool blinkState = false;

// ---------- STAGE LOGIC ----------
void runStage1() {
  // Full power magnets
  analogWrite(magnetPin, 1023);

  // White wave
  if (millis() - lastAnim > 120) {
    lastAnim = millis();
    strip.clear();
    strip.setPixelColor(waveIndex, strip.Color(255, 255, 255));
    strip.show();
    waveIndex++;
    if (waveIndex >= NUMPIXELS) waveIndex = 0;
  }
}

void runStage2() {
  // Half power magnets
  analogWrite(magnetPin, 512);

  // From ends to center, orange
  if (millis() - lastAnim > 250) {
    lastAnim = millis();
    strip.clear();

    if (inwardStep >= 0) {
      strip.setPixelColor(0, strip.Color(255, 80, 0));
      strip.setPixelColor(7, strip.Color(255, 80, 0));
    }
    if (inwardStep >= 1) {
      strip.setPixelColor(1, strip.Color(255, 80, 0));
      strip.setPixelColor(6, strip.Color(255, 80, 0));
    }
    if (inwardStep >= 2) {
      strip.setPixelColor(2, strip.Color(255, 80, 0));
      strip.setPixelColor(5, strip.Color(255, 80, 0));
    }
    if (inwardStep >= 3) {
      strip.setPixelColor(3, strip.Color(255, 80, 0));
      strip.setPixelColor(4, strip.Color(255, 80, 0));
    }

    strip.show();
    inwardStep++;
    if (inwardStep > 4) inwardStep = 0;
  }
}

void runStage3() {
  // Zero power magnets
  analogWrite(magnetPin, 0);

  // Blinking red
  if (millis() - lastAnim > 300) {
    lastAnim = millis();
    blinkState = !blinkState;

    if (blinkState) {
      setAllPixels(255, 0, 0);
    } else {
      clearStrip();
    }
  }
}

void runRest() {
  // Magnets off
  analogWrite(magnetPin, 0);

  // Smooth breathing standby light
  float t = millis() / 1000.0;
  float breath = (sin(t * 2.0) + 1.0) * 0.5;   // 0..1
  uint8_t brightness = (uint8_t)(breath * 80.0); // soft standby glow

  for (int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, brightness));
  }
  strip.show();
}

void setup() {
  pinMode(magnetPin, OUTPUT);
  analogWriteRange(1023);

  strip.begin();
  strip.show();

  Serial.begin(115200);
  delay(500);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/stage1", handleStage1);
  server.on("/stage2", handleStage2);
  server.on("/stage3", handleStage3);
  server.on("/rest", handleRest);
  server.begin();

  Serial.println("Web server started");
}

void loop() {
  server.handleClient();

  switch (currentMode) {
    case MODE_STAGE1:
      runStage1();
      break;
    case MODE_STAGE2:
      runStage2();
      break;
    case MODE_STAGE3:
      runStage3();
      break;
    case MODE_REST:
      runRest();
      break;
  }
}