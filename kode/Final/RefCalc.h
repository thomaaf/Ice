#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class RefCalc{
private:
	
	int heating = 0;
	double wetTemperature = 17.0;
	double dryTemperature = 30.0;
	double treshold = 0.07;
	double reference = wetTemperature;
	int resetZeroMean = 0;
public:
	void refCalculation(double temperatureHeavyFiltered,double pValue,double mean);
	double getReference();
	int getHeating();
	int getResetZeroMean();
	void setResetZeroMean();
};
