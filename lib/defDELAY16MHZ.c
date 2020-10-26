#include "defDELAY16MHZ.h"

void fn8051SYSDELAY(unsigned int time){
	unsigned int counter,counter1;
	for(counter1=0;counter1<time;counter1++){
		for(counter=0;counter<16000;counter++){
		}
	}
}
void fn8051SYSDELAYUS(unsigned int time){
	unsigned int counter,counter1;
	for(counter1=0;counter1<time;counter1++){
		for(counter=0;counter<16;counter++){
		}
	}
}