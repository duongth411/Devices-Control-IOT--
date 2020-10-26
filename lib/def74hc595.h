/*-------------------------------------------------------------------------------------------*/
// Ten file: def74hc595.h
// Phien ban: V1.8
// Nguoi Viet: Tran Hoang Duong
// Ngay khoi dau: 01/04/2020
// Ngay ket thuc: 01/04/2020
// Tinh trang: TESTED OK
// Ghi chu: File thu vien 4 x led 7 thanh
/*-------------------------------------------------------------------------------------------*/
#ifndef _def74hc595_H_
#define _def74hc595_H_
#define COMMONANODE   //comment di neu su dung led am chung

	void fn80517segled_DISPLAY_(unsigned char *,bit ,unsigned char  );
	void fn80517segledCTRLLED(unsigned char *);
	void fn80517segledSENDDT74hc595(unsigned char );
	void fn80517segled_DATA_FLOAT(unsigned char , unsigned char , float *);
	void fn80517segled_DATA_RELAY(unsigned char , unsigned char );
	void fn80517segled_DATA_INT(unsigned int* );
	void fn80517segled_DATA_4BYTE(unsigned char , unsigned char , unsigned char  , unsigned char );
	void fn80517segled_DATA_RTC (unsigned char *hour, unsigned char *minute);
	void fn80517segled_DATA_MODE (unsigned char*,bit);
#endif