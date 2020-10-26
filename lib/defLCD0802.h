#ifndef _defLCD0802_H_
#define _defLCD0802_H_

	#define LCD_4BIT_CLR 			(portLCD & 0xE1)
	#define portLCD 				P1
	#define LCD_CURSOR_ON 			LCD_SEND_COMMAND(0x0E);
	#define LCD_CURSOR_OFF 			LCD_SEND_COMMAND(0x0C);
	#define LCD_CURSOR_BLINK 		LCD_SEND_COMMAND(0x0F);
	#define LCD_SHIFT_LEFT 			LCD_SEND_COMMAND(0x18);
	#define LCD_SHIFT_RIGHT 		LCD_SEND_COMMAND(0x1C);
	#define LCD_CURSOR_MOVE_LEFT 	LCD_SEND_COMMAND(0x10);
	#define LCD_CURSOR_MOVE_RIGHT 	LCD_SEND_COMMAND(0x14);
	#define LCD_CLEARDISPLAY 		LCD_SEND_COMMAND(0x01);
	#define LCD_RETURNHOME 			{LCD_SEND_COMMAND(0x02); LCD_SET_CURSOR(0,0);}
	#define FunctionSet 0x28
	#define DisplayMode 0x0C
	#define EntryMode 0x06
	#define HOME 0x02
	#define CLEAR 0x01
	
	void LCD_Init (void) ;
	void LCD_SEND_COMMAND (unsigned char cmnd);
	void LCD_DISPLAY_CHAR (unsigned char char_data);
	void LCD_DISPLAY_STRING (unsigned char *str);
	void LCD_SET_CURSOR (bit row, unsigned char pos);
	void LCD_NEW_SLIDE (bit mode);					//1 phai 0 trai
	void LCD_DISPLAY_NUMBER (unsigned char char_data);
	void LCD_SCROLL_1LINE(unsigned char* STRING,unsigned char LENGTH);
	void LCD_NEXT_SCREEN (bit mode);
	
#endif

