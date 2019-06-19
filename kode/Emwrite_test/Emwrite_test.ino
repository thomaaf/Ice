#include"temp_IR.h"

class temp_IR tmp;


void setup() {
  tmp.begin();
  Serial.begin(38400);
  delay(1000);
  Serial.print(",temp_obj,");
  Serial.print(tmp.get_OBJTOT_tmp());
  Serial.print(",em,");
  Serial.print(tmp.get_emissivity(0),HEX);  
  Serial.print(",em2,");
  Serial.println(tmp.get_emissivity(2),HEX);    
  delay(1000);
  tmp.set_emissivity(73);
  delay(3000);
  Serial.print(tmp.get_emissivity(0),HEX);  
  Serial.print(",em2,");
  Serial.println(tmp.get_emissivity(2),HEX);    
  
}

void loop() {

}
