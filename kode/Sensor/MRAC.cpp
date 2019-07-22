 #include "MRAC.h"

void MRAC::dthetaCalc(double epsilon,double *phip,double *dtheta){
	for(int i = 0; i<3; i++){
		dtheta[i] = gamma*epsilon*phip[i];
	}
}