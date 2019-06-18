#include <Wire.h>
#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


class temp_RTD{
	private: 
		uint16_t read_word16(uint8_t reg);
		void  write_word16();
	public: 

		void  begin();
		float get_AMB_tmp();
		float get_OBJ1_tmp();
		float get_OBJ2_tmp();
		float get_OBJTOT_tmp();
		void set_emissivity(int em);
		
};