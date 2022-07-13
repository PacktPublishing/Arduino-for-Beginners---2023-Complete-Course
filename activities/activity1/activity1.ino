void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
}

void loop() {
  Serial.println("LED on");
  digitalWrite(13, HIGH);
  delay(2000);
  Serial.println("LED off");
  digitalWrite(13, LOW);
  delay(1000);
}
