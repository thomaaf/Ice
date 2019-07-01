#include "PWMdriver.h"
#include "temp_IR.h"
class PWMdriver PWM(10,9,0,0);
class temp_IR tmp;
void setup() {
  PWM.begin();
  PWM.set_control(1,0);
  tmp.begin();
  Serial.begin(230400);
  pinMode(A0,INPUT);
  pinMode(A3,INPUT);
  analogReference(DEFAULT );
}

int t = 0;
float a = 1/1.5680;
float b = 38.9209/1.5680;
float x_est_prev = 0;
float p_prev = 0;
float Q = 10;
float R = 1;

float kalman(){
  int delta_t = millis()-t; t = millis();
  float trans = 1 + a*delta_t; 
  // Time update
  float x_est_ = trans*x_est_prev;
  float p_est_ = trans*p_prev*trans + Q;
  Serial.print("Kalman_p_est_,");
  Serial.print(p_est_);  
  // Measurment update
  float K = p_est_*1/(p_est_ + R);
  float x_est = x_est_ + K*(((float)(analogRead(A1)-518)/1023*100) - x_est_ );
  float p_est = (1-K)*p_est_;
  x_est_prev = x_est;
  p_prev = p_est;
  Serial.print(",Kalman_est,");
  Serial.print(x_est);
  Serial.print(",Kalman_cov,");
  Serial.print(p_est);    
  Serial.print(",Kalman_gain,");
  Serial.print(K);      
  return x_est;
  
  
  
  
  }

float getCurrentMeas(){
  int x = analogRead(A0);
  float pwm = (1.2244*pow(x,3))/1000000 - (1.4089*pow(x,2))/1000 + 0.70477*x -11.6520; 
  return pwm/255*1.2;
}

float getCurrentEst(int duty){
  return duty*9/(255*7.5);
}
float i = 0;
void loop() {
    i = round((round((sin((float)2*millis()/1000)+1)*255/2) + round((sin((float) millis()/6000)+1)*255/2))/2);
    i = round((round((sin((float) millis()/1000)+1)*255/2))/1);
    PWM.set_control(1,i);
    for (int j = 0; j <5; j++){
      Serial.print("duty, ");
      Serial.print(i);
      Serial.print(",Temp_ir,");
      Serial.print(tmp.get_OBJ1_tmp());
      Serial.print(",currentMeas,");
      Serial.print(getCurrentMeas()); 
      Serial.print(",currentEst,");
      Serial.print(getCurrentEst(i));
      Serial.print(",RTD,");
      Serial.print((float)(analogRead(A3)-518)/1023*100);
      Serial.print(",millis,");
      Serial.println(millis());        
    }} 
