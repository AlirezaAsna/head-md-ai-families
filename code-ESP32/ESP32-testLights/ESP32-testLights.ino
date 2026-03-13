const int magnetPin = 13;
const int ledPin = 27;

void setup() {
  ledcAttach(magnetPin, 5000, 8);
  ledcAttach(ledPin, 5000, 8);
}

void loop() {
  for (int pwm = 255; pwm >= 0; pwm--) {
    ledcWrite(magnetPin, pwm);
    ledcWrite(ledPin, pwm);
    delay(30);
  }

  delay(1000);

  for (int pwm = 0; pwm <= 255; pwm ++) {
    ledcWrite(magnetPin, pwm);
    ledcWrite(ledPin, pwm);
    delay(30);
  }

  delay(1000);
}