 #include "MRAC.h"

MRAC::MRAC(double zeta, double omega0, double lambda, double gamma,double Km){  
	this -> w0 = omega0; 
	this -> zeta    = zeta; 
	this -> lambda  = lambda;
	this -> gamma = gamma; 
	Wm[2] =  -pow(omega0,2);
	Wm[3] = -2*zeta*omega0;
	B[1]  = pow(omega0,2);
}

double MRAC::getU(){
	return up;
}

int MRAC::getDuty(){
	return duty;
}
void MRAC::printStates(double tmp,double current){
	Serial.print("w1,");
	Serial.print(w1);
	Serial.print(",w2,");
	Serial.print(w2);	
	Serial.print(",ym,");
	Serial.print(ym[0]);
	Serial.print(",yp,");
	Serial.print(tmp);	
	Serial.print(",z,");
	Serial.print(z[0]);	
	Serial.print(",phi1,");
	Serial.print(phi1[0]);	
	Serial.print(",phi2,");
	Serial.print(phi2[0]);	
	Serial.print(",phi3,");
	Serial.print(phi3[0]);					
	Serial.print(",up,");
	Serial.print(up);
	Serial.print(",current,");
	Serial.print(current);	
	Serial.print(",duty,");
	Serial.print(duty);
	Serial.print(",e,");
	Serial.print(e);
	Serial.print(",t,");
	Serial.print(t);
	Serial.print(",theta,[");
	for (int i = 0; i<3; i++){
		Serial.print(theta[i]);
		Serial.print(";");
	}
	Serial.print(theta[3]);	
	Serial.print("]");			
	Serial.print(",Q,[");
	for (int i = 0; i<4; i++){
		Serial.print(Q[i]);
		if (i == 3){Serial.print("]");break;}
		Serial.print(";");
	}
	Serial.print(",R,[");
	for(int i = 0; i<4; i ++ ){
		for(int j = 0; j<4; j++){
			Serial.print(R[i*4 + j]);
			if((j == 3)&&(i == 3)){
				Serial.println("]");
			}else{Serial.print(";");}			
		}
	}


}


double MRAC::eulerProgression(double *StateMatrix,double *StateDerivative, int col,int mill, int id){
	double dt = (double)(mill - t)/1000;
	/*
	Serial.print("ID: ");
	Serial.print(id);
	Serial.print(" dt: ");
	Serial.print(dt);
	Serial.print(" [ ");
	*/
	for (int i = 0; i<col;i++){
		StateMatrix[i] = StateMatrix[i] + dt*StateDerivative[i];
		
		//Serial.print(StateMatrix[i]);
		//if (id == 8){Serial.print("("); Serial.print(StateDerivative[i]);Serial.print(")");}
		//Serial.print(" , ");
	}
	//Serial.println("]");

}


void MRAC::derivativeCalculation(double yp, double r, double current){
	double gamma2	[4] = {10  ,10  ,2  ,5};
	double phiP 	[4] = {phi1[0],phi2[0],phi3[0],yp};
	double w    	[4] = {w1     , w2    , yp    ,r};
	double zhat 	    = 0;
	double m    	    = 0;
	double dym  	[2] = {0,0};
	double dz   	[2] = {0,0};
	double dphi1	[2] = {0,0};
	double dphi2	[2] = {0,0};
	double dphi3	[2] = {0,0};
	double dtheta	[4] = {0,0,0,0};
	double dw1 			= 0;
	double dw2 			= 0;
	up = 0;

	//Calculation of Zhat,m, e, and Up
	for (int i = 0; i<4; i++){
		zhat = zhat + phiP[i]*theta[i];
		m = m + phiP[i]*phiP[i];
		up = up + theta[i]*w[i];
	}
	m = m + 1;
	e = (z[0] - zhat)/m;
	duty = (int) round(up/1.2*255);
	//derivative calculations

	dw1   	 = -lambda*w1 + current;
	dw2   	 = -lambda*w2 + yp;		
	for (int i = 0; i<2; i++){
		dym[i]	 	= Wm[2*i ]*ym 	[0] + Wm[2*i + 1]*ym  [1] + B[i]*r;
		dz[i]	 	= Wm[2*i ]*z 	[0] + Wm[2*i + 1]*z   [1] + B[i]*up;
		dphi1[i] 	= Wm[2*i ]*phi1	[0] + Wm[2*i + 1]*phi1[1] + B[i]*w1;
		dphi2[i] 	= Wm[2*i ]*phi2	[0] + Wm[2*i + 1]*phi2[1] + B[i]*w2;
		dphi3[i] 	= Wm[2*i ]*phi3	[0] + Wm[2*i + 1]*phi3[1] + B[i]*yp;
		dtheta[i]   = gamma2[i]  *e*phiP[i];
		dtheta[2+i] = gamma2[2+i]*e*phiP[2+i];		
	}
	//Serial.println("");
	int mill = millis();
	eulerProgression(ym,dym,2, mill,1);
	eulerProgression(&w1,&dw1,1, mill,2);
	eulerProgression(&w2,&dw2,1, mill,3);
	eulerProgression(z,dz,2, mill,4);
	eulerProgression(phi1,dphi1,2, mill,5);
	eulerProgression(phi2,dphi2,2, mill,6);
	eulerProgression(phi3,dphi3,2, mill,7);
	eulerProgression(theta,dtheta,4, mill,8);
	t = mill;

}	

