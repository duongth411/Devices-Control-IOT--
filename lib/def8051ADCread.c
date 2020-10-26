#include "def8051ADCread.h"
#include "N76E003.h"
#include "Common.h"
#include "SFR_Macro.h"
#include "Function_define.h"
bit BIT_TMP;
void fn8051ADCBypass (void){ 
	unsigned char i;
	for (i=0;i<0x03;i++){
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0);
	}
}
float fn8051ADCRead_bandgap(){ //IAP read
	unsigned char BandgapHigh,BandgapLow,BandgapMark;	
	float Bandgap_Value,ADCBandgap_Voltage;
	set_IAPEN;
	IAPCN = 0x04;
	IAPAL = 0x0d;
	IAPAH = 0x00;
	set_IAPGO;
	BandgapLow  = IAPFD;
	BandgapMark = BandgapLow&0xF0;
		
	if (BandgapMark==0x80)
	{
		BandgapLow = BandgapLow&0x0F;
		IAPAL = 0x0C;
		IAPAH = 0x00;
		set_IAPGO;
		BandgapHigh = IAPFD;
		Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
		ADCBandgap_Voltage = Bandgap_Value*3/4;
	}
	if (BandgapMark==0x00)
	{
		BandgapLow = BandgapLow&0x0F;
		IAPAL = 0x0C;
		IAPAH = 0x00;
		set_IAPGO;
		BandgapHigh = IAPFD;
		Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
		ADCBandgap_Voltage= Bandgap_Value*3/4;
	}
	if (BandgapMark==0x90)
	{
		IAPAL = 0x0E;
		IAPAH = 0x00;
		set_IAPGO;
		BandgapHigh = IAPFD;
		IAPAL = 0x0F;
		IAPAH = 0x00;
		set_IAPGO;
		BandgapLow = IAPFD;
		BandgapLow = BandgapLow&0x0F;
		Bandgap_Value = (BandgapHigh<<4)+BandgapLow;
		ADCBandgap_Voltage= Bandgap_Value*3/4;
	}
	clr_IAPEN;
	return ADCBandgap_Voltage;
}
float fn8051ADCREAD(unsigned char ADCchannel,bit return_type){
	float bandgapvalue,VDD,VAIN,ADCvalue,VADC;
	if(return_type == 0){
		fn8051ADCRead_bandgap();
		ADCCON0 = 0x08;
		ADCCON1 = 0x01;
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0){};
		bandgapvalue = (ADCRH<<4) + ADCRL;
		VDD = (0xFFF/bandgapvalue)*fn8051ADCRead_bandgap();
		Disable_ADC;
		ADCCON0 = ADCchannel;
		ADCCON1 = 0x01;
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0){};
		ADCvalue = (ADCRH<<4) + ADCRL;
		VAIN = (float)ADCvalue/0x1000;
		VADC = (float)(VAIN*VDD);
		Disable_ADC;
		VADC = VADC/1000;
		return VADC;	
	}
	else{
		ADCCON0 = ADCchannel;
		ADCCON1 = 0x01;
		clr_ADCF;
		set_ADCS;
		while(ADCF == 0){};
		ADCvalue = (ADCRH<<4) + ADCRL;
		Disable_ADC;	
		return ADCvalue;	
	}
}