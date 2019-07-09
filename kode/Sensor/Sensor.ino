#include "MRAC2.h"
#include "MRAC.h"
#include "PWMdriver.h"
#include "temp_IR.h"
double omega0 = 1.0;
double lambda = 3.0;
double gamma = 1.0;
double Km = 1.0;
double zeta = 1.0;
class MRAC2 mrac2(omega0, lambda, gamma, Km);
class MRAC mrac(zeta, omega0, lambda, gamma, Km);
class PWMdriver PWM(10,9,0,0);
class temp_IR tmp;
double r = 2;
double y = 2;
void setup() {
  PWM.begin();
  PWM.set_control(1,100);
  tmp.begin();  
  pinMode(A0,INPUT);
  pinMode(A3,INPUT);  
  Serial.begin(57600);
  Serial.println("");
}
//S,2,1)
int dir;
double getCurrentMeas(){
  int x = analogRead(A0);
  double pwm = (1.2244*pow(x,3))/1000000 - (1.4089*pow(x,2))/1000 + 0.70477*x -11.6520; 
  return pwm/255*1.2;
}
double alfa = 0.1;
double tmpRTD = 0;
double ref = 0;


void loop() {  
  double meas = ((double)(analogRead(A3)-518)/1023*100)-22.4; 
  tmpRTD = tmpRTD + alfa*(meas - tmpRTD);
  //if (millis()>20000){mrac2.derivativeCalculation(tmp,-20);}
  //else {mrac2.derivativeCalculation(tmp,-10);}
  ref = 4*sin((double)millis()/1000) + 2.3*sin((double)millis()/1500) ;
  //ref = -20; 
  mrac.derivativeCalculationIntegral(tmpRTD,ref,getCurrentMeas()*dir*-1);
  mrac.printStates(tmpRTD, getCurrentMeas()*dir*-1);
  int duty = mrac.getDuty();
  
  if (duty<0){dir =  1; duty = duty*-1;}
  else{dir = -1;}

  PWM.set_control(dir,duty );

}
