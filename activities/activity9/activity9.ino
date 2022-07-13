#define LED_PIN 12

unsigned long previousTimeLEDBlink = millis();
unsigned long blinkDelay = 500;

int LEDState = LOW;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    int data = Serial.parseInt();
    if ((data >= 100) && (data <= 4000)) {
      blinkDelay = data;
    }
  }

  unsigned long timeNow = millis();
  if (timeNow - previousTimeLEDBlink > blinkDelay) {
    if (LEDState == LOW) {
      LEDState = HIGH;
    }
    else {
      LEDState = LOW;
    }
    digitalWrite(LED_PIN, LEDState);
    previousTimeLEDBlink += blinkDelay;
  }
}
