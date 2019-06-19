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
	Serial.print((analogRead(A0)-511)/10.23);
  Serial.print(",IR_amb,");
  Serial.print(tmp.get_AMB_tmp()); 
  Serial.print(",IR_OBJ1,");
  Serial.print(tmp.get_OBJ1_tmp()); 
  Serial.print(",IR_OBJ2,");
  Serial.print(tmp.get_OBJ2_tmp());     
  Serial.print(",IR_OBJTOT,");
  Serial.println(tmp.get_OBJTOT_tmp());   
  
	
}
