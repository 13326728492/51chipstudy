#include <reg52.h>
#include "Delay.h"
sbit P3_1=P3^1;
sbit P3_0=P3^0;
sbit P3_2=P3^2;
sbit P3_3=P3^3;
unsigned char Key_KeyNumber;

unsigned char Key(void)
{
	unsigned char Temp=0;
	Temp=Key_KeyNumber;
	Key_KeyNumber=0;
	return Temp;
}

unsigned char Key_GetState()
{
	unsigned char KeyNumber=0;
	
	if(P3_1==0){KeyNumber=1;}
	if(P3_0==0){KeyNumber=2;}
	if(P3_2==0){KeyNumber=3;}
	if(P3_3==0){KeyNumber=4;}
	
	return KeyNumber;
}

void Key_Loop(void)
{
	static unsigned char NowState,LastState;
	LastState=NowState;				
	NowState=Key_GetState();		
	
	if(LastState==1 && NowState==0)
	{
		Key_KeyNumber=1;
	}
	if(LastState==2 && NowState==0)
	{
		Key_KeyNumber=2;
	}
	if(LastState==3 && NowState==0)
	{
		Key_KeyNumber=3;
	}
	if(LastState==4 && NowState==0)
	{
		Key_KeyNumber=4;
	}
}