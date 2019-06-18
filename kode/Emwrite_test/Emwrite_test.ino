#include"temp_IR.h"

class temp_IR tmp;


void setup() {
  tmp.begin();
  Serial.begin(9600);
}

void loop() {

  Serial.print("temp_amb,");
  Serial.print(tmp.get_AMB_tmp());
  Serial.print(",temp_obj,");
  Serial.println(tmp.get_OBJTOT_tmp());
      //tmp.set_emissivity(73);

  delay(1000);
}
