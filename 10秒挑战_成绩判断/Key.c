#include <reg52.h>
#include "Delay.h"
sbit P3_0=P3^0;
sbit P3_1=P3^1;
sbit P3_2=P3^2;
sbit P3_3=P3^3;
sbit P1_3=P1^3;
sbit P1_7=P1^7;

unsigned char Key()
{
	unsigned char KeyNumber=0;
	
	if(P3_0==0){Delay(20);while(P3_0==0);Delay(20);KeyNumber=1;}          //普通按键
	if(P3_1==0){Delay(20);while(P3_1==0);Delay(20);KeyNumber=2;}
	if(P3_2==0){Delay(20);while(P3_2==0);Delay(20);KeyNumber=3;}
	if(P3_3==0){Delay(20);while(P3_3==0);Delay(20);KeyNumber=4;}
	P1_3=0;
	if(P1_7==0){Delay(20);while(P1_7==0);Delay(20);KeyNumber=5;}         //使用矩阵键盘
	
	return KeyNumber;
}