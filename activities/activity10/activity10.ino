#define LED_1_PIN 12
#define LED_2_PIN 11
#define LED_3_PIN 10

#define BUTTON_PIN 2

#define POTENTIOMETER_PIN A2

unsigned long previousTimeLED1Blink = millis();
unsigned long blinkDelayLED1 = 500;

int LED1State = LOW;

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10);

  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);

  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  // action 1
  if (Serial.available() > 0) {
    int data = Serial.parseInt();
    if ((data >= 100) && (data <= 4000)) {
      blinkDelayLED1 = data;
    }
  }

  // action 2
  unsigned long timeNow = millis();
  if (timeNow - previousTimeLED1Blink > blinkDelayLED1) {
    if (LED1State == LOW) {
      LED1State = HIGH;
    }
    else {
      LED1State = LOW;
    }
    digitalWrite(LED_1_PIN, LED1State);
    previousTimeLED1Blink += blinkDelayLED1;
  }

  // action 3
  int potentiometerValue = analogRead(POTENTIOMETER_PIN);
  int intensity = potentiometerValue / 4;
  analogWrite(LED_2_PIN, intensity);
  
  // action 4
  if (digitalRead(BUTTON_PIN) == HIGH) {
    digitalWrite(LED_3_PIN, HIGH);
  }
  else {
    digitalWrite(LED_3_PIN, LOW);
  }
}