void MRAC::derivativeCalculationIntegral(double yp, double r, double current){
	double gamma2	[4] = {10  ,10  ,2  ,5};
	double phiP 	[4] = {phi1[0],phi2[0],phi3[0],yp};
	double w    	[4] = {w1     , w2    , yp    ,r};
	double zhat 	    = 0;
	double m    	    = 0;
	double dym  	[2] = {0,0};
	double dz   	[2] = {0,0};
	double dphi1	[2] = {0,0};
	double dphi2	[2] = {0,0};
	double dphi3	[2] = {0,0};
	double dtheta	[4] = {0,0,0,0};
	double dw1 			= 0;
	double dw2 			= 0;
	up = 0;

	//Calculation of Zhat,m, e, and Up
	for (int i = 0; i<4; i++){
		zhat = zhat + phiP[i]*theta[i];
		m = m + phiP[i]*phiP[i];
		up = up + theta[i]*w[i];
	}
	m = m + 1;
	e = (z[0] - zhat);
	if (up>0){
		up = min(12,up);
	}
	else {
		up = max(-12,up);
	}	
	duty = (int) round(up/1.2*25.5);
	//derivative calculations

	dw1   	 = -lambda*w1 + current*10;
	dw2   	 = -lambda*w2 + yp;		
	for (int i = 0; i<2; i++){
		dym[i]	 	= Wm[2*i ]*ym 	[0] + Wm[2*i + 1]*ym  [1] + B[i]*r;
		dz[i]	 	= Wm[2*i ]*z 	[0] + Wm[2*i + 1]*z   [1] + B[i]*up;
		dphi1[i] 	= Wm[2*i ]*phi1	[0] + Wm[2*i + 1]*phi1[1] + B[i]*w1;
		dphi2[i] 	= Wm[2*i ]*phi2	[0] + Wm[2*i + 1]*phi2[1] + B[i]*w2;
		dphi3[i] 	= Wm[2*i ]*phi3	[0] + Wm[2*i + 1]*phi3[1] + B[i]*yp;

	}
	double dR [16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};
	double dQ [4]  = {0,0,0,0};
	double beta = 0.5;
	for (int i = 0; i<4; i++){
		double var = 0;
		dQ[i] 		= -beta*Q[i] - phiP[i]*z[0];
		for (int j = 0; j<4; j++){
			//Serial.println(phiP[i]*phiP[j]);
/*			if ((i == 0)&&(j==0)){
				Serial.print(phiP[i]);
				Serial.print(" : ");
				Serial.println(phiP[i]*phiP[j]);
				Serial.print(" : ");
				Serial.println(m);
			}*/
			dR[i*4 + j] = -beta*R[i*4 + j] + phiP[i]*phiP[j];
			var = var + R[j];
		}
		dtheta[i] = -gamma*var*theta[i] + Q[i];
	}
	//Serial.println("");
	int mill = millis();
	eulerProgression(ym,dym      ,2, mill,1);
	eulerProgression(&w1,&dw1    ,1, mill,2);
	eulerProgression(&w2,&dw2    ,1, mill,3);
	eulerProgression(z,dz        ,2, mill,4);
	eulerProgression(phi1,dphi1  ,2, mill,5);
	eulerProgression(phi2,dphi2  ,2, mill,6);
	eulerProgression(phi3,dphi3  ,2, mill,7);
	eulerProgression(R,dR        ,16, mill,8);
	eulerProgression(Q,dQ        ,4, mill,9);		
	eulerProgression(theta,dtheta,4, mill,10);
	t = mill;

}	
