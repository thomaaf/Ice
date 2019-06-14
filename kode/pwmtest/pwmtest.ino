#include "PWMdriver.h"

class PWMdriver PWM(10,9,0,0);
void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("duty,");
  Serial.print(100);
  Serial.print(",temp_amb,");
  Serial.println(24);
  delay(200);
    
  
}
