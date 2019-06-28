void setup() {
  Serial.begin(9600);
  pinMode(A0,INPUT);

}

void loop() {
  Serial.println(analogRead(A0));
  delay(200);
  // put your main code here, to run repeatedly:

}
