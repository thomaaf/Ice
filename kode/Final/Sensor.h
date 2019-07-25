#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Sensor{
private:
	int laserControlPin = 0;

	int 	lightSensorReadPin = 0;
	double 	lightFilterValue = 0.0;
	double 	lightValue = 0.0;
	
	int ambientTemperatureReadPin = 0; 
	int temperatureSensorReadPin = 0;
	
	double ambientTemperature = 0;
	double temperatureHeavyFiltered = 20.0;
	double 	temperatureHeavyFilterValue = 0.001;

	double 	temperature = 20.0;
	double 	temperatureFilterValue = 0.0;


 
	double zeroMean = 0;
	double zeroVariance = 0;
	
	double mean = 0;
	double variance = 0;
	double pValue = 0;

public:
	Sensor(int laserControlPin, int lightSensorReadPin,int ambientTemperatureReadPin, double lightFilterValue,int temperatureSensorReadPin, double temperatureFilterValue);
	void begin();

	//void enableLaser();
	//void disableLaser();

	void measureLight();
	double getLightValue();

	void measureTemperature();
	double getTemperature();
	double getHeavyTemperature();
	double getAmbientTemperature();

	void modifiedMovingAverage();
	void setZeroMeanAndVar(double mean,double var);
	double getLightZeroMean();
	double getLightMean();
	double getLightZeroVar();
	double probabilityTest();

	void resetZeroMean();



};
