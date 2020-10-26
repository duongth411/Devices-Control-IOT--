#include "defLCD0802.h"
#include "defDELAY16MHZ.h"
#include "N76E003.h"
#include "port.h"


// sbit LCD_RS 				= p0^0;
// sbit LCD_EN				= p1^0; //uncoment neu ko dung port.h
void LCD_SEND_COMMAND(unsigned char x)
{
	unsigned char nibh, nibl, port_tg;
	nibh = (x & 0xF0) >> 3;
	nibl = (x & 0x0F) << 1;
	RS = 0;
	port_tg = portLCD & 0xE1;
	portLCD = (nibh | port_tg);
	EN = 1;
	EN = 0;
	portLCD = (nibl | port_tg);
	EN = 1;
	EN = 0;
	fn8051SYSDELAY(1);
}

void LCD_DISPLAY_CHAR(unsigned char x)
{
	unsigned char nibh, nibl, port_tg;
	nibh = (x & 0xF0) >> 3;
	nibl = (x & 0x0F) << 1;
	RS = 1;
	port_tg = portLCD & 0xE1;
	portLCD = (nibh | port_tg);
	EN = 1;
	EN = 0;
	portLCD = (nibl | port_tg);
	EN = 1;
	EN = 0;
	fn8051SYSDELAY(1);
}

void LCD_Init()
{
	RS = 0;
	portLCD = 0x00;
	portLCD |= 0x04;
	EN = 1;
	EN = 0;
	fn8051SYSDELAY(1);
	LCD_SEND_COMMAND(FunctionSet);
	LCD_SEND_COMMAND(DisplayMode);
	LCD_SEND_COMMAND(EntryMode);
	LCD_SEND_COMMAND(CLEAR);
	LCD_SEND_COMMAND(HOME);
}
void LCD_SET_CURSOR (bit row, unsigned char pos){
	if (row == 0) {LCD_SEND_COMMAND((pos & 0x3F)|0x80);}
	else if (row == 1) {LCD_SEND_COMMAND((pos & 0x3F)|0xC0);}
}
void LCD_DISPLAY_NUMBER (unsigned char char_data){
	LCD_DISPLAY_CHAR(char_data+'0');
}
void LCD_DISPLAY_STRING (unsigned char *str)  {
	int i;
	for(i=0;str[i]!=0;i++) {
		LCD_DISPLAY_CHAR (str[i]);  
	} 
}
void LCD_NEW_SLIDE (bit mode){
	unsigned char i;
	for(i=0;i<8;i++){
		if(mode == 0){LCD_SHIFT_RIGHT}
		else LCD_SHIFT_LEFT
		fn8051SYSDELAY(3);
	}
}
void LCD_SCROLL_1LINE(unsigned char* STRING,unsigned char LENGTH){		//sua lai
	unsigned char COUNTER, OLDLEN ;
	unsigned char i;
	static bit mode;
	if(((COUNTER <= 255)&&(COUNTER > (LENGTH - 8)))||(OLDLEN!=LENGTH)){
		COUNTER = 1;
		mode = 0;
	}
	else if((COUNTER == (LENGTH - 8))||(COUNTER == 0)){
		mode ^= 1;
	}
	for(i=COUNTER;i<(COUNTER+8);i++){
		LCD_DISPLAY_CHAR(*(STRING+i));
	}
	if(!mode) COUNTER++;
	else if(mode) COUNTER--;
	OLDLEN = LENGTH;
}
void LCD_NEXT_SCREEN (bit mode){
	unsigned char i;
	for(i=0;i<8;i++){
		if(mode == 0){LCD_SHIFT_RIGHT}
		else LCD_SHIFT_LEFT
	}
}


























