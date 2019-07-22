#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MRAC{
private:
	double am = 0; double gamma = 0.0; double lambda = 0.0;
	double theta [4] = {2.2600,0.9500,0.5600 ,-1.0800}; //{5.7300,1.0800,1.4700,-1.9100};
	//double theta [4] = {5.7300,1.0800,1.4700,-1.9100}; 
	double z = 0.0;
	double epsilon = 0;
	double w [2] = {0.0,0.0};
	double yp = 0.0;
	double ypSim[2] = {0.0,0.0};
	double ym = 0.0;
	double phip[4] ={0.0,0.0,0.0,0.0};
	double up = 0.0;
	double r = 0.0;
	double upIn = 0.0;
	double km = 1.0;
	void 	epsilonCalc();
	void 	dthetaCalc(double *dtheta);
	void 	dphipCalc(double *dphip);
	void 	dwCalc(double *dw);
	double 	dzCalc();
	double 	dymCalc();
	void dypSimCalc(double *dypSim);

	double t = 0;
public:
	void mainCalculations(double tmp, double up_in);
	MRAC(double am, double gamma, double lambda,double r);
	void printStates();		
	double getU();


};