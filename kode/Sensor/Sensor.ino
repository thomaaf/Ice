#include "MRAC.h"
#include "PWMdriver.h"
#include "temp_IR.h"
class MRAC mrac(1,3,2,0,10);
class PWMdriver PWM(10,9,0,0);
class temp_IR tmp;
double r = 2;
double y = 2;
void setup() {
  PWM.begin();
  PWM.set_control(1,0);
  tmp.begin();  
  pinMode(A0,INPUT);
  pinMode(A3,INPUT);  
  Serial.begin(38400);
}

void loop() {	
	double tmp = ((double)(analogRead(A3)-518)/1023*100)-25;
	mrac.derivativeCalculation(tmp,-2);

	int duty = round(mrac.getU()/2*255);
	if (duty<0){duty = -max(duty,-255);}
	else {duty= min(duty,255);}
	mrac.printStates(tmp,duty);
	PWM.set_control(1,duty );
}
