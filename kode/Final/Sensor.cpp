 #include "Sensor.h"
Sensor::Sensor(int laserControlPin, int lightSensorReadPin,double lightFilterValue,int temperatureSensorReadPin, double temperatureFilterValue){
	this -> laserControlPin = laserControlPin;
	
	this -> lightSensorReadPin = lightSensorReadPin;
	this -> lightFilterValue = lightFilterValue;

	this -> temperatureSensorReadPin = temperatureSensorReadPin;
	this -> temperatureFilterValue = temperatureFilterValue;
}
void Sensor::begin(){
	pinMode(laserControlPin,OUTPUT);
	pinMode(lightSensorReadPin,INPUT);
	pinMode(temperatureSensorReadPin,INPUT);
	digitalWrite(laserControlPin,LOW);
}	


void Sensor::enableLaser(){
	digitalWrite(laserControlPin,HIGH);
}
void Sensor::disableLaser(){
	digitalWrite(laserControlPin,LOW);
}


void Sensor::measureLight(){
	lightValue = lightValue + lightFilterValue*(analogRead(lightSensorReadPin) - lightValue);
	//lightValue = 700 - millis()/50;
}

double Sensor::getLightValue(){
	return lightValue;
}

void Sensor::measureTemperature(){
	temperature = temperature + temperatureFilterValue*(((double)(analogRead(temperatureSensorReadPin)-518)/1023*100) - temperature);
}

double Sensor::getTemperature(){
	return temperature;
}

void Sensor::calculateReference(){
	switch (state){
		case 1 : //Looking for dew to be removed
			reference = dryTemperatureReference;
			if(lightValue>dryLightTreshold){
			//Light has reached steadyState, and dew has therefore been removed
				state = -1;
				dryTemperatureReference --; //Reduce the dewRemoved temperatureReference
				reference = dewTemperatureReference;
			}
			else if((temperature > dryTemperatureReference - 1)&&(temperature < dryTemperatureReference + 1 )){
			//Temperature has reached the estimated temperature where dew should have been removed, but
			//has not detected that the dew has been removed
				dryTemperatureReference++; //Increase the dryTemperature estimate, and set new reference
				reference = dryTemperatureReference;
			}
			else if(temperature>maxTemp){
			//The temperature has exceeded the maximum temperature. The dew has most likely been removed, but
			// It has not been detected. Resett the lightTreshold to this value, and also resett the dryTemperature to 
			// some value below the maxRating. Allowing the procedure to start again. '
				state = 0;
				reference = NULL;
			}
			return;
		case -1:
			reference = dewTemperatureReference;
			if( lightValue < dewLightTreshold){ 
			//Dew detechted
				state = 1;
				dewTemperatureReference ++;	//Increase the dewTemperatureReference in order to check for changes in next loop
				reference  = dryTemperatureReference;
			}
			else if((temperature > dewTemperatureReference - 0.3 )&&(temperature < dewTemperatureReference + 0.3)){ 
			//Temperature has reached estimated dewpoint, but has not discovered dew
				dewTemperatureReference --; // Decrease the estimat, and set new reference
				reference = dewTemperatureReference;
			}
			else if(temperature<minTemp){
			//The temperature has exceeded the minimum temperature. Should decrease the treshold
				state = 0;
				reference = NULL;
			}
			return;
		case 0 :
			reference = NULL;
			return;
	}
}

double Sensor::getReference(){
	return reference;
}
int Sensor::getRefState(){
	return state;
}
double Sensor::getDryLightTreshold(){
	return dryLightTreshold;
}

double Sensor::getDewLightTreshold(){
	return dewLightTreshold;
}
double Sensor::getDewTemperatureReference(){
	return dewTemperatureReference;
}
double Sensor::getDryTemperatureReference(){
	return dryTemperatureReference;
}