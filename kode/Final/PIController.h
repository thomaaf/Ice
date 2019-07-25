class PIController{
private:
	double integral = 0;
	double intSat = 0;
	double derivative = 0;
	double Kp = 0;
	double Ki = 0;
	double u  = 0;
	double error = 0;
	int dir = 0;
	int t = 0;
	double dt = 0;
	int rollover = 0;	
	double lp = 0;

public:
	PIController(double Kp,double Ki,double intSat);
	void  inputCalculation(double ref, double meas, int t,int state);
	double getInput();
	int getDir();
	int getDt();
	double getIntegral();
	int getT();
	double getError();
	void resetIntegral();
	double getDerivative();
};