#include "Final.h"

void setup(){
	Serial.begin(74880);
	sensor.begin();
	PWM.begin();
	PWM.set_control(-1,0 );
	Serial.println("");
	Serial.println("");
}

char nameList[] = "DryTempRef,DewTempRef,refstate,ref,light,temperature,duty,dir,dt,integral,t,error";

void loop(){
	sensor.measureLight();
	sensor.measureTemperature();
	sensor.calculateReference();	
	
	//double ref = (double)10*sin((double)millis()/1000)-10;
	double ref = sensor.getReference();
	//double ref = -10;
	PIC.inputCalculation(ref,sensor.getTemperature(),millis(),sensor.getRefState());
	PWM.set_control(PIC.getDir(),PIC.getInput());

	double stateList[] = {sensor.getDryTemperatureReference(),sensor.getDewTemperatureReference(),sensor.getRefState(), ref,sensor.getLightValue(),sensor.getTemperature(),PIC.getInput(),PIC.getDir(),PIC.getDt(),PIC.getIntegral(),PIC.getT(),PIC.getError()}; 
	printStates(nameList,sizeof(nameList),stateList);
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

