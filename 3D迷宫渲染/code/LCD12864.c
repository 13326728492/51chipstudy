#include <intrins.h>
#include "Delay.h"
#include <reg52.h>
#define LCD_DATAPORT P1


sbit LCD_RS = P3^0;
sbit LCD_EN = P3^2;
sbit LCD_RW = P3^1;

//模式选择只针对START模式有效
//如果是continue则是选择上一个地图



volatile unsigned char keil_not_optimize_me;

//unsigned char checkbusy(){
//	unsigned char  temp;
//	LCD_DATAPORT = 0xFF;
//	LCD_RS =0;
//	LCD_RW = 1;
//	LCD_EN = 1;
//	delay(1);
//	temp = LCD_DATAPORT;
//	LCD_EN = 0;
//	return temp&0x80;
//}


void LCD_Write_Cmd(unsigned char cmd)
{
//	while(checkbusy())
	LCD_RS = 0;
	LCD_RW = 0;
	LCD_EN = 0;
	
	LCD_DATAPORT = cmd;
	LCD_EN = 1;

	LCD_EN = 0;


}


void LCD_Write_Data(unsigned char dat)
{
//	while(checkbusy());
	LCD_RS = 1;
	LCD_RW = 0;
	LCD_EN = 0;
	
	LCD_DATAPORT = dat;
	
	LCD_EN = 1;

	LCD_EN = 0;

	
}
unsigned char LCD_Read_Data(void)
{
	unsigned char Data = 0;
	LCD_DATAPORT = 0xFF;
	LCD_RS = 1;
	LCD_RW = 1;
	LCD_EN = 1;
	
	Data = LCD_DATAPORT;

	LCD_EN = 0;
	
	return Data;
}

void LCD_Init()
{
	delay(100);
	
	LCD_Write_Cmd(0x30);
	delay(10);
	
	LCD_Write_Cmd(0x30);
	delay(10);
	
	LCD_Write_Cmd(0x30);
	delay(10);
	LCD_Write_Cmd(0x30);  // ? ??:4??????
    delay(10);
	
	
	LCD_Write_Cmd(0x0c);
	delay(10);
	
	LCD_Write_Cmd(0x01);
	delay(10);
	LCD_Write_Cmd(0x06);
	delay(10);
	

}

void LCD_Enter_GDRAM_Mode()
{
	LCD_Write_Cmd(0x34);
	delay(1);
	LCD_Write_Cmd(0x36);
	delay(1);
	
}


void LCD_Enter_Text_Mode()
{
	LCD_Write_Cmd(0x30);
	delay(1);
	LCD_Write_Cmd(0x0c);
}




void LCD_Text_Mode_Clear(void)
{
	
    

	LCD_Write_Cmd(0x01);

	delay(10);
		LCD_Write_Cmd(0x80);
	delay(10);
	    
}

//void LCD_GDRAM_Mode_Clear(unsigned char* buf)
//{
//	unsigned char i;
//	for(i=0;i<1024;i++)
//	{
//		buf[i]= 0x00;
//	}
//	{ unsigned char dummy = 0; }
//}







void LCD_DrawPixel(unsigned char x,unsigned char y)
{
	
	unsigned char H,L;
	unsigned char pos;

	
	pos = 15 - (x%16);

	if(y<32)
	{
		LCD_Write_Cmd(0x80+y);
		_nop_();
		LCD_Write_Cmd(0x80+(x/16));
		_nop_();_nop_();
		LCD_Read_Data();
		H = LCD_Read_Data();
		L = LCD_Read_Data();
		if(pos >= 8)
		{
			H |= 1 << (pos-8);
		}
		else
		{
			L |= 1 << pos;
		}
		
		LCD_Write_Cmd(0x80+y);
		_nop_();
		LCD_Write_Cmd(0x80+(x/16));
		_nop_();
		LCD_Read_Data();
		LCD_Write_Data(H);
		LCD_Write_Data(L);
		_nop_();
	}
	else if(y>=32)
	{
		LCD_Write_Cmd(0x80+y-32);
		_nop_();
		LCD_Write_Cmd(0x88+(x/16));
		_nop_();
		LCD_Read_Data();
		H = LCD_Read_Data();
		L = LCD_Read_Data();
		
		if(pos >= 8)
		{
			H |= 1 << (pos -8);
		}
		else
		{
			L |= 1 << pos;
		}
		
		LCD_Write_Cmd(0x80+y-32);
		_nop_();
		LCD_Write_Cmd(0x88+(x/16));
		_nop_();
		LCD_Write_Data(H);
	
		LCD_Write_Data(L);
		_nop_();
		
	}
}


void LCD_Draw_Vertical_Line(unsigned char x, unsigned char y1,unsigned char y2){
	unsigned char y;
		
	if(y1>y2)
	{
		unsigned char temp = y1;
		y1 = y2;
		y2 = temp;
	}


	
	for(y = y1; y < ((y2+y1)/2) ; y++)
	{
		LCD_DrawPixel(x,y);
	}
	for(y= ((y2+y1)/2+1); y<=y2;y++)
	{
		LCD_DrawPixel(x,y);
	}
}

void LCD_GDRAM_MODE_CLEAR()
{
	unsigned char x,y;
	
	for(y=0;y<32;y++)
	{
		LCD_Write_Cmd(0x80 + y);
		LCD_Write_Cmd(0x80);
		for(x=0;x<16;x++)
		{
			LCD_Write_Data(0x00);
			LCD_Write_Data(0x00);
		}
	}
	
	for(y=0;y<32;y++)
	{
		LCD_Write_Cmd(0x80+y);
		LCD_Write_Cmd(0x88);
		for(x=0;x<16;x++)
		{
			LCD_Write_Data(0x00);
			LCD_Write_Data(0x00);
		}
	}
}

//???????????????????
//void LCD_render_to_screen(unsigned char* buf)
//{
//	unsigned char i,j;
//	for(i=0;i<32;i++)
//	{
//		LCD_Write_Cmd(0x80+i);
//		LCD_Write_Cmd(0x80);
//		for(j=0;j<16;j++)
//		{
//			LCD_Write_Data(*buf++);
//		}
//	}
//	for(i=0;i<32;i++)
//	{
//		LCD_Write_Cmd(0x80+i);
//		LCD_Write_Cmd(0x88);
//		for(j=0;j<16;j++)
//		{
//			LCD_Write_Data(*buf++);
//		}
//	}
//}

void LCD_String(unsigned char x,unsigned char y,const char* str)
{
	unsigned char i = 0;
	unsigned char addr;
	if(y == 0)
	{addr = 0x80;}
	if(y == 1)
	{addr =0x90;}
	if(y ==2)
	{addr = 0x88;}
	if(y == 3)
	{addr = 0x98;}
	addr = addr+x;
	LCD_Write_Cmd(addr);
	delay(2);
	while(str[i] != '\0')
	{
		
		LCD_Write_Data(str[i]);
		delay(2);
		i++;
	} 
}