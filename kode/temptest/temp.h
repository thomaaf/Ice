#include <Wire.h>

#define MLX90615_I2C_ADDR    0x5A
//RAM ACCESS
#define RAM_RAW_DATA_IR_CHANNEL_1 	0x04
#define RAM_RAW_DATA_IR_CHANNEL_2 	0x05
#define RAM_TA                    	0x06
#define RAM_TOBJ1                 	0x07 
#define RAM_TOBJ2                 	0x08 

//EPROM ACCESS
#define EPROOM_MAX					0x20
#define EPROOM_MIN 					0x21
#define EPROOM_TA_RANGE 			0x23
#define EPROOM_PWM_CONTROL 			0x22
#define EPROOM_EMMISSION 			0x24

class tempsensor{
	private: 
		uint16_t read_word16(uint8_t reg);
	public: 

		void  begin();
		float get_AMB_tmp();
		float get_OBJ1_tmp();
		float get_OBJ2_tmp();
		float get_OBJTOT_tmp();
		
};