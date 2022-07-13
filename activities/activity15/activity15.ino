#define LED_1_PIN 12
#define LED_2_PIN 11
#define LED_3_PIN 10
#define ECHO_PIN 3
#define TRIGGER_PIN 4

unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;

volatile unsigned long pulseInTimeBegin;
volatile unsigned long pulseInTimeEnd;
volatile bool newDistanceAvailable = false;

double previousDistance = 400.0;

void triggerUltrasonicSensor()
{
  digitalWrite(TRIGGER_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);
}

double getUltrasonicDistance()
{
  double durationMicros = pulseInTimeEnd - pulseInTimeBegin;
  double distance = durationMicros / 58.0; // cm (148.0: inches)
  if (distance > 400.0) {
    return previousDistance;
  }
  distance = previousDistance * 0.7 + distance * 0.3;
  previousDistance = distance;
  return distance;
}

void echoPinInterrupt()
{
  if (digitalRead(ECHO_PIN) == HIGH) { // start measuring
    pulseInTimeBegin = micros();
  }
  else { // stop measuring
    pulseInTimeEnd = micros();
    newDistanceAvailable = true;
  }
}

void powerLEDsFromDistance(double distance)
{
  if (distance >= 100) {
    digitalWrite(LED_1_PIN, LOW);
    digitalWrite(LED_2_PIN, LOW);
    digitalWrite(LED_3_PIN, HIGH);
  }
  else if (distance >= 15) {
    digitalWrite(LED_1_PIN, LOW);
    digitalWrite(LED_2_PIN, HIGH);
    digitalWrite(LED_3_PIN, LOW);
  }
  else {
    digitalWrite(LED_1_PIN, HIGH);
    digitalWrite(LED_2_PIN, LOW);
    digitalWrite(LED_3_PIN, LOW);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(LED_1_PIN, OUTPUT);
  pinMode(LED_2_PIN, OUTPUT);
  pinMode(LED_3_PIN, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoPinInterrupt,
                  CHANGE);
}

void loop() {
  unsigned long timeNow = millis();
  
  if (timeNow - lastTimeUltrasonicTrigger > ultrasonicTriggerDelay) {
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }

  if (newDistanceAvailable) {
    newDistanceAvailable = false;
    double distance = getUltrasonicDistance();
    powerLEDsFromDistance(distance);
    Serial.println(distance);
  }
}
