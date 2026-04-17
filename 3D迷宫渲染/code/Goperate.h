#ifndef __GOPERTAE_H__
#define __GOPERATE_H__

extern xdata unsigned char map[8][8];
extern int px;
extern int py;
extern int dir;

extern int dx[4];
extern int dy[4];
//void render(void);
void rendertest2(unsigned char map[8][8]);
void turn(void);
void move_forward(void);
void move_back(void);

#endif