#include "MRAC.h"
#include"PWMdriver.h"
double am = 1; double gamma =1; double lambda = 1; double r = 1;

double tmpRTD = 0;
class MRAC mrac(am,gamma,lambda,r);
class PWMdriver PWM(10,9,0,0);
void setup() {
	Serial.begin(37800); 	
	PWM.begin();
  pinMode(A0,INPUT);
	pinMode(A3,INPUT); 	
	pinMode(A2,INPUT);
  PWM.set_control(-1,255 );
  tmpRTD = ((double)(analogRead(A3)-518)/1023*100 - 23); 
}


double alfa2 = 0.4;

double alfa= 0.9;

double up_in = 0;
//double up = 0;
int dir = 0;
double light = 0;

double t = 0;
double derivative = 0;
double integral = 0;
double prevError = 0;
double ref = -10;
double Kp = 10; double Kd = 0; double Ki = 0.08;
int u = 0;
void loop() {
  /*
  //PWM.set_control(-1,255 );
  //light = light + alfa2*(analogRead(A2) - light);
  double meas = ((double)(analogRead(A3)-518)/1023*100 - 23); 
  tmpRTD = tmpRTD + alfa*(meas - tmpRTD);
  //Serial.println(tmpRTD);



  //if (millis()>20000){mrac2.derivativeCalculation(tmp,-20);}
  //else {mrac2.derivativeCalculation(tmp,-10);}
  //ref = 4*sin((double)millis()/1000) + 2.3*sin((double)millis()/1500) ;
	ref = -5; 	
	up_in = getCurrentMeas();
	mrac.printStates();
	double uph = mrac.getU();
  mrac.mainCalculations(tmpRTD,0 );

  	if (uph>0){dir = 1; uph = min(255,uph*2);}
  	else{dir = -1; uph = -max(-255,uph*2);}
  	PWM.set_control(dir,uph );
  */
  
  //ref = (sin((double) millis()/2500)*5 -5);
  
  ref = -30;
  double dt = (millis() - t)/1000;  
  double meas = ((double)(analogRead(A3) - 511)/1023*100 - 24.24); 
  if(abs(meas-tmpRTD)<4 ) {  tmpRTD = tmpRTD + alfa*(meas - tmpRTD);};

  //tmpRTD = meas;
  double error = ref - tmpRTD;
  derivative = (error - prevError)/dt;
  integral = integral + dt*error*1.0;
  if (integral>0){ integral = min(250.0/Ki,integral);}
  else {integral = max (-250.0/Ki,integral);}
  double alfa3 = 1;
  //u = u + round(alfa3*(round(Kp*error + Ki*integral + Kd*derivative) - u));
  prevError = error;
  u = (int)(Kp*error + Ki*integral + Kd*derivative);
  //if(u>0){ u = min(u,255);} else{u = max(u,-255);}

  if (u>0){dir = 1; u = min(255,u);}
  else{dir =  -1; u = -max(-255,u);}  
  PWM.set_control(dir,u );
  Serial.print("u,");
  Serial.print(u);
  Serial.print(",integral,");
  Serial.print(integral*Ki);
  Serial.print(",derivative,");
  Serial.print(derivative*Kd);
  Serial.print(",error,");
  Serial.print(error);
  Serial.print(",ref,");
  Serial.print(ref);
  Serial.print(",tmp,");
  Serial.println(tmpRTD);  
  delay(5);


 //PWM.set_control( 1,255); 
 //double meas = ((double)(analogRead(A3) - 511)/1023*100 - 24.24);
 //Serial.println(meas);
 /*
  for(int i = 0; i<255; i+=10){
    double meas = ((double)(analogRead(A3) - 511)/1023*100 - 24.24);
    PWM.set_control(-1,i);   
    Serial.print(i);
    Serial.print(" : ");
    Serial.println(meas);
    delay(200);
  

  }
  */
  
}


double getCurrentMeas(){
  int x = analogRead(A0);
  double pwm = (1.2244*pow(x,3))/1000000 - (1.4089*pow(x,2))/1000 + 0.70477*x -11.6520; 
  return pwm/255*1.2 + 0.04;
}