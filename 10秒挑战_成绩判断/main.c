#include "AT24C02.h"
#include "Delay.h"
#include "Key.h"
#include "LCD1602.h"
#include "Timer0.h"
#include "Nixie.h"
#include <reg52.h>
unsigned char KeyNum;
unsigned char RunFlag;
unsigned char Sec,Min;
unsigned long Sum;
unsigned int MSec;

sbit P2_0=P2^0;
sbit P2_1=P2^1;


void main()
{
	LCD_Init();
	LCD_ShowString(1,1,"JUDGE_SYSTEM");
	Timer0_Init();
	while(1)
	{
		KeyNum = Key();
		if(KeyNum == 1)								//开始计时与停止计时
		{
			RunFlag=!RunFlag;
		}
		if(KeyNum == 2)               //清0
		{
			Sec=0;
			Min=0;
			MSec=0;
			
		}
		if(KeyNum == 3)               //读取相应结果
		{
			Min=AT24C02_ReadByte(0);
			Sec=AT24C02_ReadByte(1);
			MSec=AT24C02_ReadByte(2);
		}
		if(KeyNum == 4)               //存储相应结果
		{
			AT24C02_WriteByte(0,Min);
			Delay(5);
			AT24C02_WriteByte(1,Sec);
			Delay(5);
			AT24C02_WriteByte(2,MSec);
			Delay(5);
		}
		if(KeyNum == 5)               //成绩判断
		{
			Sum=Min*60*1000+Sec*1000+MSec;
			if(Sum<=9500)
			{
				
				P2_1=1;
				P2_0=0;
			}
			else if(Sum<=10500&&Sum>9500)
			{
				
				P2_0=1;
				P2_1=0;
			}
			else if(Sum<=11500&&Sum>10500)
			{
				
				P2_0=0;
				P2_1=0;
			}
			else
			{
				P2_0=1;
				P2_1=1;
			}
			
		}

		Nixie_SetBuf(1,Min%10);
		Nixie_SetBuf(2,11);
		Nixie_SetBuf(3,Sec/10);
		Nixie_SetBuf(4,Sec%10);
		Nixie_SetBuf(5,11);
		Nixie_SetBuf(6,MSec/100);
		Nixie_SetBuf(7,(MSec/10)%10);
		Nixie_SetBuf(8,MSec%10);
		
	}
	
	
}
void Sec_Loop(void)
{
	if(RunFlag)
	{
		MSec++;
		if(MSec>=1000)
		{
			MSec=0;
			Sec++;
			if(Sec>=60)
			{
				Sec=0;
				Min++;
				if(Min>=60)
				{
					Min=0;
				}
			}
		}
	}
}


void Timer0_Routine() interrupt 1
{
	static unsigned int T0Count2;
	TL0 = 0x18;
	TH0 = 0xFC;

	T0Count2++;
	if(T0Count2>=2)
	{
		T0Count2=0;
		Nixie_Scan_Loop();
	}
	

		Sec_Loop();

	
}
