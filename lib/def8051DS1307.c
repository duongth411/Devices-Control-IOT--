#include "def8051DS1307.h"
#include "def8051SoftI2c.h"

void fn8051DS1307Out_1Hz(){
	fn8051I2CStart(); /* Start i2c bus */
	fn8051I2CWrite(0xD0); /* Connect to DS1307 */
	fn8051I2CWrite(0x07);   //vi tri con tro RTC
	fn8051I2CWrite(0x10);
	fn8051I2CStop();
}
unsigned char fn8051DS1307Readdata(unsigned char addr){
	unsigned int temp,ret;
	fn8051I2CStart(); 		/* Start i2c bus */
	fn8051I2CWrite(0xD0); 	/* Connect to DS1307 */
	fn8051I2CWrite(addr); 	/* Request RAM address on DS1307 */
	fn8051I2CStart(); 		/* Start i2c bus */
	fn8051I2CWrite(0XD1); 	/* Connect to DS1307 for Read */
	ret = fn8051I2CRead(); 	/* Receive data */
	fn8051I2CStop();
	//**********************************************************
	temp = ret; 							/*BCD to HEX*/
	ret = (((ret/16)*10)+ (temp & 0x0f)); 	/*for Led 7seg*/
	//**********************************************************
	return ret;
}
void fn8051DS1307Writedata(unsigned char addr,unsigned char dat){
	unsigned int temp;
	//********************************************** 
	temp = dat ; 						/*HEX to BCD*/
	dat = (((dat/10)*16)|(temp %10)); 	/*for Led 7seg*/
	//**********************************************
	fn8051I2CStart()
; 		/* Start i2c bus */
	fn8051I2CWrite(0XD0); 	/* Connect to DS1307 */
	fn8051I2CWrite(addr); 	/* Request RAM address on DS1307 */
	fn8051I2CWrite(dat); 	/* Connect to DS1307 for Read */
	fn8051I2CStop();
}


