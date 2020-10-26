#include "def8051SoftI2C.h"
#include "N76E003.h"
void fn8051I2CStart(void){
	SCL = 1; SCL = 0;
	SDA = 1; SCL = 1;
	SDA = 0; 
	SCL = 0;
}
void fn8051I2CStop(void){
	SCL = 1; SCL = 0;
	SDA = 0;
	SCL = 1; 
	SDA = 1;
}
bit fn8051I2CWrite(unsigned char dat){
	unsigned char i;
	for (i=0;i<8;i++){
		SDA = (dat & 0x80) ? 1:0;
		SCL=1;SCL=0;
		dat<<=1;
	}
	SCL = 1;
	SCL = 0;
	return dat;
}
unsigned char fn8051I2CRead(void){	
	bit rd_bit;
	unsigned char i, dat;
	dat = 0x00;
	for(i=0;i<8;i++) 										/* For loop read data 1 byte */
	{
		SCL = 1; 				/* Set SCL */
		rd_bit = SDA; 										/* Keep for check acknowledge */
		dat = dat<<1;
		dat = dat | rd_bit; 							/* Keep bit data in dat */
		SCL = 0; 													/* Clear SCL */
	}
	return dat;
}
void fn8051I2CACKWAIT(){
    SDA = 1; //Configure SDA_PIN as Input to read the ACK                //delay_us(1);
    SCL = 1;
    while(SDA==1);
    SCL = 0;
}