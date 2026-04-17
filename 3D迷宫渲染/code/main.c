#include "LCD12864.h"
#include "Delay.h"
#include "AT24C02.h"
#include "Time.h"
#include <reg52.h>
#include "Goperate.h"

sbit P27 = P2^7;
sbit P33 = P3^3;
sbit P35 = P3^5;
sbit P34 = P3^4;
sbit P25 = P2^5;
sbit P24 = P2^4;

typedef enum{
	MENU,
	GAME,
	LEVEL_CHOICE
}State;

typedef enum{
	KEY_DOWN,
	KEY_OK,
	NONEMwo

}MenuKeyState;

typedef enum{
	KEY_FORWARD,
	KEY_BACK,
	KEY_TURN,
	KEY_BREAK,
	NONEG
}GameKeyState;




State now_state = MENU;

unsigned char menu_select = 0;
unsigned char level_state = 0;

//player digital
int px = 6;
int py = 6;
int dir = 0;

int dx[4]={2,0,-2,0};
int dy[4]={0,-2,0,2};



xdata unsigned char map1[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,1,1,0,0,0,1},
    {1,0,1,0,0,1,1,1},
    {1,0,1,0,1,1,2,1},
    {1,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1}
};
xdata unsigned char map2[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,0,1,1,0,0,0,1},
    {1,0,1,1,0,1,1,1},
    {1,0,0,0,0,1,0,1},
    {1,0,1,1,1,1,0,1},
    {1,0,0,0,1,0,0,1},
    {1,1,1,0,0,0,2,1},
    {1,1,1,1,1,1,1,1}
};

xdata unsigned char map[8][8] = {0};



//key
MenuKeyState menu_key_state;
GameKeyState game_key_state;



bit key_menu_switch_flag;
bit key_menu_select_flag;
bit key_game_forward_flag;
bit key_game_back_flag;
bit key_game_turn_flag;
bit key_game_break_flag;







void LCD_Clear_When_Switch_Mode(void);
void draw_menu(void);
void menuKey(void);
void key_game(void);
void key_clear_all_flags(void);
void judge_if_success(void);
void check_level_state(void);
void read_store_test(void);
void read_write_test(void);



void key_clear_all_flags(void)
{
	key_menu_switch_flag = 0;
	key_menu_select_flag = 0;
	key_game_forward_flag = 0;
	key_game_back_flag = 0;
	key_game_turn_flag = 0;
	key_game_break_flag = 0;
}





void LCD_Clear_When_Switch_Mode(void)
{
	if(now_state == MENU)
	{
		LCD_Text_Mode_Clear();
	}
	else
	{
		LCD_GDRAM_MODE_CLEAR();
	}
}

void draw_menu(void)
{
//	LCD_Text_Mode_Clear();
	
	LCD_String(0,0," START NEW     ");
	LCD_String(0,1," CHOICE        ");
	LCD_String(0,2," EXIT          ");
	LCD_String(0,3," CONTINUS      ");
	
	if(menu_select == 0)
	{
		LCD_String(0,0,">");
		LCD_String(0,3," ");
	}
	else if(menu_select == 1)
	{
		LCD_String(0,1,">");
		LCD_String(0,0," ");
	}
	else if(menu_select == 2)
	{
		LCD_String(0,2,">");
		LCD_String(0,1," ");
	}
	else if(menu_select == 3)
	{
		LCD_String(0,3,">");
		LCD_String(0,2," ");
	}
}

//void menuKey(void)
//{
//	if(menu_key_state == KEY_DOWN)
//	{
//		menu_select++;
//		if(menu_select>3)menu_select = 0;
//		draw_menu();
//		delay(100);
//	}
//	else if(menu_key_state == KEY_OK)
//	{
//		if(menu_select == 0)
//		{
//			if(menu_select ==0)
//			{
//				now_state = GAME;
//				//restart and init
//				px = 6;
//				py = 6;
//				dir = 0;
//			}
//		}
//		else if(menu_select == 1)
//		{
//			now_state = MENU;
//			draw_menu();
//		}
//		else if(menu_select == 2)
//		{
//			LCD_Text_Mode_Clear();
//			LCD_String(2,2,"Bye~");
//			while(1);
//		}
//		else if(menu_select == 3)
//		{
//			now_state = GAME;
//			
//			//load
//		}
//	}
//}

//void key_game(void)
//{
//	if(game_key_state == KEY_FORWARD)
//	{
////		move_forward();
//	}
//	else if(game_key_state == KEY_BACK)
//	{
////		move_back();
//	}
//	else if(game_key_state == KEY_TURN)
//	{
////		turn();
//	}
//	else if(game_key_state == KEY_BREAK)
//	{
//		now_state = MENU;
////		save_game();
//		LCD_Enter_Text_Mode();
//		LCD_Text_Mode_Clear();
//		LCD_String(0,0,"Save OK!");
//		delay(2000);
//	}
//}





