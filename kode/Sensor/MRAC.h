#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MRAC{
	private: 
		int gamma = 0;
		int t = 0;
		double omega0 = 0;
		double zeta = 0;
		int lambda = 0;
		int F = -lambda;
		int g = 1;
		// Matricies		
		double Wm [4] = {0,1, -pow(omega0,2),-2*zeta*omega0};
		double B  [2] = {0,pow(omega0,2)};
		// States
		double w1 = 0;
		double w2 = 0;
		double ym   [2] = {0,0};
		double z    [2] = {0,0};
		double phi1 [2] = {0,0};
		double phi2 [2] = {0,0};
		double phi3 [2] = {0,0};
		double theta[4] = {0,0,0,1};
		double u = 0;
		double eulerProgression(double *StateMatrix,double *StateDerivative, int col,int mill,int id);
	public: 
		MRAC(double zeta, double omega0, int lambda, double ym0, int gamma );
		void	derivativeCalculation(double y, double r);
		void	printStates(double tmp,int duty);
		double getU();
};