#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MRAC{
	private:
		double theta[4]	= {0.0,0.0,0.0,1.0};
		double w1 		= 0.0;
		double w2 		= 0.0;
		double ym   [2] = {0,0};
		double up		= 0;
		int    duty		= 0;
		double e 		= 0; 
		double r 		= 0;
		double w0		 = 0;
		double Km 		= 0;
		double gamma 	= 0;
		double lambda 	= 0;
		int    t 			= 0;		
		double zeta 	= 0;
		double Wm   [4]	= {0,1, -pow(w0,2),-2*zeta*w0};
		double B    [2]	= {0,pow(w0,2)};
		double z    [2] = {0,0};
		double phi1 [2] = {0,0};
		double phi2 [2] = {0,0};
		double phi3 [2] = {0,0};
		double Q [4]  = {0,0,0,0};
		double R [16] = {0,0,0,0, 0,0,0,0, 0,0,0,0, 0,0,0,0};

		double eulerProgression(double *StateMatrix,double *StateDerivative, int col,int mill,int id);
	public: 
		MRAC(double zeta, double omega0, double lambda,double gamma,double Km);
		void	derivativeCalculation(double yp, double r,double current);
		void 	derivativeCalculationIntegral(double yp, double r, double current);
		void	printStates(double tmp,double current);
		int 	getDuty();
		double 	getU();
};