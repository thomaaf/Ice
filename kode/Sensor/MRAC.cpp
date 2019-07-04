 #include "MRAC.h"

MRAC::MRAC(double zeta, double omega0, int lambda, double ym0, int gamma ){  
	this -> omega0 = omega0; 
	this -> zeta    = zeta; 
	this -> lambda  = lambda;
	this -> gamma = gamma; 
	F		= -lambda;
	Wm[2] =  -pow(omega0,2);
	Wm[3] = -2*zeta*omega0;
	B[1]  = pow(omega0,2);
 	ym[1] = ym0;  
}

double MRAC::getU(){
	return u;
}
void MRAC::printStates(double tmp,int duty){
	Serial.print("w1,");
	Serial.print(w1);
	Serial.print(",tmp,");
	Serial.print(tmp);
	Serial.print(",phi1,");
	Serial.print(phi1[0]);
	Serial.print(",phi2,");
	Serial.print(phi2[0]);
	Serial.print(",phi3,");
	Serial.print(phi3[0]);			
	Serial.print(",duty,");
	Serial.print(duty);	
	Serial.print(",w2,");
	Serial.print(w2);
	Serial.print(",ym,");
	Serial.print(ym[0]);
	Serial.print(",z,");
	Serial.print(z[0]);
	Serial.print(",u,");
	Serial.print(u);	
	Serial.print(",theta,[");
	for (int i = 0; i<3; i++){
		Serial.print(theta[i]);
		Serial.print(";");
	}
	Serial.print(theta[3]);	
	Serial.println("]");
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
		//if (id == 1){Serial.print("("); Serial.print(StateDerivative[i]);Serial.print(")");}
		//Serial.print(" , ");
	}
	//Serial.println("]");

}


void MRAC::derivativeCalculation(double y, double r){
	double phi[4]  = {phi1[0],phi2[0],phi3[0],y};
	double phiP[4] = {w1, w2, y,r};
	double zhat = 0;
	double m = 0;

	double dym[2] = {0,0};
	double dz[2] = {0,0};
	double dphi1[2] = {0,0};
	double dphi2[2] = {0,0};
	double dphi3[2] = {0,0};
	double dtheta[4] = {0,0,0,0};
	double dw1 = 0;
	double dw2 = 0;

	for (int i = 0; i<4; i++){
		zhat = zhat + phi[i]*theta[i];
		m = m + phi[i]*phi[i];
		u =  theta[i]*phiP[i];
	}
	m = m + 1;
	double e = (z[0] - zhat)/m;
	Serial.println(zhat);
	for (int i = 0; i<2; i++){
		dw1   	 = F*w1 + g*u;
		dw2   	 = F*w2 + g*y;		

		dym[i]	 = Wm[0+2*i]*ym[0]   + Wm[2*i+1]*ym[1] + B[i]*r;
		dz[i]	 = Wm[0+2*i]*z[0]   + Wm[2*i+1]*z[1]   + B[i]*u;
		dphi1[i] = Wm[0+2*i]*phi1[0] + Wm[2*i+1]*phi1[1] + B[i]*w1;
		dphi2[i] = Wm[0+2*i]*phi2[0] + Wm[2*i+1]*phi2[1] + B[i]*w2;
		dphi3[i] = Wm[0+2*i]*phi3[0] + Wm[2*i+1]*phi3[1] + B[i]*y;
		dtheta[i]   = gamma*e*phi[i];
		dtheta[2+i] = gamma*e*phi[2+i];		
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
