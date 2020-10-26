
 
#include "main.h"

void main(){	
	fn8051SYSPinMode();	
	LCD_Init();
	LCD_SET_CURSOR(0,0);
	LCD_DISPLAY_STRING("");
	LCD_SET_CURSOR(1,0);
	LCD_DISPLAY_STRING("");
	ITRPIORITYSETUP
	_TURN_PIN_OFF_
	fn8051ITRRegSettup();
	fn8051UART0Init();
	fn8051DS1307Out_1Hz();
	fn8051SIMINIT();
	vrcGlb8051UARTPointer = (unsigned char*) malloc(RXDATASIZE * sizeof(unsigned char));
	fn8051SYSDELAY(200);
	while(1){
		if(vrbGlb8051SYSMainFlag == 0){
			fn8051SYS_DISPLAY_mode();
			fn8051SYS_MENU_MODE();
		}
		else if (vrbGlb8051SYSMainFlag == 1){
			TIMER0ITR_HALT_
			vrcGlbSYSRECEIVEADDR = fn8051SYSCheckAddress();		
			if(vrcGlbSYSRECEIVEADDR == vrcGlbSYSMYAddress){
				vrcGlbSYSREQESTEDMODE = fn8051SYSCheckRQ();
				if(vrcGlbSYSREQESTEDMODE == 1){					
					fn8051SYS_ACT_mode();
					fn8051SYSRESETALLFLAG();
				}
				else if(vrcGlbSYSREQESTEDMODE == 2){			
					fn8051SYS_SAV_mode();
					fn8051SYSRESETALLFLAG();
				}	
				else if(vrcGlbSYSREQESTEDMODE == 3){			
					fn8051SYS_TMR_mode();
					fn8051SYSRESETALLFLAG();
				}
				else if(vrcGlbSYSREQESTEDMODE == 4){		
					fn8051SYS_RQC_mode() ;
					fn8051SYSRESETALLFLAG();
				}
				else if(vrcGlbSYSREQESTEDMODE == 5){			
					fn8051SYS_RQS_mode();
					fn8051SYSRESETALLFLAG();
				}
				else if(vrcGlbSYSREQESTEDMODE == 6){	
					fn8051SYS_RQA_mode() ;
					fn8051SYSRESETALLFLAG();
				}
				else if(vrcGlbSYSREQESTEDMODE == 0){
					fn8051SYSRESETALLFLAG();
				}
			}
			else if (vrcGlbSYSRECEIVEADDR != vrcGlbSYSMYAddress) {
				fn8051SYS_JOIN_mode() ;
				fn8051SYSRESETALLFLAG();
			}	
		}
	}
}
void fn8051SYSPinMode(){
	Set_All_GPIO_Quasi_Mode;
	P15_PushPull_Mode;
	P03_PushPull_Mode;
	P02_PushPull_Mode;
	P00_PushPull_Mode;
	P10_PushPull_Mode;
	P11_PushPull_Mode;
	P12_PushPull_Mode;
	P13_PushPull_Mode;
	P14_PushPull_Mode;
}
void fn8051ITRRegSettup(){
	TMOD = 0x11;
	TH0 = 0xC1;
	TL0 = 0x80;
	set_T0M;
	TF0 = 0; 
	TIMER0ITR_ON_
	clr_T1M;
	TF1 = 0; 
	ET1 = 1;
	TR1 = 1;
	TH1 = 0x63;
	TL1 = 0xC0;
}
void fn8051UART0Init(){
	if ( vrcGlbSYSMODULETYPE != OFFLINE){
		InitialUART0_Timer3(BAUDRATE);
		clr_TI;
		clr_RI;
		set_EA;
		set_ES;    
	}
}
void  fn8051ITRVector1(void) interrupt 1{ //timer 1ms
	unsigned char CHECKSETTIMECOUNT,SHIFTCOUNT;
	static unsigned char checklocate = 0;
	CHECKSETTIMECOUNT++;
	SHIFTCOUNT++;
	if(CHECKSETTIMECOUNT > 60){
		fn8051SYSRelayTimingCheck(checklocate);
		checklocate++;
		if(checklocate == 101){checklocate = 0;}
		CHECKSETTIMECOUNT = 0;
	}
	if(SHIFTCOUNT>2){
		SHIFT_INDICATOR = 1;
		SHIFTCOUNT = 0;
	}
	TH0 = 0xC1;
	TL0 = 0x80;
}
void  fn8051ITRVector3(void) interrupt 3{ //timer 30ms TIMER 1
	unsigned char togglecount;
	togglecount++;
	if(togglecount > 250){				//2,1s
		vrbGlb8051TOGGLEFLAG = 1;
		togglecount = 0;
	}	
	TH1 = 0x63;
	TL1 = 0xC0;
}
void fn8051ITRVector4(void) interrupt 4 {
	unsigned char throwaway;
    if(RI==1){
		throwaway = SBUF;
		if (throwaway == '>'){
			vrbGlb8051SIMFLAG = 1;
			RI = 0;
		}
		else if((throwaway >= '!')&&(throwaway <= 'z')){
			*(vrcGlb8051UARTPointer+INDEX) = throwaway;
			INDEX++;
			if(*(vrcGlb8051UARTPointer+INDEX-1) == '$') {
				vrbGlb8051SYSMainFlag = 1;
				fn8051_BUZZER_(1);
			}
			RI = 0;
		}
		else if((throwaway < '!')||(throwaway > 'z')){
			throwaway=0;
			RI = 0;
		}	
	}
	if(TI==1){
		TI =0;
	}
}
unsigned char fn8051SYSCheckAddress(){
	unsigned char i=2,RECEIVEDADDR =0;
	if(vrcGlbSYSMODULETYPE == SIM){
		vrcGlb8051UARTPointer = strstr(vrcGlb8051UARTPointer,"S:");
	}
	if((*(vrcGlb8051UARTPointer)=='S')&&(*(vrcGlb8051UARTPointer+1)==':')){               
		while((*(vrcGlb8051UARTPointer+i)) > 47 && (*(vrcGlb8051UARTPointer+i) < 58)){
				RECEIVEDADDR = (*(vrcGlb8051UARTPointer+i)-'0')+RECEIVEDADDR*10;	
				i++;
		}
	}
	i =2;
	return RECEIVEDADDR;
}
unsigned char fn8051SYSCheckRQ(){
	unsigned char *vrc8051SYSPointer,RequestType = 0;
	char *indicator = ";";
	vrc8051SYSPointer = (unsigned char*) malloc(SmalldataSize * sizeof(unsigned char));
	vrc8051SYSPointer = strtok(vrcGlb8051UARTPointer,indicator);
	free(vrc8051SYSPointer);
	indicator = "!";
	vrc8051SYSPointer = strtok(NULL,indicator);
		 if(strcmp(vrc8051SYSPointer,ACT) == 0)  {RequestType = 1; }
	else if(strcmp(vrc8051SYSPointer,SAV) == 0)  {RequestType = 2; }
	else if(strcmp(vrc8051SYSPointer,TMR) == 0)  {RequestType = 3; }
	else if(strcmp(vrc8051SYSPointer,RQC) == 0)  {RequestType = 4; }
	else if(strcmp(vrc8051SYSPointer,RQS) == 0)  {RequestType = 5; }
	else if(strcmp(vrc8051SYSPointer,RQA) == 0)  {RequestType = 6; }
	else if(strcmp(vrc8051SYSPointer,JOINO) == 0){RequestType = 7; }
	else if(strcmp(vrc8051SYSPointer,JOINF) == 0){RequestType = 8; }
	free(vrc8051SYSPointer);
	return RequestType;
}
void fn8051SYS_ACT_mode(){
	unsigned char *vrcPointer,RelayNum,RelayStatus,i=0;
	char *indicator = "$";
	vrcPointer = (unsigned char*) malloc(HugeDADASIZE * sizeof(unsigned char));
	vrcPointer = strtok(NULL,indicator);
	strcat(vrcPointer,"&");
	while(*(vrcPointer+i) != '&'){
		if(*(vrcPointer+i) == 'R'){
			RelayNum = *(vrcPointer+i+2)-'0';	
			RelayStatus = (*(vrcPointer+i+4)-'0')?1:0;
			if (RelayNum == 1){pnoGlb8051Relay1 = RelayStatus;}
			else if (RelayNum == 2) {pnoGlb8051Relay2 = RelayStatus;}
		}
		i++;
	}
}	
void fn8051SYS_TMR_mode(){
	unsigned char *vrc8051SYSPointer,vrucSYSstrlen =0,HOUR,MIN,SEC,systaxcheck=0;
	char *indicator = ";";
	vrc8051SYSPointer = (unsigned char*) malloc(SmalldataSize * sizeof(unsigned char));
	vrc8051SYSPointer = strtok(NULL,indicator);
	vrucSYSstrlen = strlen(vrc8051SYSPointer);
	if(*(vrc8051SYSPointer)=='H'&&*(vrc8051SYSPointer+1)==':'){
		HOUR = ((*(vrc8051SYSPointer+2)-'0'))*10+(*(vrc8051SYSPointer+3)-'0'); 			
	}
	else{
		vrbGlb8051SYSWrongSyntax = 1;
	}	
	vrc8051SYSPointer = strtok(NULL,indicator);	
	if(*(vrc8051SYSPointer)=='M'&&*(vrc8051SYSPointer+1)==':'){
		MIN = ((*(vrc8051SYSPointer+2)-'0'))*10+(*(vrc8051SYSPointer+3)-'0');
	}
	else{
		vrbGlb8051SYSWrongSyntax = 1;
	}	
	vrc8051SYSPointer = strtok(NULL,indicator);	
	if(*(vrc8051SYSPointer)=='S'&&*(vrc8051SYSPointer+1)==':'){
		SEC = ((*(vrc8051SYSPointer+2)-'0'))*10+(*(vrc8051SYSPointer+3)-'0');
	}
	else{
		vrbGlb8051SYSWrongSyntax = 1;
	}
	vrc8051SYSPointer = strtok(NULL,indicator);	
	if(*vrc8051SYSPointer != '$'){
		vrbGlb8051SYSWrongSyntax = 1;
	}
	vrc8051SYSPointer = strtok(NULL,indicator);	
	if(vrbGlb8051SYSWrongSyntax ==0){
		vrcGlb8051DS1307Hour = HOUR;
		vrcGlb8051DS1307Min  = MIN;
		vrcGlb8051DS1307Sec  = SEC;
	}
	fn8051DS1307WriteTime();
	free(vrc8051SYSPointer);
}
void fn8051SYS_SAV_mode(){
	unsigned char *vrc8051TimePointer,Locate=4,RelayNum,RelayStatus,*timestring;
	unsigned int TimeSet= 0;
	char *indicator = ";";
	timestring = (unsigned char*) malloc(HugeDADASIZE * sizeof(unsigned char));
	vrc8051TimePointer = (unsigned char*) malloc(SmalldataSize * sizeof(unsigned char));
	timestring = strtok(NULL,"$");
	timestring = strcat(timestring,"$;");
	vrc8051TimePointer = strtok(timestring,indicator);
	while(*(vrc8051TimePointer) != '$'){
		if(*vrc8051TimePointer=='R'&&*(vrc8051TimePointer+1)==':'){
			RelayNum = *(vrc8051TimePointer+2)-'0';		
			while(*(vrc8051TimePointer+Locate) >= '0' && *(vrc8051TimePointer+Locate) <= '9'){
				TimeSet = ((*(vrc8051TimePointer+Locate))-'0')+(TimeSet*10);
				Locate++;
			}
			RelayStatus = (*(vrc8051TimePointer+Locate+1))-'0'; 
			fn8051SYSTimeEncode(TimeSet,RelayStatus,RelayNum);	
			TimeSet = 0;
			Locate = 4;
			}
		vrc8051TimePointer = strtok(NULL,indicator);
	}	
}
void fn8051SYS_RQS_mode(){
	unsigned char relaystatus,i;
	unsigned int time;
	if(vrcGlbSYSREQESTEDMODE == 5){
		fn8051SIMSENDSTRING();
		fn8051UARTSendString("S:");
		fn8051UARTSendNumber(vrcGlbSYSMYAddress,0);
		fn8051UARTSendChar(';');
		fn8051UARTSendString(RQS);
		fn8051UARTSendChar('!');
	}
	for (i=0;i<VrcGlbAPROMRelayADR1;i+=2){
		time = (Vruca1GlbTimerRelay1[i+1]&(0x3f)) +(unsigned int) (Vruca1GlbTimerRelay1[i]*60);
		relaystatus = (Vruca1GlbTimerRelay1[i+1] & 0x40) ? 1:0 ;
		fn8051UARTSendString("RS:");
		fn8051UARTSendNumber(1,0);
		fn8051UARTSendChar(':');
		fn8051UARTSendNumber(time,0);
		fn8051UARTSendChar(':');
		fn8051UARTSendNumber(relaystatus,0);
		fn8051UARTSendChar(';');
	}
	for (i=0;i<VrcGlbAPROMRelayADR2;i+=2){
		time = (Vruca1GlbTimerRelay2[i+1]&(0x3f)) +(unsigned int) (Vruca1GlbTimerRelay2[i]*60);
		relaystatus = (Vruca1GlbTimerRelay2[i+1] & 0x40) ? 1:0 ;
		fn8051UARTSendString("RS:");
		fn8051UARTSendNumber(2,0);
		fn8051UARTSendChar(':');
		fn8051UARTSendNumber(time,0);
		fn8051UARTSendChar(':');
		fn8051UARTSendNumber(relaystatus,0);
		fn8051UARTSendChar(';');
	}
	fn8051UARTSendChar('$');
	if(vrcGlbSYSREQESTEDMODE == 5){
		fn8051SIMSENDSMS();
	}
}
void fn8051SYS_RQC_mode(){
	unsigned long int TMRtime;
	char i;
	fn8051DS1307ReadTime();
	TMRtime = vrcGlb8051DS1307Sec+(unsigned int)(vrcGlb8051DS1307Min*60)+(unsigned long int)(vrcGlb8051DS1307Hour*3600);
	if(vrcGlbSYSREQESTEDMODE == 4){
		fn8051SIMSENDSTRING();
		fn8051UARTSendString("S:");
		fn8051UARTSendNumber(vrcGlbSYSMYAddress,0);
		fn8051UARTSendChar(';');
		fn8051UARTSendString(RQC);
		fn8051UARTSendChar('!');
	}
	fn8051UARTSendNumber(TMRtime,0);
	fn8051UARTSendChar(';');
	for (i=1;i<3;i++){
		fn8051UARTSendString("RC:");
		fn8051UARTSendNumber(i,0);
		fn8051UARTSendChar(':');
		if(i == 1){fn8051UARTSendNumber(pnoGlb8051Relay1,0);}
		else if(i == 2){fn8051UARTSendNumber(pnoGlb8051Relay2,0);}
		fn8051UARTSendChar(';');
	}
	fn8051UARTSendString("ADCC:1:");
	fn8051UARTSendFLOAT(fn8051ADCREAD(1,0),0);
	fn8051UARTSendChar(';');
	fn8051UARTSendString("ADCC:2:");
	fn8051UARTSendFLOAT(fn8051ADCREAD(0,0),0);
	fn8051UARTSendChar(';');
	if(vrcGlbSYSREQESTEDMODE == 4){  
		fn8051UARTSendChar('$');
		fn8051SIMSENDSMS();
	}
	
}
void fn8051SYS_RQA_mode(){
	fn8051SIMSENDSTRING();
	fn8051UARTSendString("S:");
	fn8051UARTSendNumber(vrcGlbSYSMYAddress,0);
	fn8051UARTSendChar(';');
	fn8051UARTSendString(RQA);
	fn8051UARTSendChar('!');
	fn8051SYS_RQC_mode();
	fn8051SYS_RQS_mode();
	fn8051SIMSENDSMS();
}
void fn8051SYS_JOIN_mode(){
	vrcGlbSYSREQESTEDMODE = fn8051SYSCheckRQ();
	if(vrcGlbSYSREQESTEDMODE<7){
		//not my device's addreess case
	}
	else if(vrcGlbSYSREQESTEDMODE == 7){
		vrcGlbSYSMYAddress = vrcGlbSYSRECEIVEADDR;
	}
	else if (vrcGlbSYSREQESTEDMODE == 8){
		// join failed
	}
}
void fn8051SYS_DISPLAY_mode(){						//Show the main Menu
	float VADC;
	unsigned char DISPLAY_INDICATOR = 0,HOLD_DT=0;
	signed char BUTTON_ENCODE;
	bit TRACKING = 0;
	BUTTON_ENCODE = 0;
	TIMER1ITR_ON_
	TIMER0ITR_ON_
	free(vrcGlb8051UARTPointer);  
	_DISPLAY_FUNC_(1);
	while (vrbGlb8051SYSMainFlag == 0){
		if(DISPLAY_INDICATOR == 0){
			fn8051DS1307ReadTime();
			LCD_SET_CURSOR(1,0);
			fn8051UARTSendNumber(vrcGlb8051DS1307Hour,1);
			LCD_DISPLAY_CHAR(':');
			fn8051UARTSendNumber(vrcGlb8051DS1307Min,1);
			LCD_DISPLAY_CHAR(':');
			fn8051UARTSendNumber(vrcGlb8051DS1307Sec,1);
		}
		else if(DISPLAY_INDICATOR == 1){
			LCD_SET_CURSOR(1,11);
			if (pnoGlb8051Relay1 == 0){ LCD_DISPLAY_STRING("Off");}
			else { LCD_DISPLAY_STRING("On");}
		}
		else if(DISPLAY_INDICATOR == 2){
			LCD_SET_CURSOR(1,19);
			if (pnoGlb8051Relay2 == 0){ LCD_DISPLAY_STRING("Off");}
			else { LCD_DISPLAY_STRING("On");}
		}
		else if(DISPLAY_INDICATOR == 3){
			VADC = fn8051ADCREAD(0,0);
			LCD_SET_CURSOR(1,29);
			fn8051UARTSendFLOAT(VADC,1);
		}
		else if(DISPLAY_INDICATOR == 4){
			VADC = fn8051ADCREAD(1,0);
			LCD_SET_CURSOR(1,37);
			fn8051UARTSendFLOAT(VADC,1);
		}
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if (BUTTON_ENCODE == 0){
			if(TRACKING == 0 && vrbGlb8051TOGGLEFLAG == 1){
				DISPLAY_INDICATOR++;
				if (DISPLAY_INDICATOR >= 5){ DISPLAY_INDICATOR = 0;}
				vrbGlb8051TOGGLEFLAG = 0;
				LCD_NEW_SLIDE(L);
			}
			else if (TRACKING == 1){
				DISPLAY_INDICATOR = DISPLAY_INDICATOR;
			}
		}
		else if(BUTTON_ENCODE == 1){ 
			DISPLAY_INDICATOR ++;
			if(DISPLAY_INDICATOR>=5){
				DISPLAY_INDICATOR = 0;
			}
			LCD_NEW_SLIDE(L);		
		}
		else if (BUTTON_ENCODE == -1){
			DISPLAY_INDICATOR--;
			if(DISPLAY_INDICATOR<=255&&DISPLAY_INDICATOR>10){
				DISPLAY_INDICATOR = 4;
			}
			LCD_NEW_SLIDE(R);
		}
		else if (BUTTON_ENCODE == 2) {
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				HOLD_DT = 0;
				break;
			}
			else {
				TRACKING ^= 1;
				if(TRACKING == 0) { 
					vrbGlb8051TOGGLEFLAG = 0;
					fn8051_BUZZER_(1);
				}
				else {
					fn8051_BUZZER_(0);
				}
				HOLD_DT = 0;	
			}
		}
	}
}
void fn8051SYS_MENU_MODE(){							//Show the Setting Menu Screen
	unsigned char DISPLAY_INDICATOR = 0,STRING_LEN,HOLD_DT =0;
	signed char BUTTON_ENCODE;
	_DISPLAY_FUNC_(0);
	while(vrbGlb8051SYSMainFlag == 0){
		if(DISPLAY_INDICATOR == 0){
			if(SHIFT_INDICATOR == 1){
				STRING_LEN = strlen(BTTHIETBI);
				LCD_SET_CURSOR(1,0);
				LCD_SCROLL_1LINE(BTTHIETBI,STRING_LEN);
				SHIFT_INDICATOR = 0;
			}
		}
		else if(DISPLAY_INDICATOR == 2){
			if(SHIFT_INDICATOR == 1){
				STRING_LEN = strlen(CAIGIO);
				LCD_SET_CURSOR(1,16);
				LCD_SCROLL_1LINE(CAIGIO,STRING_LEN);
				SHIFT_INDICATOR = 0;
			}
		}
		else if(DISPLAY_INDICATOR == 4){
			if(SHIFT_INDICATOR == 1){
				STRING_LEN = strlen(DANHSACH);
				LCD_SET_CURSOR(1,32);
				LCD_SCROLL_1LINE(DANHSACH,STRING_LEN);
				SHIFT_INDICATOR = 0;
			}
		}
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if (BUTTON_ENCODE == 0){} //do nothing
		else if(BUTTON_ENCODE == 1){ 
			DISPLAY_INDICATOR++;
			if(DISPLAY_INDICATOR>4){DISPLAY_INDICATOR = 0;}
			LCD_NEW_SLIDE(L);			
		}
		else if(BUTTON_ENCODE == -1){
			DISPLAY_INDICATOR--;
			if(DISPLAY_INDICATOR==255){DISPLAY_INDICATOR = 4;}
			LCD_NEW_SLIDE(R);
		}			
		else if (BUTTON_ENCODE == 2) {
			BUTTON_ENCODE = fn8051ButtenEncode();
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				break;
			}
			else {
				if (DISPLAY_INDICATOR == 0){
					fn8051SYS_RELAYSET_mode();
					break;
				}
				else if (DISPLAY_INDICATOR == 1){
					fn8051SYS_SAVONBOARD_mode();
					break;
				}
				else if (DISPLAY_INDICATOR == 2){
					fn8051SYS_TIMESET_mode();
					break;			
				}
				else if (DISPLAY_INDICATOR == 3){
					fn8051SYS_SETUP_MODE();
					break;
				}
				else if (DISPLAY_INDICATOR == 4){
					//fn8051SYS_SAVEDLIST_MODE();
				}					
			}
		}
	}
}
void _DISPLAY_FUNC_(unsigned char mode){			// LCd display Func
	unsigned char i=0;
	if(mode == 0){			// MENU MODE
		LCD_CLEARDISPLAY
		fn8051SYSDELAY(50);
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("DANG TAI");
		LCD_SET_CURSOR(1,0);
		LCD_DISPLAY_STRING("XIN CHO!");
		fn8051SYSDELAY(150);
		LCD_CLEARDISPLAY
		for (i = 0;i<5;i++){
			fn8051SYSDELAY(20);
			LCD_SET_CURSOR(0,(8*i)+2);
			LCD_DISPLAY_STRING("MENU");
			LCD_SET_CURSOR(0,8*i);
			LCD_DISPLAY_CHAR(0x7f);
			LCD_SET_CURSOR(0,(8*i)+7);
			LCD_DISPLAY_CHAR(0x7e);
		}
		LCD_SET_CURSOR(1,0);
		LCD_DISPLAY_STRING("On/Off");
		LCD_SET_CURSOR(1,8);
		LCD_DISPLAY_STRING("Timing");
		LCD_SET_CURSOR(1,16);
		LCD_DISPLAY_STRING("Setting");
		LCD_SET_CURSOR(1,24);
		LCD_DISPLAY_STRING("Mode");
		LCD_SET_CURSOR(1,32);
		LCD_DISPLAY_STRING("Menu");
	}
	else if (mode == 1){	//DISPLAY MODE
		LCD_CLEARDISPLAY
		fn8051SYSDELAY(50);
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("Clock:");
		LCD_SET_CURSOR(0,8);
		LCD_DISPLAY_STRING("Device:");
		LCD_SET_CURSOR(0,16);
		LCD_DISPLAY_STRING("Device:");
		LCD_SET_CURSOR(0,24);
		LCD_DISPLAY_STRING("Sensor:");
		LCD_SET_CURSOR(0,32);
		LCD_DISPLAY_STRING("Sensor:");
		LCD_SET_CURSOR(1,24);
		LCD_DISPLAY_STRING("1: ");
		LCD_SET_CURSOR(1,32);
		LCD_DISPLAY_STRING("2: ");
		LCD_SET_CURSOR(1,8);
		LCD_DISPLAY_STRING("1: ");
		LCD_SET_CURSOR(1,16);
		LCD_DISPLAY_STRING("2: ");
	}
	else if (mode == 2){	//SETUP MODE
		LCD_CLEARDISPLAY
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("Mode: ");
		LCD_SET_CURSOR(0,8);
		LCD_DISPLAY_STRING("Connect:");  //Using connect type 
		LCD_SET_CURSOR(0,16);
		LCD_DISPLAY_STRING("Address:");
	}
	else if (mode == 3){	//RELAYSET MODE
		LCD_CLEARDISPLAY
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("Device:");
		LCD_SET_CURSOR(1,1);
		LCD_DISPLAY_CHAR(':');
	}
	else if (mode == 4){	//Saveonboard MODE
		LCD_CLEARDISPLAY
		fn8051SYSDELAY(50);
		LCD_SET_CURSOR(0,1);
		LCD_DISPLAY_STRING("Timing:");
		LCD_SET_CURSOR(1,1);
		LCD_DISPLAY_STRING("CoutDown");
		LCD_SET_CURSOR(0,8);
		LCD_DISPLAY_STRING("Device");
		LCD_SET_CURSOR(0,16);
		LCD_DISPLAY_STRING("HH:MM");
		LCD_SET_CURSOR(1,19);
		LCD_DISPLAY_CHAR(':');
		LCD_SET_CURSOR(1,17);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,18);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,20);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,21);
		LCD_DISPLAY_NUMBER(0);
	}
	else if (mode ==5){
		LCD_CLEARDISPLAY
		fn8051SYSDELAY(50);
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("HH:MM");
		LCD_SET_CURSOR(1,3);
		LCD_DISPLAY_CHAR(':');
		LCD_SET_CURSOR(1,1);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,2);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,5);
		LCD_DISPLAY_NUMBER(0);
		LCD_SET_CURSOR(1,6);
		LCD_DISPLAY_NUMBER(0);
	}
}
void fn8051SYS_SETUP_MODE(){					// Set the working mod for your Device (online or offline)
	signed char BUTTON_ENCODE;
	unsigned char DISPLAY_INDICATOR = 0,buffervalue = 0,HOLD_DT = 0;
	bit USING_MODE =0;
	_DISPLAY_FUNC_(2);
	while(vrbGlb8051SYSMainFlag == 0){
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if(DISPLAY_INDICATOR==0){
			if( BUTTON_ENCODE ==1||BUTTON_ENCODE==-1){
				USING_MODE^=1;
			}
			LCD_SET_CURSOR(1,0);
			if(USING_MODE == 1){
				LCD_DISPLAY_STRING("ONLINE");
			}
			else if(USING_MODE == 0){
				LCD_DISPLAY_STRING("OFFLINE");
			}
		}
		if(USING_MODE == 0){vrcGlbSYSMODULETYPE = OFFLINE;}
		fn8051_BUZZER_(1);
		if(DISPLAY_INDICATOR== 1 && USING_MODE == OFFLINE){
			if( BUTTON_ENCODE == 1||BUTTON_ENCODE == -1){
				buffervalue += BUTTON_ENCODE;
				if(buffervalue>3) {buffervalue = 3;}
				else if (buffervalue==255){buffervalue = 1;}
			}
			LCD_SET_CURSOR(1,8);
			if(buffervalue == 1){LCD_DISPLAY_STRING("WIFI");}			//using nodeMcu
			else if(buffervalue == 2){LCD_DISPLAY_STRING("SIM");}			//using Sim8A
			else if(buffervalue == 3){LCD_DISPLAY_STRING("LORA");}	//using Lora (UART)
			vrcGlbSYSMODULETYPE = buffervalue;
		}
		buffervalue =0;
		fn8051_BUZZER_(1);
		if(DISPLAY_INDICATOR == 2 && USING_MODE == OFFLINE){
			buffervalue = 100;
			if( BUTTON_ENCODE == 1||BUTTON_ENCODE == -1){
				buffervalue += BUTTON_ENCODE;
				if(buffervalue>100) {buffervalue = 0;}
				else if (buffervalue==255){buffervalue = 99;}
			}
			LCD_SET_CURSOR(1,16);
			if(buffervalue == 100){LCD_DISPLAY_STRING("randomIP");}	
			else fn8051UARTSendNumber(buffervalue,1);
			vrcGlbSYSMYAddress = buffervalue;
		}
		fn8051_BUZZER_(1);
		if (BUTTON_ENCODE == 2) {
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				break;
			}
			else {
				DISPLAY_INDICATOR ++;
				if(DISPLAY_INDICATOR > 3) { DISPLAY_INDICATOR = 0;}
				HOLD_DT = 0;	
			}
		}
		// if(buffervalue == 0){
			// fn8051uartsendstring("s:");
			// fn8051uartsendnumber(0,0);                                                                                                                                        
			// fn8051uartsendstring(";");
			// fn8051uartsendstring("join!$");
		// }
	}
	if( vrcGlbSYSMODULETYPE != 0 ) {
		LCD_SET_CURSOR(0,0);
		LCD_DISPLAY_STRING("RESET");
		LCD_SET_CURSOR(1,0);
		LCD_DISPLAY_STRING("After: ");
		LCD_SET_CURSOR(1,6);
		LCD_DISPLAY_CHAR('s');
		for(DISPLAY_INDICATOR = 6;DISPLAY_INDICATOR<0;DISPLAY_INDICATOR--){
			LCD_SET_CURSOR(1,5);
			LCD_DISPLAY_NUMBER(DISPLAY_INDICATOR-1);
			fn8051SYSDELAY(1000);
		}
		set_SWRST;
	}
}
void fn8051SYS_RELAYSET_mode(){					//Relay Switching
	unsigned char HOLD_DT=0,RELAYNUM = 0;
	signed char BUTTON_ENCODE;
	bit RELAYSTATUS;
	_DISPLAY_FUNC_(3);
	while(vrbGlb8051SYSMainFlag == 0){
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if( BUTTON_ENCODE == 0){}
		else if ((BUTTON_ENCODE == 1)||(BUTTON_ENCODE == -1)){
			RELAYNUM += BUTTON_ENCODE;
			if(RELAYNUM == 2){RELAYNUM =0;}
			else if (RELAYNUM == 255){RELAYNUM =1;}
		}
		else if (BUTTON_ENCODE == 2){
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				break;
			}
			else{
				if( RELAYNUM == 0) RELAYSTATUS = pnoGlb8051Relay1;
				else if ( RELAYNUM == 1) RELAYSTATUS = pnoGlb8051Relay2;
				RELAYSTATUS ^= 1;
			}
		}
		LCD_SET_CURSOR(1,0);
		LCD_DISPLAY_NUMBER(RELAYNUM+1);
		LCD_SET_CURSOR(1,5);
		if(RELAYSTATUS == 1)  LCD_DISPLAY_STRING ("ON");
		else if (RELAYSTATUS == 0)  LCD_DISPLAY_STRING ("OFF");	
		if( RELAYNUM == 0)  pnoGlb8051Relay1 = RELAYSTATUS;
		else if ( RELAYNUM == 1)  pnoGlb8051Relay2 = RELAYSTATUS;
	}
}
void fn8051SYS_SAVONBOARD_mode(){			//Timing Save
	unsigned char RELAYNUM = 0,buttonsum = 0,DISPLAY_INDICATOR = 0,BUFFER = 0,HOLD_DT =0;
	signed char BUTTON_ENCODE;
	unsigned int ENCODEDTIME = 0;
	bit RELAYSTATUS = 0,SAVEMODE = 0;
	_DISPLAY_FUNC_(4);
	while(vrbGlb8051SYSMainFlag == 0){
		if(DISPLAY_INDICATOR == 0){
			SAVEMODE = BUFFER%2	?1:0;
			if (SAVEMODE == 0){
				LCD_SET_CURSOR(0,0);
				LCD_DISPLAY_CHAR(0x7e);
				LCD_SET_CURSOR(1,0);
				LCD_DISPLAY_CHAR(' ');
			}
			else{
				LCD_SET_CURSOR(0,0);
				LCD_DISPLAY_CHAR(' ');
				LCD_SET_CURSOR(1,0);
				LCD_DISPLAY_CHAR(0x7e);
			}
		}
		else if (DISPLAY_INDICATOR == 1){
			LCD_SET_CURSOR(1,8);
			LCD_DISPLAY_CHAR(0x7e);
			LCD_DISPLAY_NUMBER(BUFFER);
			RELAYNUM = BUFFER;
		}
		else if (DISPLAY_INDICATOR == 2){
			LCD_SET_CURSOR(1,12);
			LCD_DISPLAY_CHAR(0x7e);
			RELAYSTATUS = BUFFER%2 ?1:0;
			if(RELAYSTATUS == 1){LCD_DISPLAY_STRING("ON");}
			else LCD_DISPLAY_STRING("OFF");
		}
		else if ( DISPLAY_INDICATOR == 3){
			LCD_CURSOR_ON
			LCD_CURSOR_BLINK
			LCD_SET_CURSOR(1,17);
			if(BUFFER>2) BUFFER = 2;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			ENCODEDTIME = (unsigned int) BUFFER*600;
		}
		else if ( DISPLAY_INDICATOR == 4){
			LCD_SET_CURSOR(1,18);
			if(BUFFER>9) BUFFER = 9;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			ENCODEDTIME += (unsigned int) BUFFER*60;
		}
		else if ( DISPLAY_INDICATOR == 5){
			LCD_SET_CURSOR(1,20);
			if(BUFFER>6) BUFFER = 6;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			ENCODEDTIME += BUFFER*10;
		}
		else if ( DISPLAY_INDICATOR == 6){
			LCD_SET_CURSOR(1,21);
			if(BUFFER>9) BUFFER = 9;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			ENCODEDTIME += BUFFER;
		}
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if(BUTTON_ENCODE == 0){}
		else if(BUTTON_ENCODE == 1){
			BUFFER++;
		}
		else if(BUTTON_ENCODE == -1){
			BUFFER--;
		}
		else if(BUTTON_ENCODE==2){
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				break;
			}
			else{
				HOLD_DT =0;
				DISPLAY_INDICATOR++;
				BUFFER = 0;
				if((DISPLAY_INDICATOR == 1)||(DISPLAY_INDICATOR == 3)){
					LCD_NEXT_SCREEN(1);
				}
				else if (DISPLAY_INDICATOR == 7){
					if(SAVEMODE == 1) {
						fn8051DS1307ReadTime();
						ENCODEDTIME += ((unsigned int) vrcGlb8051DS1307Hour*60 + vrcGlb8051DS1307Min);
					}
					fn8051SYSTimeEncode(ENCODEDTIME,RELAYSTATUS,RELAYNUM);
					DISPLAY_INDICATOR = 0;
					LCD_CURSOR_OFF
					LCD_CLEARDISPLAY
					LCD_SET_CURSOR(0,0);
					LCD_DISPLAY_STRING("SAVED!");
					fn8051_BUZZER_(1);
					fn8051SYSDELAY(50);
					_DISPLAY_FUNC_(4);
				}
			}
		}
	}	
}
void fn8051SYS_TIMESET_mode(){				// Clock Setting
	unsigned char HOUR= 0,MINUTE=0,HOLD_DT=0,DISPLAY_INDICATOR=0,BUFFER =0;
	signed char BUTTON_ENCODE =0;
	_DISPLAY_FUNC_(5);
	LCD_CURSOR_ON
	LCD_CURSOR_BLINK
	while(vrbGlb8051SYSMainFlag == 0){
		if ( DISPLAY_INDICATOR == 0){
			LCD_SET_CURSOR(1,1);
			if(BUFFER>2) BUFFER = 2;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			HOUR = BUFFER*10;
		}
		else if ( DISPLAY_INDICATOR == 1){
			LCD_SET_CURSOR(1,2);
			if(BUFFER>9) BUFFER = 9;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			HOUR += BUFFER;
		}
		else if ( DISPLAY_INDICATOR == 2){
			LCD_SET_CURSOR(1,5);
			if(BUFFER>6) BUFFER = 6;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			MINUTE = BUFFER*10;
		}
		else if ( DISPLAY_INDICATOR == 3){
			LCD_SET_CURSOR(1,6);
			if(BUFFER>9) BUFFER = 9;
			else if(BUFFER == 255) BUFFER = 0;
			LCD_DISPLAY_NUMBER(BUFFER);
			LCD_CURSOR_MOVE_LEFT
			MINUTE += BUFFER;
		}
		BUTTON_ENCODE = 0;
		BUTTON_ENCODE = fn8051ButtenEncode();
		if(BUTTON_ENCODE == 0){}
		else if(BUTTON_ENCODE == 1){
			BUFFER++;
		}
		else if(BUTTON_ENCODE == -1){
			BUFFER--;
		}
		else if(BUTTON_ENCODE==2){
			while(BUTTON_ENCODE == 2 && HOLD_DT < 10){
				HOLD_DT++;
				BUTTON_ENCODE =0;
				BUTTON_ENCODE = fn8051ButtenEncode();
			}
			if(HOLD_DT > 8){
				LCD_CURSOR_OFF
				break;
			}
			else{
				HOLD_DT =0;
				DISPLAY_INDICATOR++;
				BUFFER = 0;
				if (DISPLAY_INDICATOR == 4){
					LCD_CURSOR_OFF
					LCD_CLEARDISPLAY
					LCD_SET_CURSOR(0,0);
					LCD_DISPLAY_STRING("Saved!");
					vrcGlb8051DS1307Hour = HOUR;
					vrcGlb8051DS1307Min = MINUTE;
					fn8051DS1307WriteTime();
					fn8051_BUZZER_(1);
					break;
				}
			}
		}
	}	
} 
void fn8051DS1307ReadTime(){
	vrcGlb8051DS1307Sec  = fn8051DS1307Readdata(SEC);
	vrcGlb8051DS1307Min  = fn8051DS1307Readdata(MIN);
	vrcGlb8051DS1307Hour = fn8051DS1307Readdata(HOUR);
}
void fn8051DS1307WriteTime(){
	fn8051DS1307Writedata(SEC,vrcGlb8051DS1307Sec);
	fn8051DS1307Writedata(MIN,vrcGlb8051DS1307Min);
	fn8051DS1307Writedata(HOUR,vrcGlb8051DS1307Hour);
}
void fn8051UARTSendChar (unsigned char BUFFER){
	SBUF = BUFFER ;
	while(TI==0){};
}

