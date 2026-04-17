#include "LCD12864.h"
#include "Goperate.h"

#define renderCofficient1 4
#define renderCofficient2 6
int ray_x;
int ray_y;
int distance;


int lineHeight;
int drawStart;
int drawEnd;

const int screenH;


 
void cast_ray(int x,unsigned char map[8][8])
{
	int camera_x = x - 64;
	int rdx,rdy;
	int gx,gy;
	if(dir == 0 || dir == 2)
	{
		rdx = dx[dir]/2;
		rdy = dy[dir]/2 + camera_x/renderCofficient1;
	}
	else
	{
		rdx = dx[dir]/2 + camera_x/renderCofficient1;
		rdy = dy[dir]/2;
	}
	
	ray_x = px;
	ray_y = py;
	distance = 0;
	// caculate ray distance
	while(1)
	{
		ray_x += rdx;
		ray_y += rdy;
		distance += 2;
		
		gx = ray_x/4;
		gy = ray_y/4;
		if(gx<0 || gx >= 8 || gy <0 || gy >=8)break;
		if(map[gy][gx] == 1)break;
	}
}
	
//void render(void)
//{
//	unsigned char x;
//	int gx_hit;
//	int gy_hit;
//	
//	for(x = 0 ; x < 128 ; x++)
//	{

//		cast_ray(x);
//		gx_hit = ray_x/4;
//		gy_hit = ray_y/4;
//		lineHeight = (64*renderCofficient2)/distance;
//		drawStart = 32 - lineHeight/2 ;
//		drawEnd = 32 + lineHeight/2 ;
//		
//		if(drawStart < 0)drawStart = 0;
//		if(drawEnd >= 64)drawEnd = 63;
//		
//		if(map[gy_hit][gx_hit])
//		{
//			LCD_Draw_Vertical_Line(x,drawStart,drawEnd);
//		}
//		else if(map[gy_hit][gx_hit] == 2)
//		{
//			int mid = (drawStart + drawEnd)/2;
//			LCD_Draw_Vertical_Line(x,drawStart,mid-4);
//			LCD_Draw_Vertical_Line(x,mid+4,drawEnd);
//		}
//	}
//	
//}

void rendertest2(unsigned char map[8][8])
{
	unsigned char x;
	int gx_hit;
	int gy_hit;
	for(x = 0;x<128;x++)
	{
		cast_ray(x,map);
		gx_hit = ray_x/4;
		gy_hit = ray_y/4;
		lineHeight = (64*renderCofficient2)/distance;
		drawStart = 12 - lineHeight/4 ;
		drawEnd = drawStart + lineHeight;
		
		if(drawStart < 0)drawStart = 0;
		if(drawEnd >= 64)drawEnd = 63;
		
		if(map[gy_hit][gx_hit])
		{
			LCD_Draw_Vertical_Line(x,drawStart,drawEnd);
		}
		else if(map[gy_hit][gx_hit] == 2)
		{
			int mid = (drawStart + drawEnd)/2;
			LCD_Draw_Vertical_Line(x,drawStart,mid-4);
			LCD_Draw_Vertical_Line(x,mid+4,drawEnd);
		}
	}
}

void turn(void)
{
	dir++;
	if(dir > 3)dir = 0;
}

void move_forward(void)
{
	int new_x = px  + dx[dir];
	int new_y = py  + dy[dir];
	
	int gx = new_x /4;
	int gy = new_y /4;
	
	if(map[gy][gx] == 1)return;   //Åö×²¼ì²â
	
	px = new_x;
	py = new_y;
}

void move_back(void)
{
	int new_x = px - dx[dir];
	int new_y = py - dy[dir];
	
	int gx = new_x /4;
	int gy = new_y /4;
	
	if(map[gy][gx] == 1)return;
	
	px = new_x;
	py = new_y;
	
}