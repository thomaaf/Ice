#include "Final.h"

void setup(){
	Serial.begin(500000);
	Serial.println("");
	Serial.println("");
	PWM.begin();
	PWM.set_control(-1,255 );
	sensor.begin();
	calibrate();
}

char nameList[] = "zeroMean,mean,pVal,ambTemp,ref,light,temperature,duty,dir,dt,integral,t,error";

void loop(){
	
	sensor.measureLight();	
	sensor.measureTemperature();
	sensor.modifiedMovingAverage();
	//Serial.println(sensor.probabilityTest());
	double ref = (double)7.5*sin((double)millis()/5000)+17.5;
	//double ref = sensor.getReference();
	//double ref = -10;
	PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
	PWM.set_control(PIC.getDir(),PIC.getInput());

	double stateList[] = {sensor.getLightZeroMean(),sensor.getLightMean(),sensor.probabilityTest(),sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDir(),PIC.getDt(),PIC.getIntegral(),PIC.getT(),PIC.getError()}; 
	
	printStates(nameList,sizeof(nameList),stateList);		
}

void calibrate(){
	const static int sampleLength = 300; 
	double zeroMean = 0;
	double zeroVariance = 0;
	double avgMean = 0;
	double avgVar = 0;
	for(int j = 0; j < 8; j++){
		zeroMean = 0;
		zeroVariance = 0;
		double dataSet[sampleLength] = {0};
		for(int i = 0; i < sampleLength; i++){
			sensor.measureLight();	
			sensor.measureTemperature();
			//sensor.calculateReference();	
			//sensor.calibrate();

			double ref = (double)7.5*sin((double)millis()/5000)+17.5;
			//double ref = sensor.getReference();
			//double ref = -10;
			PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
			PWM.set_control(PIC.getDir(),PIC.getInput());

			double stateList[] = {sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDir(),PIC.getDt(),PIC.getIntegral(),PIC.getT(),PIC.getError()}; 
			printStates(nameList,sizeof(nameList),stateList);			
			Serial.println("Calibration Phase");
			dataSet[i] = stateList[2];
			zeroMean += dataSet[i];
		}
		zeroMean = (double)zeroMean/sampleLength;
		for(int i = 0; i < sampleLength; i++){
			zeroVariance += pow(dataSet[i] - zeroMean,2);
			//Serial.println(zeroVariance);
		}
		zeroVariance =(double) zeroVariance/(sampleLength-1);
		avgVar += zeroVariance;
		avgMean += zeroMean;

	}
	zeroMean = avgMean/8.0;
	zeroVariance = avgVar/8.0;	
	sensor.setZeroMeanAndVar(zeroMean,zeroVariance);
}
void printStates(char *nameList, double nameListLength, double *stateList){
	int stateCounter = 0;
	for (int i = 0; i <nameListLength-1; i++){
		Serial.print(nameList[i]);
		if (nameList[i] == ','){
			Serial.print(stateList[stateCounter]);
			Serial.print(",");
			stateCounter ++;
		}
	}
	Serial.print(",");
	Serial.print(stateList[stateCounter]);
	Serial.println("");

}

