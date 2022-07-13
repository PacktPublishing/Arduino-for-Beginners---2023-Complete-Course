#define LED_1_PIN 12
#define LED_2_PIN 11
#define LED_3_PIN 10

#define BUTTON_PIN 2

#define LED_PIN_ARRAY_SIZE 3

int LEDBlinkState = 1;

byte LEDPinArray[LED_PIN_ARRAY_SIZE] = 
      {LED_1_PIN, LED_2_PIN, LED_3_PIN};

void setLEDPinModes()
{
  for (int i = 0; i < LED_PIN_ARRAY_SIZE; i++) {
    pinMode(LEDPinArray[i], OUTPUT);
  }  
}

void turnOffAllLEDs()
{
  for (int i = 0; i < LED_PIN_ARRAY_SIZE; i++) {
    digitalWrite(LEDPinArray[i], LOW);
  }
}

void toggleLEDs()
{
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
}

void setup() {
  pinMode(BUTTON_PIN, INPUT);

  setLEDPinModes();
  turnOffAllLEDs();
}

void loop() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    toggleLEDs();
    delay(300); 
  }
}
