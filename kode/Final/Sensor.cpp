 #include "Sensor.h"
Sensor::Sensor(int laserControlPin, int lightSensorReadPin,int ambientTemperatureReadPin, double lightFilterValue,int temperatureSensorReadPin, double temperatureFilterValue){
	this -> laserControlPin = laserControlPin;
	
	this -> lightSensorReadPin = lightSensorReadPin;
	this -> lightFilterValue = lightFilterValue;
	this -> ambientTemperatureReadPin = ambientTemperatureReadPin; 
	this -> temperatureSensorReadPin = temperatureSensorReadPin;
	this -> temperatureFilterValue = temperatureFilterValue;

}
void Sensor::begin(){
	Serial.println("Init Sensor system...");

	pinMode(laserControlPin,OUTPUT);
	pinMode(lightSensorReadPin,INPUT);
	pinMode(temperatureSensorReadPin,INPUT);
	digitalWrite(laserControlPin,LOW);
	while((abs(getTemperature()) - 25.0 > 10) || (abs(getAmbientTemperature()) - 25.0 > 10)||(getLightValue()>820)||(getLightValue()<100)){
		measureTemperature();
		measureLight();				
		Serial.println("");
		Serial.println("=====Sensors out of normal bounds=====");
		Serial.println("Sensor status:");
		Serial.print("\t Ambient temperature: "); Serial.println(getAmbientTemperature());
		Serial.print("\t Temperature        : "); Serial.println(getTemperature());
		Serial.print("\t Light              : "); Serial.println(getLightValue());
		delay(100);
	}
	Serial.println("Sensor status:");
	Serial.print("\t Ambient temperature: "); Serial.println(getAmbientTemperature());
	Serial.print("\t Temperature        : "); Serial.println(getTemperature());
	Serial.print("\t Light              : "); Serial.println(getLightValue());
	int  t = millis();

	while(millis() - t < 5000){
		measureLight();
		measureTemperature();
	}
	Serial.println("Starting main loop...");

}	




void Sensor::measureLight(){
	//Ema Filtered// lightValue = lightValue + lightFilterValue*(analogRead(lightSensorReadPin) - lightValue);
	
	lightValue = analogRead(lightSensorReadPin) - round((double)0.4144*temperature);
}

double Sensor::getLightValue(){
	return lightValue;
}

void Sensor::measureTemperature(){
	temperatureHeavyFiltered = temperatureHeavyFiltered + temperatureHeavyFilterValue*(((double)(analogRead(temperatureSensorReadPin)-509)/1018*100) - temperatureHeavyFiltered);
	temperature = temperature + temperatureFilterValue*(((double)(analogRead(temperatureSensorReadPin)-509)/1018*100) - temperature);
}

double Sensor::getTemperature(){
	return temperature;
}

double Sensor::getHeavyTemperature(){
	return temperatureHeavyFiltered;
}

double Sensor::getAmbientTemperature(){
	return (double) analogRead(ambientTemperatureReadPin)/1023*500;
}

void Sensor::modifiedMovingAverage(){
	// Two rolling Mean. zeroMean with long Slength, s.t mean fits settlings and longer duration disturbances
	int sampleLengthShort = 50;
	int sampleLengthLong  = 8000;

	mean = (double)((sampleLengthShort - 1)*mean + lightValue)/sampleLengthShort;
	zeroMean = (double)((sampleLengthLong - 1)*zeroMean + lightValue)/sampleLengthLong;
}

double Sensor::getLightZeroMean(){
	return zeroMean;
}

double Sensor::getLightMean(){
	return mean;
}

double Sensor::getLightZeroVar(){
	return zeroVariance;
}

double Sensor::probabilityTest(){
	pValue = (mean - zeroMean)/pow(zeroVariance,0.5); 
	return pValue;
	//if(pValue < confidenceLevel){return 1;}
	//else{return 0;}
}

void Sensor::setZeroMeanAndVar(double mean,double var){
	Serial.println(mean);
	Serial.println(var);
	this -> zeroMean = mean;
	this -> zeroVariance = var;
	this -> mean = mean;
}


void Sensor::resetZeroMean(){
	zeroMean = mean;
}