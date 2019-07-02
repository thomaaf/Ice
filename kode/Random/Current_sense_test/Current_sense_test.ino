void setup() {
  Serial.begin(19200);
  pinMode(A1,INPUT);

}

void loop() {
  Serial.print("test,");
  Serial.println(analogRead(A1));
  // put your main code here, to run repeatedly:

}