void test_draw_menu()
{
	

	if(P27 == 0)
	{
		delay(190);
		menu_select++;
		if(menu_select==4){
			menu_select = 0;
		}
		draw_menu();
	}
	else if(P33 == 0)
	{
		delay(190);
		if(menu_select == 0)
		{
			now_state = GAME;
			LCD_Text_Mode_Clear();
			LCD_Enter_GDRAM_Mode();
			LCD_GDRAM_MODE_CLEAR();
			px = 6;
			py = 6;
			dir = 0;
		check_level_state();
		}
		if(menu_select == 1)
		{
			now_state = LEVEL_CHOICE;
			LCD_Text_Mode_Clear();
//			LCD_Text_Mode_Clear();
//			LCD_String(1,0," Easy");
//			LCD_String(1,1," Difficult");
//			levelJudge();
//			if(P24 == 0)
//			{
//				delay(190);
//				level_state++;
//				if(level_state >1 )level_state = 0;
//				
//			}
				now_state = LEVEL_CHOICE;
				LCD_Text_Mode_Clear();
		}
		if(menu_select == 2)
		{
			LCD_Text_Mode_Clear();
			LCD_String(2,2,"Bye~");
			delay(1000);
			LCD_Text_Mode_Clear();
			while(1);
		}
		if(menu_select == 3)
		{
			now_state = GAME;
			LCD_Text_Mode_Clear();
			LCD_Enter_GDRAM_Mode();
//			px = AT24C02_ReadByte(1);
//			py = AT24C02_ReadByte(2);
//			dir = AT24C02_ReadByte(3);
//			level_state = AT24C02_ReadByte(4);
//			check_level_state();
			read_write_test();
		}

		
	}
	
}





void testGameKey(void)
{
	if(P34 == 0)
	{
		delay(190);
		move_forward();
		LCD_GDRAM_MODE_CLEAR();
	}
	else if(P35 == 0)
	{
		delay(190);
		move_back();
		LCD_GDRAM_MODE_CLEAR();
	}
	else if(P24 == 0)
	{
		delay(190);
		turn();
		LCD_GDRAM_MODE_CLEAR();
	}
	else if(P25 == 0)
	{
		delay(190);
		now_state = MENU;
		LCD_GDRAM_MODE_CLEAR();
		LCD_Enter_Text_Mode();
//		AT24C02_WriteByte(1,px);
//		AT24C02_WriteByte(2,py);
//		AT24C02_WriteByte(3,dir);
//		AT24C02_WriteByte(4,level_state);
		read_write_test();
		delay(5);
		LCD_Text_Mode_Clear();
		LCD_String(1,1,"Save Success!");
		delay(1000);
		LCD_Text_Mode_Clear();
		delay(100);
		LCD_Text_Mode_Clear();
		delay(1000);
		draw_menu();
		LCD_Text_Mode_Clear();
		draw_menu();
	}
}



void testrender()
{
	unsigned char i;
	for(i=0;i<128;i++){
	LCD_Draw_Vertical_Line(i,1,10);
	}
}

void judge_if_success(void)
{
	if(map[py/4][px/4]==2)
	{
		now_state = MENU; 
		LCD_Enter_Text_Mode();
		LCD_String(1,2,"Configuration!");
		delay(3000);
	}
}

void draw_level_choice(void)
{
	LCD_String(0,0," Easy");
	LCD_String(0,1," Difficult");
	if(level_state == 0)
	{
		LCD_String(0,0,">");
		LCD_String(0,1," ");
	}
	else
	{
		LCD_String(0,0," ");
		LCD_String(0,1,">");
	}
	
	
}

void testLevelChoice(void)
{
	unsigned char i,j;
	draw_level_choice();
	if(P24 == 0)
	{
		delay(190);
		level_state++;
		if(level_state>1)level_state=0;
		draw_level_choice();
	}
	else if(P25 == 0)
	{
		if(level_state == 0)
		{
			for(i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{
					map[i][j] = map1[i][j];
				}
			
			}
			LCD_Text_Mode_Clear();
			LCD_String(1,1,"Easy Mode");
			delay(1000);
			now_state = MENU;
			LCD_Text_Mode_Clear();
			draw_menu();
		}
		else if(level_state == 1)
		{
			for(i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{
					map[i][j] = map2[i][j];
				}
			}
			LCD_Text_Mode_Clear();
			LCD_String(1,1,"Challenge");
			delay(1000);
			now_state = MENU;
			LCD_Text_Mode_Clear();
			draw_menu();
		}
	}
	
}

