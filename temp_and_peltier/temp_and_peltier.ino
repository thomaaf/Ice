#include"temp.h"
#include "PWMdriver.h"

class PWMdriver PWM(10,9,0,0);
class tempsensor tmp;

void setup() {
  tmp.begin();
  PWM.begin();
  Serial.begin(9600);
  PWM.set_control(1,200);
}

void loop() {
  Serial.print("duty,");
  Serial.print(0);
  Serial.print(",temp_amb,");
  Serial.print(tmp.get_AMB_tmp());
  Serial.print(",temp_obj,");
  Serial.println(tmp.get_OBJTOT_tmp());
  delay(100);
}
