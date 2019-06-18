#include "PWMdriver.h"
#include"temp_IR.h"
class PWMdriver PWM(10,9,0,0);
class temp_IR tmp;

void setup() {
	pinMode(A0, INPUT);
  PWM.begin();
  tmp.begin();
	Serial.begin(38400);
  PWM.set_control(1,255);
  Serial.println(analogRead(A0));

}

void loop() {
	Serial.print("RTD,");
	Serial.print(analogRead(A0));
  Serial.print(",temp_amb,");
  Serial.print(tmp.get_AMB_tmp());
  Serial.print(",temp_obj,");
  Serial.println(tmp.get_OBJTOT_tmp());
  
	
}
