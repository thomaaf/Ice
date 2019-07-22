#include"temp_IR.h"
#include "PWMdriver.h"
class temp_IR tmp;
class PWMdriver PWM(10,9,0,0);

void setup() {

  Serial.begin(38400);

  delay(1000);
  PWM.begin();
  PWM.set_control(1,200 );  
  tmp.begin();
  pinMode(A3,INPUT);    
  //delay(1000);
  //tmp.set_emissivity(73);
  //delay(3000);
  Serial.print(tmp.get_emissivity(0),HEX);  
  Serial.print(",em2,");
  Serial.println(tmp.get_emissivity(2),HEX);    
  
}
double alfa = 1;
double tmpRTD  = 0;
void loop() {
  
  double meas = ((double)(analogRead(A3)-504)/1023*100); 
  //double meas = ((double)(analogRead(A3))); 
  tmpRTD = tmpRTD + alfa*(meas - tmpRTD);  
  Serial.print("RTD,");
  Serial.print(meas);
  Serial.print(",temp_obj,");
  Serial.print(tmp.get_OBJTOT_tmp());
  Serial.print(",Ambient, ");
  Serial.print(tmp.get_AMB_tmp());
  Serial.print(",obj1, ");
  Serial.print(tmp.get_OBJ1_tmp());
  Serial.print(",obj2, ");
  Serial.print(tmp.get_OBJ2_tmp());    
  Serial.print(",em,");
  Serial.print(tmp.get_emissivity(0),HEX);  
  Serial.print(",em2,");
  Serial.println(tmp.get_emissivity(2),HEX);    
  delay(50);
  
}
