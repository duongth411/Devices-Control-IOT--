#ifndef _def8051DS1307_H_
#define _def8051DS1307_H_

	void fn8051DS1307Out_1Hz();
	unsigned char fn8051DS1307Readdata(unsigned char addr);
	void fn8051DS1307Writedata(unsigned char addr,unsigned char dat);

	
#endif