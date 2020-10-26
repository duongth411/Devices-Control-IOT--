#ifndef _def8051SoftI2c_H_
#define _def8051SoftI2c_H_
	///////////////////
	//thay doi SDA SCL trong N76E003 hoac uncomment dong duoi
//	sbit SDA	= P3^0;    
//	sbit SCL	= P1^7;  
	///////////////////
	void fn8051I2CStart(void);
	void fn8051I2CStop(void);
	bit fn8051I2CWrite(unsigned char dat);
	unsigned char fn8051I2CRead(void);
	void fn8051I2CACKWAIT();
#endif