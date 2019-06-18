 #include "temp_IR.h"

byte crc8(byte *addr, byte len)
{
 byte crc = 0;
 while (len--) {
   byte inbyte = *addr++;
   for (byte i = 8; i; i--)
   {
     byte carry = (crc ^ inbyte) & 0x80;
     crc <<= 1;
     if (carry)
       crc ^= 0x7;
     inbyte <<= 1;
   }
 }
 return crc;
} 


void temp_IR::write_word16(){
  uint8_t i2c_addr_ = MLX90615_I2C_ADDR;
  byte buf [] = {0x00, 0x24,0x00,0x00  };
  byte crc = crc8 (buf, sizeof buf);
  Serial.print ("CRC was: ");
  Serial.println (crc, HEX);   


  
  Wire.beginTransmission(0x00);
  Wire.write(0x24); //Register Address to write to
  Wire.write(0x00); //Erase low byte (write 0)
  Wire.write(0x00); //Erase high byte (write 0)1
  Wire.write(crc); //Erase high byte (write 0)1
  Serial.println( Wire.endTransmission() );
  delay(1000);


  byte bu2 [] = {0x00, 0x24,0xBB,0x00  };
  crc = crc8 (bu2, sizeof bu2);
  Serial.print ("CRC was: ");
  Serial.println (crc, HEX);   

  Wire.beginTransmission(0x00);
  Wire.write(0x24); //Register Address to write to
  Wire.write(0xBB); //Erase low byte (write 0)
  Wire.write(0x00); //Erase high byte (write 0)1
  Wire.write(crc); //Erase high byte (write 0)1
  Serial.println( Wire.endTransmission() );
  delay(1000);
}

void temp_IR::set_emissivity(int em){
  write_word16();
  Serial.print("Emissivity: ");
  Serial.println(read_word16(EPROOM_EMMISSION),HEX);
  Serial.print("Emissivity2: ");
  Serial.println(read_word16(0x2F),DEC);

}

void temp_IR::begin() {
  Wire.begin();
}

float temp_IR::get_AMB_tmp(){
	return read_word16(RAM_TA)*0.02 - 273.15;
}

float temp_IR::get_OBJ1_tmp(){
	return read_word16(RAM_TOBJ1)*0.02 - 273.15;
}

float temp_IR::get_OBJ2_tmp(){
	return read_word16(RAM_TOBJ2)*0.02 - 273.15;
}

float temp_IR::get_OBJTOT_tmp(){
	float obj1 = read_word16(RAM_TOBJ1);
	float obj2 = read_word16(RAM_TOBJ2);
	return (((obj1 + obj2)/2)*0.02 - 273.15); 
}
uint16_t temp_IR::read_word16(uint8_t reg){
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