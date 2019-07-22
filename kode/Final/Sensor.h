#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class Sensor{
private:
	int laserControlPin = 0;
	int laserState = 0;

	int 	lightSensorReadPin = 0;
	double 	lightFilterValue = 0.0;
	double 	lightValue = 700.0;
	
	int temperatureSensorReadPin = 0;
	double 	temperature = 20.0;
	double 	temperatureFilterValue = 0.0;

	double reference = 0;				//The reference that is to be followed
	int state = -1; //State = 1 => Heating, removing dew. State = -1 => Cooling, checking for de

	//Drystates
	double dryLightTreshold = 650;		//The Light treshold that needs to be passed for dew to be removed
	double dryTemperatureReference = 25;	//The Temperature estimate and reference for when dew is removed 
	double maxTemp = 30; 				//The max temperature for when dew should have been removed
	//DewStates
	double dewLightTreshold	= 500;		//The light treshold that needs to be passed for dew to have appeared
	double dewTemperatureReference = 8;	//The temperature estimate and reference for when dew appeared
	double minTemp = -5;


public:
	Sensor(int laserControlPin, int lightSensorReadPin,double lightFilterValue,int temperatureSensorReadPin, double temperatureFilterValue);
	void begin();

	void enableLaser();
	void disableLaser();

	void measureLight();
	double getLightValue();

	void measureTemperature();
	double getTemperature();

	void calculateReference();	
	double getReference();
	int getRefState();

	double getDryLightTreshold();
	double getDewLightTreshold();
	double getDewTemperatureReference();
	double getDryTemperatureReference();
};
