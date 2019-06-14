#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

class PWMdriver{
private:
	int en1;
	int en2;
	int D;
	int dir;
	void activate();
public:
	PWMdriver(int en1Pin, int en2Pin, int duty, int direction );
	void begin();
	
	void set_Duty_Cycle(int duty);
	int get_Duty_Cycle();
	
	int get_En1();
	int get_En2();
	void set_Dir(int direction);

	void set_control(int direction, int duty);
};
