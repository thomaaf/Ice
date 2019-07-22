#include "MRAC.h"
#include "PWMdriver.h"
#include "temp_IR.h"

class PWMdriver PWM(10,9,0,0);
//class temp_IR tmp;

void setup() {

}

double getCurrentMeas(){
  int x = analogRead(A0);
  double pwm = (1.2244*pow(x,3))/1000000 - (1.4089*pow(x,2))/1000 + 0.70477*x -11.6520; 
  return pwm/255*1.2;
}



void loop() {  

}
