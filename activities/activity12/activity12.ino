#define BUTTON_PIN 2

volatile unsigned long lastTimeButtonPressed = millis();
unsigned long debounceDelay = 50;

volatile bool buttonPressed = false;

int counter = 0;

void buttonPressedInterrupt()
{
  unsigned long timeNow = millis();
  if (timeNow - lastTimeButtonPressed > debounceDelay) {
    lastTimeButtonPressed = timeNow;
    buttonPressed = true;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN),
                  buttonPressedInterrupt,
                  RISING);
}

void loop() {
  if (buttonPressed) {
    buttonPressed = false;
    counter++;
    Serial.print("Counter is : ");
    Serial.println(counter);
  }
}
