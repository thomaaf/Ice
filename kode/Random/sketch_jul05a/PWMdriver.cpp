 #include "PWMdriver.h"


PWMdriver::PWMdriver(int en1Pin, int en2Pin, int duty,int direction) {
  en1 = en1Pin;
  en2 = en2Pin;
  D = duty;
  dir = direction;
}	

void PWMdriver::activate(){
	switch (dir){
		case 1:
			digitalWrite(en1, LOW);
			analogWrite(en2, D);
			return;
		case -1: 
			analogWrite(en1, D);
			digitalWrite(en2, LOW);
			return;
		case 0: 
			digitalWrite(en1, LOW);
			digitalWrite(en2, LOW);
			return;
	}	

}

void PWMdriver::begin(){
	pinMode(en1,OUTPUT);
	pinMode(en2,OUTPUT);
  
	digitalWrite(en1, LOW);
	digitalWrite(en2, LOW);
}


void PWMdriver::set_Duty_Cycle(int duty){
	D = duty;
	activate();
}

int PWMdriver::get_Duty_Cycle(){
	return D;
}

int PWMdriver::get_En1(){
	return en1;
}

int PWMdriver::get_En2(){
	return en2;
}

void PWMdriver::set_Dir(int direction){
	dir = direction;
	activate();

}

void PWMdriver::set_control(int direction, int duty){
	dir = direction;
	D = duty;
	activate();
}
