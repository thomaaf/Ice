 #include "MRAC.h"

void 	MRAC::epsilonCalc(){
	double m = 1.0;
	double zhat = 0.0;
	for (int i =0 ;i<4; i++){
		m = m + phip[i]*phip[i];
		zhat = zhat + theta[i]*phip[i];
	}
	double epsilonUnormalized = z - zhat;
	epsilon = epsilonUnormalized/m;

}

void MRAC::dthetaCalc(double *dtheta){
	
	for(int i = 0; i<4; i++){
		dtheta[i] = gamma*epsilon*phip[i];
	}

}

void MRAC::dphipCalc(double *dphip){

	
    dphip[0]  = -am*phip[0]   + km*w[0];
    dphip[1]  = -am*phip[1]   + km*w[1];
    dphip[2]  = -am*phip[2]   + km*yp;

}

void MRAC::dwCalc(double *dw){
	dw[0] = -lambda*w[0] + upIn;
	dw[1] = -lambda*w[1] + yp;
}

double MRAC::dzCalc(){
	double dz = -am*z + km*upIn;
	return dz;
}
double MRAC::dymCalc(){
	double dym = -am*ym + km*r;
	return dym;
}

void MRAC::dypSimCalc(double *dypSim){
    double a1 = 1.0; double a0 = 1.0;
    dypSim[0] = ypSim[1];
    dypSim[1] = up - a1*ypSim[1] - a0*ypSim[0];
}

double MRAC::getU(){
		return up;
}

void MRAC::mainCalculations(double tmp,double up_in){
	r = -10.00;
	//r = (sin((double) millis()/2500)*15-15);

	//yp = ypSim[0];
	yp = tmp;
	upIn = up; 
	double dtheta[4] = {0.0,0.0,0.0,0.0};
	double dphip [4] = {0.0,0.0,0.0,0.0};
	double dw [2]    = {0.0,0.0};
	double dz = 0;
	double dym =0 ;
	up = theta[0]*w[0] + theta[1]*w[1] + theta[2]*yp + theta[3]*r;
	//up =sin((double) t/500)*5;
	epsilonCalc();

	dthetaCalc(dtheta);

	dphipCalc(dphip);	

	dwCalc(dw);

	dz = dzCalc();

	dym = dymCalc();

	double dypSim[2] = {0.0,0.0}; dypSimCalc(dypSim);
	
	double dt = (millis()-t)/1000;

	theta[0] = theta[0] + dt*dtheta[0]; theta[1] = theta[1] + dt*dtheta[1];
	theta[2] = theta[2] + dt*dtheta[2]; theta[3] = theta[3] + dt*dtheta[3];

	phip[0] = phip[0] + dt*dphip[0]; phip[1] = phip[1] + dt*dphip[1];
	phip[2] = phip[2] + dt*dphip[2]; phip[3] = yp;
	w[0] = w[0] + dt*dw[0]; w[1] = w[1] + dt*dw[1];
	ypSim[0] = ypSim[0] + dt*dypSim[0];
	ypSim[1] = ypSim[1] + dt*dypSim[1];
	z = z + dt*dz;
	ym = ym + dt*dym;
	t = millis();
}

MRAC::MRAC(double am, double gamma, double lambda,double r ){  
	this -> am = am; 
	this -> gamma  = gamma;
	this -> lambda = lambda; 
	this -> r = r;
}

void MRAC::printStates(){
	Serial.print("z,"); Serial.print(z);
	Serial.print(",epsilon,"); Serial.print(epsilon);
	Serial.print(",yp,"); Serial.print(yp);
	Serial.print(",t,"); Serial.print(t);
	Serial.print(",ym,"); Serial.print(ym);
	Serial.print(",up,"); Serial.print(up);
	Serial.print(",upIn,"); Serial.print(upIn);	
	Serial.print(",r,"); Serial.print(r);
	Serial.print(",theta,[");
	for(int i = 0; i<4; i++){
		Serial.print(theta[i]);
		if (i == 3){Serial.print("]");break;}
		Serial.print(";");
	}
	Serial.print(",phip,[");
	for(int i = 0; i<4; i++){
		Serial.print(phip[i]);
		if (i == 3){Serial.print("]");break;}
		Serial.print(";");
	}	
	Serial.print(",w,[");
	for(int i = 0; i<2; i++){
		Serial.print(w[i]);
		if (i == 1){Serial.print("]");break;}
		Serial.print(";");
	}	
	Serial.print(",ypSim,[");
	for(int i = 0; i<2; i++){
		Serial.print(ypSim[i]);
		if (i == 1){Serial.println("]");break;}
		Serial.print(";");
	}	
}