#include <LiquidCrystal.h>

#define LCD_RS_PIN A5
#define LCD_E_PIN A4
#define LCD_D4_PIN 6
#define LCD_D5_PIN 7
#define LCD_D6_PIN 8
#define LCD_D7_PIN 9

#define ECHO_PIN 3
#define TRIGGER_PIN 4

LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN,
                  LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

int cursorLine = 0;

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

void printDistanceOnLCD(double distance)
{
  lcd.setCursor(0, 1);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print("  ");
}

void setup() {
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Rate: ");
  lcd.print(ultrasonicTriggerDelay);
  lcd.print(" ms.");

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
    printDistanceOnLCD(distance);
  }
}
