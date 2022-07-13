#define LED_PIN 11
#define POTENTIOMETER_PIN A2

void setup() {
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int LEDBrightness = potentiometerValue / 4;
  analogWrite(LED_PIN, LEDBrightness);
}