void check_level_state()
{
	unsigned char i,j;
	if(level_state == 0)
	{
		for(i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
				map[i][j] = map1[i][j];
			}
		}
	}
	else if(level_state == 1)
	{
		for(i=0;i<8;i++)
		{
			for(j=0;j<8;j++)
			{
				map[i][j] = map2[i][j];
			}
		}
	}
}

void read_store_test(void)
{
	px = AT24C02_ReadByte(1);
//	AT24C02_ReadByte(3);
	py = AT24C02_ReadByte(5);
//	AT24C02_ReadByte(7);
	dir = AT24C02_ReadByte(9);
//	AT24C02_ReadByte(11);
	level_state = AT24C02_ReadByte(13);

}

void read_write_test(void)
{
	AT24C02_WriteByte(1,2);
//	AT24C02_WriteByte(3,2);
	AT24C02_WriteByte(5,2);
//	AT24C02_WriteByte(7,1);
	AT24C02_WriteByte(9,1);
	
	AT24C02_WriteByte(13,1);
}

void init_map(void)
{
	unsigned char i,j;
			for(i=0;i<8;i++)
			{
				for(j=0;j<8;j++)
				{
					map[i][j] = map2[i][j];
				}
			}
}





void debug_print_map(void)
{
    unsigned char i, j;
    LCD_Enter_Text_Mode();
    LCD_Text_Mode_Clear();
    
    for(i=0; i<8; i++)
    {
        for(j=0; j<8; j++)
        {
            if(map[i][j] == 0)
                LCD_String(1, 1, "A"); // ??????
            else if(map[i][j] == 1)
                LCD_String(j, i, "#"); // ??? #
            else if(map[i][j] == 2)
                LCD_String(j, i, "O"); // ???? O
        }
    }
    
    // ???????
    while(P33 == 1); 
}

void main(void)
{
	
//	Timer_Init();
	
	LCD_Init();
////测试
//	LCD_Enter_Text_Mode();
//	
//	LCD_String(1,1,"H");
//	delay(1000);
//	LCD_Text_Mode_Clear();
//	LCD_Enter_GDRAM_Mode();
//	testrender();
//	
//测试地图选择玄学问题地图赋值不正常
//	LCD_String(1,1,"A");
//	map[0][0]= map2[6][6];
//	delay(5);
//	if(map[0][0] == 0x02)
//	{
//		delay(10);
//		LCD_String(0,0,"A");
//	}
//	else if(map[0][0]!=0x02)
//	{
//		LCD_String(1,3,"C");
//	}
//	LCD_String(1,2,"B");

//测试存储

//	read_write_test();
//	
//	read_store_test();
//	if(px == 2)
//	{
//		LCD_String(0,0,"px");
//	}
//	if(py == 2)
//	{
//		LCD_String(0,1,"py");
//	}
//	if(dir == 1)
//	{
//		LCD_String(0,2,"dir");
//	}
//	if(level_state == 1)
//	{
//		LCD_String(0,3,"level");
//	}
//	
//	check_level_state();
	
//	while(1);
	
	
	
//	while(1);
	
//======================================================================================
//游戏逻辑
	init_map();
	draw_menu();

	
	
	while(1)
	{

		if(now_state == MENU)
		{
			test_draw_menu();				//这里包含了对应的按键检测
		}
		else if(now_state == LEVEL_CHOICE)
		{
			testLevelChoice();
		}
		else if(now_state == GAME)
		{
			
			testGameKey();
			
			rendertest2(map);
			judge_if_success();
			
		}
		
		
	}
	

	//====================================================================================
	
	
	
	
	
	
	
	
	
	
	
//	
//	LCD_Write_Cmd(0x01);
//	delay(5);


//	//测试
//	draw_menu();
//  while(1)	
//	{
//		if(now_state != last_state)
//		{
//			last_state = now_state;
//			key_clear_all_flags();
//			if(now_state ==  MENU)
//			{
//				LCD_Enter_Text_Mode();
//			}
//			else if(now_state == GAME)
//			{
//				LCD_Enter_GDRAM_Mode();
//			}
//			
//			LCD_Clear_When_Switch_Mode();
//			
//		}
//		
//		if(now_state == MENU)
//		{
//			menu_key_state = get_MenuKey_state();
//			menuKey();
//		}
//		else if(now_state == GAME)
//		{
//			game_key_state = get_gameKey_state();
//			key_game();
//		}
//	}
//	
//	while(1);
}


//void Timer0_ISR() interrupt 1
//{
//	TH0 = 0xDC;
//	TL0 = 0x18;

//}