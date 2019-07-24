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

char nameList[] = "zeroVariance, zeroMean,mean,pVal,ambTemp,ref,light,temperature,duty,dir,dt,integral,t,error";

void loop(){
	
	sensor.measureLight();	
	sensor.measureTemperature();
	sensor.modifiedMovingAverage();
	//Serial.println(sensor.probabilityTest());
	//double ref = (double)7.5*sin((double)millis()/5000)+17.5;
	//double ref = sensor.getReference();
	double ref = -10;
	PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
	PWM.set_control(PIC.getDir(),PIC.getInput());
	double stateList[] = {sensor.getLightZeroVar(),sensor.getLightZeroMean(),sensor.getLightMean(),sensor.probabilityTest(),sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDir(),PIC.getDt(),PIC.getIntegral(),PIC.getT(),PIC.getError()}; 	
	printStates(nameList,sizeof(nameList),stateList);		
}

void calibrate(){
	const static int sampleLength = 300; 
	double mean = 0;
	double var = 0;
	//for(int j = 0; j < 8; j++){
		double dataSet[sampleLength] = {0};
		for(int i = 0; i < sampleLength; i++){
			sensor.measureLight();	
			sensor.measureTemperature();
			sensor.modifiedMovingAverage();
			//Serial.println(sensor.probabilityTest());
			//double ref = (double)7.5*sin((double)millis()/5000)+17.5;
			//double ref = sensor.getReference();
			double ref = -10;
			PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
			PWM.set_control(PIC.getDir(),PIC.getInput());
			double stateList[] = {sensor.getLightZeroVar(),sensor.getLightZeroMean(),sensor.getLightMean(),sensor.probabilityTest(),sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDir(),PIC.getDt(),PIC.getIntegral(),PIC.getT(),PIC.getError()}; 	
			printStates(nameList,sizeof(nameList),stateList);		
			//Serial.println("Calibration Phase");
			dataSet[i] = stateList[6];
			mean += dataSet[i];
		
		}
	mean = mean/sampleLength;
	for(int i=0; i<sampleLength; i++){
	    var += pow(dataSet[i] - mean, 2);
	}
	Serial.println(var);
	var = var/(sampleLength-1);
	//}
	sensor.setZeroMeanAndVar(mean,var);
	Serial.println("Calib Done..");
	delay(8000);
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

