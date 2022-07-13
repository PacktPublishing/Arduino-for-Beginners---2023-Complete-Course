#define LED_1_PIN 12
#define LED_2_PIN 11
#define LED_3_PIN 10

#define BUTTON_PIN 2

int LEDBlinkState = 1;

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);

  digitalWrite(LED_1_PIN, LOW);
  digitalWrite(LED_2_PIN, LOW);
  digitalWrite(LED_3_PIN, LOW);
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    if (LEDBlinkState == 1) {
      digitalWrite(LED_1_PIN, HIGH);
      digitalWrite(LED_2_PIN, LOW);
      digitalWrite(LED_3_PIN, HIGH);
      LEDBlinkState = 2;
    }
    else {
      digitalWrite(LED_1_PIN, LOW);
      digitalWrite(LED_2_PIN, HIGH);
      digitalWrite(LED_3_PIN, LOW);
      LEDBlinkState = 1;
    } 
    delay(300);
  }
}
