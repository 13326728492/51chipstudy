#include <reg52.h>

void Timer_Init()
{
	TMOD = 0x01;
	TH0 = 0xDC;
	TL0 = 0x18;
	ET0 = 1;
	TR0 =1;
	EA = 1;
	
}