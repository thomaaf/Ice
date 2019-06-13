 #include "temp.h"

uint16_t tempsensor::read_word16(uint8_t reg){
  uint8_t i2c_addr_ = MLX90615_I2C_ADDR;
  uint16_t data, LSB, MSB;
  Wire.beginTransmission(i2c_addr_);
  Wire.write(reg);
  Wire.endTransmission(false);  

  Wire.requestFrom(i2c_addr_, (uint8_t)3);
  LSB= Wire.read();       // read low byte
  MSB = Wire.read(); // read high byte
  MSB = MSB << 8;
  data = MSB|LSB;

  Wire.read(); // read and discard PEC (packet error code
  
  return data;  
}

void tempsensor::begin() {
  Wire.begin();
}

float tempsensor::get_AMB_tmp(){
	return read_word16(RAM_TA)*0.02 - 273.15;
}

float tempsensor::get_OBJ1_tmp(){
	return read_word16(RAM_TOBJ1)*0.02 - 273.15;
}

float tempsensor::get_OBJ2_tmp(){
	return read_word16(RAM_TOBJ2)*0.02 - 273.15;
}

float tempsensor::get_OBJTOT_tmp(){
	float obj1 = read_word16(RAM_TOBJ1);
	float obj2 = read_word16(RAM_TOBJ2);
	return (((obj1 + obj2)/2)*0.02 - 273.15); 
}
