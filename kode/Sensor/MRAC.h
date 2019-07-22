#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MRAC{
private:
	double gamma = 0.0;
	double theta [4] = {0.0,0.0,0.0,1.0};

	double 	epsilonCalc(double *phip,double theta,double z);
	
	void 	dthetaCalc(double epsilon,double *phip,double *dtheta);
	void	dphipCalc(double w1,double w2,double yp,double *phip);

public:
