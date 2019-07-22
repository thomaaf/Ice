#include "PIController.h"
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


PIController::PIController(double Kp,double Ki,double intSat){
	this-> Kp 	  = Kp;
	this-> Ki 	  = Ki;
	this-> intSat = intSat;  
}
void PIController::inputCalculation(double ref, double meas,int t,int state){
	if (state == 0){dir = 0; u = 0; return;}
	dt = (double)(t - this -> t)/1000; 
	error = ref - meas; 
	integral = integral + dt*error*3.0;
	//Serial.println(integral*Ki);
	u = (int) (Kp*error + Ki*integral);
	if (integral>0){ integral = min(intSat/Ki,integral);}
  	else {integral = max (-intSat/Ki,integral);}
  	if (u>0){dir = 1; u = min(255,u);}
  	else{dir =  -1; u = -max(-255,u);}
  	this -> t = t;   
}


double PIController::getInput(){
	return u;
}

int PIController::getDir(){
	return dir;
}

int PIController::getDt(){
	return dt*1000;
}
double PIController::getIntegral(){
	return integral;
}

int PIController::getT(){
	return t;
}
double PIController::getError(){
	return error;
}