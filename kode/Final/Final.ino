#include "Final.h"

void setup(){
	Serial.begin(115200);
	Serial.println("");
	Serial.println("");
	PWM.begin();
	PWM.set_control(-1,255 );
	sensor.begin();

	calibrate();
}

char nameList[] = "zeroMean,mean,pVal,ambTemp,ref,light,temperature,duty,dt,t,error,heating,heavtemp,derivative";

void loop(){
	double pValue = sensor.probabilityTest();
	double ref = refCalc.getReference();
	//double ref = 10;
	
	sensor.measureLight();	
	sensor.measureTemperature();
	sensor.modifiedMovingAverage();
	refCalc.refCalculation(sensor.getHeavyTemperature(),pValue,sensor.getLightMean());

	
	if(refCalc.getResetZeroMean() == 1){
		refCalc.setResetZeroMean();
		sensor.resetZeroMean();	
		Serial.println("test");
	}


	PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
	PWM.set_control(PIC.getDir(),PIC.getInput());
	double stateList[] = {sensor.getLightZeroMean(),sensor.getLightMean(),pValue,sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDt(),PIC.getT(),PIC.getError(),refCalc.getHeating(),sensor.getHeavyTemperature(),PIC.getDerivative()}; 	
		
	printStates(nameList,sizeof(nameList),stateList);		
}

void calibrate(){
	const static int sampleLength = 300; 
	double tmpmean = 0;
	double tmpvar = 0;
	//for(int j = 0; j < 8; j++){
		double dataSet[sampleLength] = {0};
		for(int i = 0; i < sampleLength; i++){
			sensor.measureLight();	
			sensor.measureTemperature();
			sensor.modifiedMovingAverage();
			double pValue = sensor.probabilityTest();
			//Serial.println(sensor.probabilityTest());
			//double ref = (double)7.5*sin((double)millis()/5000)+17.5;
			//double ref = sensor.getReference();
			double ref = 0;
			//PIC.inputCalculation(ref,sensor.getTemperature(),millis(),1.0);
			PWM.set_control(PIC.getDir(),PIC.getInput());
			//Serial.println(sensor.getLightZeroMean());
			double stateList[] = {sensor.getLightZeroMean(),sensor.getLightMean(),pValue,sensor.getAmbientTemperature(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDt(),PIC.getT(),PIC.getError(),refCalc.getHeating(),sensor.getHeavyTemperature(),PIC.getDerivative()};
			printStates(nameList,sizeof(nameList),stateList);		
			dataSet[i] = stateList[5];
			tmpmean += dataSet[i];
			delay(10);
		
		}
	tmpmean = tmpmean/sampleLength;
	for(int i=0; i<sampleLength; i++){
		//Serial.println(dataSet[i]);
	    tmpvar += pow(dataSet[i] - tmpmean, 2);
	}
	Serial.println(tmpvar);
	tmpvar = tmpvar/(sampleLength-1);
	//}
	sensor.setZeroMeanAndVar(tmpmean,80.0);
	PIC.resetIntegral();
	Serial.println("Calib Done..");
	delay(1000);
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

