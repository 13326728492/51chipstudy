#ifndef __LCD12864_H__
#define __LCD12864_H__

void LCD_Write_Cmd(unsigned char cmd);
void LCD_Write_Data(unsigned char dat);
unsigned char LCD_Read_Data(void);
void LCD_Init();
void LCD_Enter_GDRAM_Mode();
void LCD_Enter_Text_Mode();
void LCD_Text_Mode_Clear(void);
void LCD_DrawPixel(unsigned char x,unsigned char y);
void LCD_Draw_Vertical_Line(unsigned char x, unsigned char y1,unsigned char y2);
void LCD_GDRAM_MODE_CLEAR();
void LCD_String(unsigned char x,unsigned char y,const char* str);

#endif