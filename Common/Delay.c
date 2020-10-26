/*---------------------------------------------------------------------------------------------------------*/
/*                                                                                                         */
/* Copyright(c) 2016 Nuvoton Technology Corp. All rights reserved.                                         */
/*                                                                                                         */
/*---------------------------------------------------------------------------------------------------------*/

//***********************************************************************************************************
//  Nuvoton Technoledge Corp. 
//  Website: http://www.nuvoton.com
//  E-Mail : MicroC-8bit@nuvoton.com
//  Date   : Apr/21/2016
//***********************************************************************************************************

#include "N76E003.h"
#include "Common.h"
#include "Delay.h"
#include "SFR_Macro.h"
#include "Function_define.h"
bit BIT_TMP;

//-------------------------------------------------------------------------

//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void Timer1_Delay10ms(UINT32 u32CNT)
{
    clr_T1M;																		//T1M=0, Timer1 Clock = Fsys/12
    TMOD |= 0x10;																//Timer1 is 16-bit mode
    set_TR1;																		//Start Timer1
    while (u32CNT != 0)
    {
     
   
        while (TF1 != 1);												//Check Timer1 Time-Out Flag
        clr_TF1;
        u32CNT --;
    }
    clr_TR1;                               			//Stop Timer1
}
//------------------------------------------------------------------------------
void Timer2_Delay500us(UINT32 u32CNT)
{
    clr_T2DIV2;																	//Timer2 Clock = Fsys/4 
    clr_T2DIV1;
    set_T2DIV0;
    set_TR2;                                		//Start Timer2
    while (u32CNT != 0)
    {

        while (TF2 != 1);                   		//Check Timer2 Time-Out Flag
        clr_TF2;
        u32CNT --;
    }
    clr_TR2;                                		//Stop Timer2
}
//------------------------------------------------------------------------------
void Timer3_Delay100ms(UINT32 u32CNT)
{
    T3CON = 0x07;                           		//Timer3 Clock = Fsys/128
    set_TR3;                                		//Trigger Timer3
    while (u32CNT != 0)
    {
        RL3 = LOBYTE(TIMER_DIV128_VALUE_100ms); //Find  define in "Function_define.h" "TIMER VALUE"
      
     	//Check Timer3 Time-Out Flag
        clr_TF3;
        u32CNT --;
    }
    clr_TR3;                                		//Stop Timer3
}
//------------------------------------------------------------------------------
void Timer3_Delay10us(UINT32 u32CNT)
{
    T3CON = 0x07;                           		//Timer3 Clock = Fsys/128
    set_TR3;                                		//Trigger Timer3
    while (u32CNT != 0)
    {
        RL3 = LOBYTE(TIMER_DIV4_VALUE_10us); //Find  define in "Function_define.h" "TIMER VALUE"
        
     //Check Timer3 Time-Out Flag
        clr_TF3;
        u32CNT --;
    }
    clr_TR3;                                		//Stop Timer3
}
