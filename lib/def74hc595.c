#include "def74hc595.h"
#include "N76E003.h"
#include "port.h"
#define COMMONANODE 
#ifdef COMMONANODE					// duong chung + dk led  = 0 thi sang
	const bit ledon  = 0;
	const bit ledoff = 1;
	const NUMBER[10]={0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};
#else		// am chung
	const bit ledon  = 0;
	const bit ledoff = 1;
	const NUMBER[10]={0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f};
#endif
unsigned char BYTE[4];
void fn80517segled_DISPLAY_(unsigned char *INDEX,bit BLINKINDEX,unsigned char BLINKINDICATOR ){
	unsigned char i=0;
	bit buffer=0;
	pnobl80517segledCtr_L1=pnobl80517segledCtr_L2=pnobl80517segledCtr_L3=pnobl80517segledCtr_L4=ledoff;
	if(BLINKINDICATOR == 0xff){
		pnobl80517segledCtr_L1=pnobl80517segledCtr_L2=pnobl80517segledCtr_L3=pnobl80517segledCtr_L4=ledoff;
	}
	else{
		if(BLINKINDEX == 0){
			 fn80517segledCTRLLED(INDEX);
		}
		else if (BLINKINDEX == 1){
			BLINKINDICATOR &= (0x01<<((*INDEX)-1));    
			if(BLINKINDICATOR == 1) {
				switch ((*INDEX)){
					case 1: pnobl80517segledCtr_L1 = ledoff;break;
					case 2: pnobl80517segledCtr_L2 = ledoff;break;
					case 3: pnobl80517segledCtr_L3 = ledoff;break;
					case 4: pnobl80517segledCtr_L4 = ledoff;break;
				}
			}
			else if (BLINKINDICATOR == 0){
				fn80517segledCTRLLED(INDEX);
			}
		}
	}
}
void fn80517segledCTRLLED(unsigned char *INDEX){
		if(*INDEX == 1 ){		
			fn80517segledSENDDT74hc595(BYTE[1]);
			pnobl80517segledCtr_L1 = ledon;
			pnobl80517segledCtr_L2 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L4 = ledoff;
		}
		else if(*INDEX == 2 ){
			fn80517segledSENDDT74hc595(BYTE[2]);
			pnobl80517segledCtr_L2 = ledon;
			pnobl80517segledCtr_L1 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L4 = ledoff;
		}
		else if(*INDEX == 3 ){
			fn80517segledSENDDT74hc595(BYTE[3]);
			pnobl80517segledCtr_L3 = ledon;		
			pnobl80517segledCtr_L1 = pnobl80517segledCtr_L2 = pnobl80517segledCtr_L4 = ledoff;
		}
		else if(*INDEX == 4 ){
			fn80517segledSENDDT74hc595(BYTE[4]);
			pnobl80517segledCtr_L4 = ledon;
			pnobl80517segledCtr_L1 = pnobl80517segledCtr_L3 = pnobl80517segledCtr_L2 = ledoff;
		}
}
void fn80517segledSENDDT74hc595(unsigned char bytedata){ 
	unsigned char i,Q;
	Q = bytedata;
	for(i=0;i<8;i++){
		pnoGlb80517segmentSDI = Q & (0x80>>i);
		pnoGlb80517segmentCLK = 0;
		pnoGlb80517segmentCLK = 1;
	} 
	pnoGlb80517segmentSTR = 0; 
	pnoGlb80517segmentSTR = 1;
}
void fn80517segled_DATA_FLOAT(unsigned char BYTE1, unsigned char BYTE2, float *floatnumber){
	unsigned char nguyen,tphan1,Q;
	nguyen = (unsigned char) *floatnumber;
	tphan1 = (unsigned char) (*floatnumber*10- nguyen*10);
	Q = NUMBER[nguyen];
	#ifdef COMMONANODE		
		Q &= 0x7f;	// duong chung
	#else		// am chung
		Q |= 0x80;
	#endif
	BYTE[1] = NUMBER[BYTE1];
	BYTE[2] = BYTE2;
	BYTE[3] = Q;
	BYTE[4] = NUMBER[tphan1];
}
void fn80517segled_DATA_RELAY(unsigned char RELAYNUM, unsigned char STATUS){
	unsigned char Q;
	Q = NUMBER[RELAYNUM];
	#ifdef COMMONANODE	
		Q &= 0x7f;
		if(STATUS){
			BYTE[1] = Q;
			BYTE[2] = 0xff;
			BYTE[3] = 0xa3;
			BYTE[4] = 0x2B;
		}
		else{
			BYTE[1] = Q;
			BYTE[2] = 0xc0;
			BYTE[3] = 0x8e;
			BYTE[4] = 0x8e;
		}
	#else		// am chung
		Q |= 0x80;
		if(STATUS){
			BYTE[1] = Q;
			BYTE[2] = 0x00;
			BYTE[3] = 0x5c;
			BYTE[4] = 0xD4;
		}
		else{	
			BYTE[1] = Q;
			BYTE[2] = 0x3f;
			BYTE[3] = 0x71;
			BYTE[4] = 0x71;
		}
	#endif
}
void fn80517segled_DATA_INT(unsigned int* bytedata){
	unsigned char i, leddata[4];
	unsigned int heso = 1000,Q;
	Q = *bytedata;
	for(i=0;i<4;i++){
		leddata[i] = Q / heso;
		Q = Q - ((leddata[i])*heso);
		BYTE[i+1] = NUMBER[*(leddata+i)];
		heso = heso/10;
	}	
}
void fn80517segled_DATA_4BYTE(unsigned char data1, unsigned char data2, unsigned char data3 , unsigned char data4){
	BYTE[1] = data1;
	BYTE[2] = data2;
	BYTE[3] = data3;
	BYTE[4] = data4;
}
void fn80517segled_DATA_RTC (unsigned char *hour, unsigned char *minute){
	unsigned char hour1,hour2,minute3,minute4;
	if(*hour<10){
		hour1 = 0;
		hour2 = *hour;
	}
	else{
		hour1 = *hour/10;
		hour2 = *hour-(hour1*10);
	}
	if(*minute<10){
		minute4 = *minute;
		minute3 = 0;
	}
	else{
		minute3 = *minute/10;
		minute4 = *minute-(minute3*10);
	}
	#ifdef COMMONANODE		
		hour2 = NUMBER[hour2]& 0x7f;	// duong chung
	#else		// am chung
		hour2 = NUMBER[hour2]| 0x80;
	#endif
	BYTE[1] = NUMBER[hour1];
	BYTE[2] = hour2 ;
	BYTE[3] = NUMBER[minute3];
	BYTE[4] = NUMBER[minute4];
}
void fn80517segled_DATA_MODE (unsigned char * modenumber,bit mode){
	unsigned char Q;
	Q = (unsigned char)*modenumber/ 10;
	BYTE[3] = NUMBER[Q];
	Q = *modenumber - (Q*10);
	BYTE[4] = NUMBER[Q];
	if (mode == 0){
		BYTE[1] = 0x12;
		BYTE[2] = 0x47;
	}
	if (mode == 1){
		BYTE[1] = 0xF9;
		BYTE[2] = 0x0C;
	}
}

















