#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class MRAC2{
	private:
		double theta[4] = {1.88,-1.74,-3.4,4.61};
		double 	w1 = 0;
		double 	w2 = 0;
		double 	ym = 0;
		double 	up = 0;
		int		duty = 0;
		double 	e1 = 0;
		double 	r  = 0; 
		double 	w0 = 0;
		double 	Km = 0;
		double 	gamma = 0;
		double 	lambda = 0;
		double e1Int = 0;
		int t = 0;
		double eulerProgression(double *StateMatrix,double *StateDerivative, int col,int mill,int id);
		double forLoopCalculation(double up,double *w, double *theta, double *dtheta);
		int sign(double value);
	public: 
		MRAC2(double omega0, double lambda, double gamma,  double Km );
		void	derivativeCalculation(double yp, double r,double current);
		void	printStates(double tmp,double current);
		double getU();
		int getDuty();

};