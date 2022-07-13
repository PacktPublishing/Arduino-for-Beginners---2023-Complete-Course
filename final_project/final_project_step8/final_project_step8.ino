#include <LiquidCrystal.h>
#include <IRremote.h>

#define IR_RECEIVE_PIN 5

#define ECHO_PIN 3
#define TRIGGER_PIN 4

#define WARNING_LED_PIN 11
#define ERROR_LED_PIN 12

#define BUTTON_PIN 2

#define LCD_RS_PIN A5
#define LCD_E_PIN A4
#define LCD_D4_PIN 6
#define LCD_D5_PIN 7
#define LCD_D6_PIN 8
#define LCD_D7_PIN 9

#define LOCK_DISTANCE 10.0
#define WARNING_DISTANCE 50.0

// IR button mapping
#define IR_BUTTON_PLAY 64
#define IR_BUTTON_OFF  69
#define IR_BUTTON_EQ   25
#define IR_BUTTON_UP   9
#define IR_BUTTON_DOWN 7

// lcd
LiquidCrystal lcd(LCD_RS_PIN, LCD_E_PIN, LCD_D4_PIN,
                  LCD_D5_PIN, LCD_D6_PIN, LCD_D7_PIN);

// ultrasonic
unsigned long lastTimeUltrasonicTrigger = millis();
unsigned long ultrasonicTriggerDelay = 60;

volatile unsigned long pulseInTimeBegin;
volatile unsigned long pulseInTimeEnd;
volatile bool newDistanceAvailable = false;

double previousDistance = 400.0;

// warning LED
unsigned long lastTimeWarningLEDBlinked = millis();
unsigned long warningLEDDelay = 500;
byte warningLEDState = LOW;

// error LED
unsigned long lastTimeErrorLEDBlinked = millis();
unsigned long errorLEDDelay = 300;
byte errorLEDState = LOW;

// push button
unsigned long lastTimeButtonChanged = millis();
unsigned long buttonDebounceDelay = 50;
byte buttonState;

bool isLocked = false;

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
  double distance = durationMicros / 58.0;
  if (distance > 400.0) {
    return previousDistance;
  }
  distance = previousDistance * 0.6 + distance * 0.4;
  previousDistance = distance;
  return distance;
}

void echoPinInterrupt()
{
  if (digitalRead(ECHO_PIN) == HIGH) { // pulse started
    pulseInTimeBegin = micros();
  }
  else { // pulse stoped
    pulseInTimeEnd = micros();
    newDistanceAvailable = true;
  }
}

void toggleErrorLED()
{
  errorLEDState = (errorLEDState == HIGH) ? LOW : HIGH;
  digitalWrite(ERROR_LED_PIN, errorLEDState);
}

void toggleWarningLED()
{
  warningLEDState = (warningLEDState == HIGH) ? LOW : HIGH;
  digitalWrite(WARNING_LED_PIN, warningLEDState);
}

void setWarningLEDBlinkRateFromDistance(double distance)
{
  // 0 .. 400 cm -> 0 .. 1600 ms
  warningLEDDelay = distance * 4;
  // Serial.println(warningLEDDelay);
}

void lock()
{
  if (!isLocked) {
    // Serial.println("Locking");
    isLocked = true;
    warningLEDState = LOW;
    errorLEDState = LOW;
  }
}

void unlock()
{
  if (isLocked) {
    // Serial.println("Unlocking");
    isLocked = false;
    errorLEDState = LOW;
    digitalWrite(ERROR_LED_PIN, errorLEDState);
  }
}

void printDistanceOnLCD(double distance)
{
  if (isLocked) {
    lcd.setCursor(0, 0);
    lcd.print("!!! Obstacle !!!     ");
    lcd.setCursor(0, 1);
    lcd.print("Press to unlock.   ");
  }
  else {
    lcd.setCursor(0, 0);
    lcd.print("Dist: ");
    lcd.print(distance);
    lcd.print(" cm     ");

    lcd.setCursor(0, 1);
    if (distance > WARNING_DISTANCE) {
      lcd.print("No obstacle.         ");
    }
    else {
      lcd.print("!! Warning !!        ");
    }
  }
}

void handleIRCommand(long command)
{
  switch (command) {
    case IR_BUTTON_PLAY: {
      unlock();
      break;
    }
    case IR_BUTTON_OFF: {
      // todo
      break;
    }
    case IR_BUTTON_EQ: {
      // todo
      break;
    }
    case IR_BUTTON_UP: {
      // todo
      break;
    }
    case IR_BUTTON_DOWN: {
      // todo
      break;
    }
    default: {
      // do nothing
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.begin(16,2);
  IrReceiver.begin(IR_RECEIVE_PIN);
  pinMode(ECHO_PIN, INPUT);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(WARNING_LED_PIN, OUTPUT);
  pinMode(ERROR_LED_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT);

  buttonState = digitalRead(BUTTON_PIN);

  attachInterrupt(digitalPinToInterrupt(ECHO_PIN),
                  echoPinInterrupt,
                  CHANGE);

  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();
}

void loop() {
  unsigned long timeNow = millis();

  if (isLocked) {
    if (timeNow - lastTimeErrorLEDBlinked > errorLEDDelay) {
      lastTimeErrorLEDBlinked += errorLEDDelay;
      toggleErrorLED();
      toggleWarningLED();
    }

    if (timeNow - lastTimeButtonChanged > buttonDebounceDelay) {
      byte newButtonState = digitalRead(BUTTON_PIN);
      if (newButtonState != buttonState) {
        lastTimeButtonChanged = timeNow;
        buttonState = newButtonState;
        if (buttonState == LOW) { // released
          unlock();
        }
      }
    }
  }
  else {
    if (timeNow - lastTimeWarningLEDBlinked > warningLEDDelay) {
      lastTimeWarningLEDBlinked += warningLEDDelay;
      toggleWarningLED();
    }
  }

  if (IrReceiver.decode()) {
    IrReceiver.resume();
    long command = IrReceiver.decodedIRData.command;
    handleIRCommand(command);
    // Serial.println(command);
  }

  if (timeNow - lastTimeUltrasonicTrigger > ultrasonicTriggerDelay) {
    lastTimeUltrasonicTrigger += ultrasonicTriggerDelay;
    triggerUltrasonicSensor();
  }

  if (newDistanceAvailable) {
    newDistanceAvailable = false;
    double distance = getUltrasonicDistance();
    setWarningLEDBlinkRateFromDistance(distance);
    printDistanceOnLCD(distance);
    if (distance < LOCK_DISTANCE) {
      lock();
    }
    // Serial.println(distance);
  }
}
