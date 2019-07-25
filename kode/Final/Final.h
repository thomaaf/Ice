#include "PWMdriver.h"
#include "PIController.h"
#include "Sensor.h"
#include "Refcalc.h"
//#include "MRAC.h"
//double am = 1; double gamma =1; double lambda = 1; double r = 1;
//double Kp = 20.0; double Ki = 5.0; double AntiWind = 100.0;
double Kp = 1.0; double Ki = 2; double AntiWind = 100.0;
PWMdriver PWM(10,9,0,0);
Sensor sensor(8,A1,A5,0.8,A0,0.5); //Laserpin, ligtPin, lightFilter, tempPin, Tempfilter
RefCalc refCalc;
//MRAC mrac(am,gamma,lambda,r);
PIController PIC(Kp,Ki,AntiWind);//Kp,KI,Sat 