 #include "RefCalc.h"

void RefCalc::refCalculation(double temperatureHeavyFiltered,double pValue,double mean){
	switch (heating){
		case 1 :
			//if(mean>710){
			if (abs(temperatureHeavyFiltered - dryTemperature)<treshold){
				heating = 0;
				reference = wetTemperature;
				resetZeroMean = 1;

			}
			return;
		case 0 :
			if(pValue<-2.0){
				heating = 1;
				wetTemperature = temperatureHeavyFiltered;
				reference = dryTemperature;
				return; 
			}
			else if(abs(temperatureHeavyFiltered - wetTemperature) < treshold){
				wetTemperature -=0.5;
				reference = wetTemperature;
				return;
			}
		default : 
			return;
	}
}
double RefCalc::getReference(){
	return reference;
}
int RefCalc::getHeating(){
	return heating;
}

int RefCalc::getResetZeroMean(){
	return resetZeroMean;
}

void RefCalc::setResetZeroMean(){
	resetZeroMean = 0;
}