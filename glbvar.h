#ifndef _GLBVAR_H_
#define _GLBVAR_H_


#define BAUDRATE 9600
//#define _DISPLAY_XONG_ 	fn80517segled_DATA_4BYTE(0xA1, 0xc0, 0xAB, 0x86); 		
//#define _DISPLAY_SAVED_ fn80517segled_DATA_4BYTE(0x92, 0x88, 0xC1, 0x86);
//#define _DISPLAY_LUU_ 	fn80517segled_DATA_4BYTE(0xC7, 0xE3, 0xE3, 0xBF);
//#define _DISPLAY_CAI_ 	fn80517segled_DATA_4BYTE(0xC6, 0x88, 0xCF, 0xCC);
//#define _DISPLAY_CAIH_ 	fn80517segled_DATA_4BYTE(0xC6, 0x88, 0xCF, 0x89);
//#define _DISPLAY_DUNG_ 	fn80517segled_DATA_4BYTE(0xA1, 0xE3, 0xAB, 0x10);
//#define _DISPLAY_WAIT_ 	fn80517segled_DATA_4BYTE(0xBF, 0xBF, 0xBF, 0xBF);
#define _TURN_PIN_OFF_ {pnoGlb8051Relay1 = 0;pnoGlb8051Relay2 = 0;pnoGlb8051BUZZER = 1;}
#define ITRPIORITYSETUP {IP = 0x10;IPH = 0x10;}
#define TIMER0ITR_HALT_{ET0 = 0; TR0 = 0;}
#define TIMER0ITR_ON_{ET0 = 1; TR0 = 1;EA =1;}
#define TIMER1ITR_HALT_{ET1 = 0; TR1 = 0;}
#define TIMER1ITR_ON_{ET1 = 1; TR1 = 1;EA =1;}
#define ESP8266 1
#define SIM 2
#define LORA 3
#define OFFLINE 0
#define L 1
#define R 0
////////////////khai bao functions protype/////////////
	void fn8051UART0Init();
	void fn8051SYS_SETUP_MODE();
	void fn8051SYS_MENU_MODE();
	void fn8051ITRRegSettup();
	void fn8051SYSPinMode();
	void fn8051SYSDELAY(unsigned int );
	unsigned char fn8051SYSCheckRQ();
	unsigned char fn8051SYSCheckAddress();
	void fn8051SYSRESETALLFLAG();
	void fn8051SYSReStack(unsigned char);
	void fn8051SYS_ACT_mode();
	void fn8051SYS_SAV_mode();
	void fn8051SYS_RQS_mode();
	void fn8051SYS_RQC_mode();
	void fn8051SYS_TMR_mode();
	void fn8051SYS_RQA_mode();
	void fn8051SYS_JOIN_mode();
	void fn8051SYS_DISPLAY_mode();
	void fn8051SYS_SAVONBOARD_mode();
	void fn8051SYS_RELAYSET_mode();
	void fn8051SYS_TIMESET_mode();
	void fn8051SIMINIT();
	void fn8051SIMSENDSTRING();
	void fn8051SIMSENDSMS ();
	void fn8051DS1307ReadTime();
	void fn8051DS1307WriteTime();
	void fn8051ITRVector4(void) ;
	void fn8051ITRVector2(void) ;
	void fn8051UARTSendChar (unsigned char );
	void fn8051UARTSendString(char* );
	void fn8051UARTSendNumber(unsigned long int,bit );
	void fn8051UARTSendFLOAT(float Number,bit);
	signed char fn8051ButtenEncode();
	void fn8051_BUZZER_(unsigned char mode);
	void fn8051SYSTimeEncode(unsigned  int ,unsigned char ,unsigned char );
	void fn8051SYSRelayTimingCheck(unsigned char);
	void fn8051EPROMWrite (unsigned int ,unsigned char , bit );
	unsigned long int pow(char ,char );
	void _DISPLAY_FUNC_(unsigned char mode);
///////////////khai bao hang so///////////////
	const code SEC   = 	0x00;
	const code MIN   = 	0x01;
	const code HOUR  = 	0x02;
	const code BUFFERSIZE = 100;
	const code RXDATASIZE    = 1000;
	const code HugeDADASIZE  = 300;
	const code SmalldataSize = 15;
	const code TimerDATA     = 100;
	const char code *ACT = "ACT";
	const char code *SAV = "SAV";
	const char code *TMR = "TMR";
	const char code *RQC = "RQC";
	const char code *RQS = "RQS";
	const char code *RQA = "RQA"; 
	const char code *JOINO = "JOIN:OK";
	const char code *JOINF = "JOIN:FAIL";
	const char code *PHONENUMBER = "";
	const char code *BTTHIETBI = "Bat/Tat Thiet bi";
	const char code *CAIGIO = "Cai dat thoi gian thuc";
	const char code *DANHSACH = "Danh sach hen gio da luu";
///////////////khai bao bien toan cuc//////////////
	unsigned char idata vrcGlb8051DS1307Sec,vrcGlb8051DS1307Min,vrcGlb8051DS1307Hour;
	bit volatile  vrbGlb8051SYSMainFlag = 0, vrbGlb8051SYSWrongSyntax = 0,vrbGlb8051SIMFLAG = 0,vrbGlb8051TOGGLEFLAG = 0,SHIFT_INDICATOR=0;
	unsigned char idata vrcGlbSYSRECEIVEADDR = 0;
	unsigned char vrcGlbSYSMYAddress = 0 ,vrcGlbSYSREQESTEDMODE;
	unsigned char xdata *vrcGlb8051UARTPointer,vrcGlbSYSMODULETYPE = 0;
	unsigned char idata VrcGlbAPROMRelayADR1=0,VrcGlbAPROMRelayADR2=0;
	unsigned int idata INDEX =0;
	unsigned char code Vruca1GlbTimerRelay1[100] _at_ 0x3E80;
	unsigned char code Vruca1GlbTimerRelay2[100] _at_ 0x3F00;	
#endif