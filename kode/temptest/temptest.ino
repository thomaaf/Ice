#include "temp.h"

class tempsensor tmp;
void setup() {
  tmp.begin();
  Serial.begin(9600);
}

void loop() {
  Serial.print("duty,");
  Serial.print(0);
  Serial.print(",temp_amb,");
  Serial.print(tmp.get_AMB_tmp());
  Serial.print(",temp_obj,");
  Serial.println(tmp.get_OBJTOT_tmp());
  delay(1000);
}
