
void setup() {
  pinMode(A1,INPUT);
  Serial.begin(38400);
}

void loop() {
  Serial.println(analogRead(A1));
  delay(100);
}
