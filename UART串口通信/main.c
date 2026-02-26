#include <reg52.h>
#include "Delay.h"
#include "UART.h"

//unsigned char Sec=1;

//void main()
//{
//	UART_Init();
//	while(1)
//	{
//		UART_SendByte(Sec);
//		Sec++;
//		Delay(1000);
//	}
//}

void main()
{
	UART_Init();
	while(1)
	{
		
	}
}

void UART_Routine() interrupt 4
{
	if(RI==1)
	{
		P2=~SBUF;
		UART_SendByte(SBUF);
		RI=0;
	}
}