void fn8051UARTSendString(char* StringToSend){
	unsigned char LengthOfString,i;
	LengthOfString = strlen(StringToSend);
	for(i=0;i<=LengthOfString;i++){
		fn8051UARTSendChar(*(StringToSend+i));
	}
}
void fn8051SYSRESETALLFLAG(){
	free(vrcGlb8051UARTPointer);
	INDEX = 0;
	vrbGlb8051SYSMainFlag = 0;
	vrbGlb8051SIMFLAG = 0;
	EA = 1;
}
void fn8051UARTSendNumber(unsigned long int Number,bit mode){	
	unsigned char i =0,TEMP;
	if (Number ==0&&mode == 0){
		if(mode == 0){fn8051UARTSendChar('0');}
		else LCD_DISPLAY_NUMBER(0);
	}
	else if (Number <10 && mode == 1){
		LCD_DISPLAY_NUMBER(0);
		LCD_DISPLAY_NUMBER(Number);
	}
	else{
		while(Number>=pow(10,i)){
			i++;
		}
		i = i-1;
		for(i=i;i>0;i--){
			TEMP = Number/pow(10,i);
			Number = Number - (unsigned long int) (TEMP*pow(10,i));
			if(mode == 0){fn8051UARTSendChar((unsigned char)(TEMP+'0'));}
			else LCD_DISPLAY_NUMBER(TEMP);
		}
		if(mode == 0){fn8051UARTSendChar((unsigned char)(Number+'0'));}
		else LCD_DISPLAY_NUMBER(Number);
	}
}
void fn8051UARTSendFLOAT(float Number,bit mode){ 			
	unsigned char Nguyen,Tphan;
	
	Nguyen = (unsigned char)Number ;
	Tphan = (unsigned char) ((Number - Nguyen)*10);
	if(mode == 0){
		fn8051UARTSendNumber(Nguyen,0);
		fn8051UARTSendChar('.');
		fn8051UARTSendNumber(Tphan,0);
	}
	else {
		LCD_DISPLAY_NUMBER(Nguyen);
		LCD_DISPLAY_CHAR('.');
		LCD_DISPLAY_NUMBER(Tphan);
	}	
}
unsigned long int pow(char number,char times){ 
	unsigned char i=0;
	unsigned long int pownumber =1;
	for(i=0;i<times;i++){
		pownumber = pownumber*number;
	}
	return  pownumber;
}
void fn8051_BUZZER_(unsigned char mode){
	if (mode == 0){				
		pnoGlb8051BUZZER = 0;
		fn8051SYSDELAY(10);
		pnoGlb8051BUZZER = 1;
		fn8051SYSDELAY(10);
		pnoGlb8051BUZZER = 0;
		fn8051SYSDELAY(10);
		pnoGlb8051BUZZER = 1;
	}
	else if (mode == 1){
		pnoGlb8051BUZZER = 0;
		fn8051SYSDELAY(30);
		pnoGlb8051BUZZER = 1;
	}		
}
void fn8051SYSTimeEncode(unsigned int TimeSet,unsigned char RelayStatus,unsigned char RelayNum){
	unsigned char Hour,Min;
	if(TimeSet>1439){
		TimeSet-=1439;
	}
	Hour = TimeSet / 60;
	Min = TimeSet%60;
	if(RelayNum==1){
		if(RelayStatus == 1){Min|=0x40;}
		else if (RelayStatus == 0) {Min|=0x00;}
		fn8051EPROMWrite(VrcGlbAPROMRelayADR1,Hour,1);
		VrcGlbAPROMRelayADR1++;
		fn8051EPROMWrite(VrcGlbAPROMRelayADR1,Min,1);
		VrcGlbAPROMRelayADR1++;
		if(VrcGlbAPROMRelayADR1 >= 100){VrcGlbAPROMRelayADR1 = 98;}
	}
	else if(RelayNum == 2){
		if(RelayStatus == 1)	   {Min|=0x40;}
		else if (RelayStatus == 0) {Min|=0x00;}
		fn8051EPROMWrite(VrcGlbAPROMRelayADR2,Hour,0);
		VrcGlbAPROMRelayADR2++;
		fn8051EPROMWrite(VrcGlbAPROMRelayADR2,Min,0);
		VrcGlbAPROMRelayADR2++;
		if(VrcGlbAPROMRelayADR2 >= 100){VrcGlbAPROMRelayADR2 = 98;}
	}
}
void fn8051SYSRelayTimingCheck(unsigned char i){
	
	if (vrcGlb8051DS1307Hour == Vruca1GlbTimerRelay1[i] ){
		if(vrcGlb8051DS1307Min == (Vruca1GlbTimerRelay1[i+1]&0x3f)){
			pnoGlb8051Relay1 = Vruca1GlbTimerRelay1[i+1]&0x40 ?1:0;
		}
	}
	if (vrcGlb8051DS1307Hour == Vruca1GlbTimerRelay2[i] ){
		if(vrcGlb8051DS1307Min == (Vruca1GlbTimerRelay2[i+1]&0x3f)){
			pnoGlb8051Relay2 = Vruca1GlbTimerRelay2[i+1]&0x40 ?1:0;
		}
	}
}	
void fn8051EPROMWrite (unsigned int vri8051APROMADDR,unsigned char vruc8051DATA, bit vrb8051ARPORMSAVEPAGE){
	unsigned int vri8051APROMPAGEADR;
	if(vrb8051ARPORMSAVEPAGE == 1) { 
		vri8051APROMPAGEADR = 0x3E80;			
	}
	else if(vrb8051ARPORMSAVEPAGE == 0) {
		vri8051APROMPAGEADR = 0x3F00;			
	}
	EA = 0;
	TA = 0xAA;
	TA = 0x55;	
	CHPCON |= 0x01;	
	TA = 0xAA;
	TA = 0x55;
	IAPUEN |= 0x01;										
	IAPCN = 0x21;	
	IAPAL = (vri8051APROMPAGEADR&0xff)+vri8051APROMADDR;
	IAPAH = (vri8051APROMPAGEADR>>8)&0xff;
	IAPFD = vruc8051DATA;
	TA = 0xAA;
	TA = 0x55;
	IAPTRG |= 0x01;			
	TA = 0xAA;
	TA = 0x55;
	IAPUEN &= ~0x01;
	TA = 0xAA;
	TA = 0x55;
	CHPCON &= ~0x01;
	EA = 1;
}
void fn8051SIMINIT(){
	if(vrcGlbSYSMODULETYPE == SIM){
		fn8051UARTSendString("ATE0\r");
		fn8051SYSDELAY(1000);
		fn8051UARTSendString("AT+IPR=9600\r");  
		fn8051SYSDELAY(1000);
		fn8051UARTSendString("AT+CMGF=1\r"); 
		fn8051SYSDELAY(1000);
		fn8051UARTSendString("AT+CNMI=2,2,0,0,0\r");   
	}		      
}
void fn8051SIMSENDSTRING(){
	if(vrcGlbSYSMODULETYPE == SIM){
		fn8051UARTSendString("AT+CMGS=\""); 
		fn8051UARTSendString(PHONENUMBER);
		fn8051UARTSendString("\"");
		while(vrbGlb8051SIMFLAG == 0){}														
	}		
}
void fn8051SIMSENDSMS (){
	if(vrcGlbSYSMODULETYPE == SIM){fn8051UARTSendChar(0x1a);}		
}
signed char fn8051ButtenEncode(){
	int ADCbuttonvalue;
	ADCbuttonvalue = (int) fn8051ADCREAD(4,1);
	fn8051SYSDELAY(20);
	if(ADCbuttonvalue >= 0 && ADCbuttonvalue < 20){
		return -1;
	}
	else if(ADCbuttonvalue > 2200 && ADCbuttonvalue < 2350){
		return 2;	
	}
	else if(ADCbuttonvalue > 2800 && ADCbuttonvalue < 3050 ){
		return 1;
	}
	else{
		return 0;
	}
}