#define LED_PIN 11

void setup() {
  // Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for (int i = 0; i <= 255; i++) {
    // Serial.println(i);
    analogWrite(LED_PIN, i);
    delay(5);
  }

  for (int i = 255; i >= 0; i--) {
    // Serial.println(i);
    analogWrite(LED_PIN, i);
    delay(5);
  }
}
