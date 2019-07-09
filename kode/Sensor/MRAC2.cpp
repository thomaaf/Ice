 #include "MRAC2.h"

MRAC2::MRAC2(double omega0, double lambda, double gamma,  double Km ){  
	this -> w0 = omega0; 
	this -> lambda  = lambda;
	this -> gamma = gamma; 
	this -> Km = Km;
}

double MRAC2::getU(){
	return up;
}
int MRAC2::getDuty(){
	return duty;
}
void MRAC2::printStates(double tmp,double current){
	Serial.print("w1,");
	Serial.print(w1);
	Serial.print(",w2,");
	Serial.print(w2);
	Serial.print(",e1Int,");
	Serial.print(e1Int);	
	Serial.print(",ym,");
	Serial.print(ym);
	Serial.print(",yp,");
	Serial.print(tmp);	
	Serial.print(",up,");
	Serial.print(up);
	Serial.print(",current,");
	Serial.print(current);	
	Serial.print(",duty,");
	Serial.print(duty);
	Serial.print(",e1,");
	Serial.print(e1);
	Serial.print(",t,");
	Serial.print(t);
	Serial.print(",theta,[");
	for (int i = 0; i<3; i++){
		Serial.print(theta[i]);
		Serial.print(";");
	}
	Serial.print(theta[3]);	
	Serial.println("]");				
}


double MRAC2::eulerProgression(double *StateMatrix,double *StateDerivative, int col,int mill, int id){
	double dt = (double)(mill - t)/1000;
	/*
	//Serial.println(dt);
	Serial.print("ID: ");
	Serial.print(id);
	Serial.print(" dt: ");
	Serial.print(dt);
	Serial.print(" [ ");
	*/	
	for (int i = 0; i<col;i++){
		StateMatrix[i] = StateMatrix[i] + dt*StateDerivative[i];
		
		//Serial.print(StateMatrix[i]);
		//if (id == 1){Serial.print("("); Serial.print(StateDerivative[i]);Serial.print(")");}
		//Serial.print(" , ");
	}
	//Serial.println("]");
}
int MRAC2::sign(double value){
 	return value/abs(value); 
}
double MRAC2::forLoopCalculation(double up,double *w, double *theta, double *dtheta){
	double gamma2[4] = {0.008,0.008,0.005,0.005};
	double maxLim  [4] = {5.0,5.0,5.0,7.0};
	for (int i = 0; i<4; i++){
		if (abs(theta[i])>abs(maxLim[i]) ){theta[i] = sign(theta[i])*maxLim[i];}
		up = up + theta[i]*w[i];
		dtheta[i] = -gamma2[i]*e1*w[i];
	}
	//Serial.println(up);
	
	if (up>0){
		up = min(120,up);
	}
	else {
		up = max(-120,up);
	}
	
	return up;
}

void MRAC2::derivativeCalculation(double yp, double r,double current){
	this -> r = r;
	double w [4] = {w1,w2,yp,r};
	double dtheta[4] = {0,0,0,0};
	double kp = 2.0; double ki = 1;
	up = 0;
	
	e1  = (yp - ym);

	up = forLoopCalculation(up,w,theta,dtheta );
	//up = -kp*e1 - ki*e1Int;
	duty = (int) round(up/120*255);
	//dtheta = -gamma*e1*w';
	double dw1 = -lambda*w1 + current*100.0;
	double dw2 = -lambda*w2 + yp;
	double dym = -w0*ym + Km*r;

	int mill = millis();
	eulerProgression(&ym,&dym,1, mill,1);
	eulerProgression(&w1,&dw1,1, mill,2);
	eulerProgression(&w2,&dw2,1, mill,3);
	eulerProgression(theta,dtheta,4, mill,4);
	eulerProgression(&e1Int,&e1,4, mill,4);
	t = mill;
	Serial.println("");

}	
