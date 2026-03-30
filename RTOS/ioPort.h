#ifndef __IOPORT_H__
#define __IOPORT_H__
sfr P0 = 0x80;
sfr P1 = 0x90;
sfr P2 = 0xA0;
sfr P3 = 0xB0;
sfr P4 = 0xC0;

//interrupt
sfr IE = 0xA8;
sfr IP = 0xB8;
sfr IPH = 0xB7;
sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0 = 0x8A;
sfr TH0 = 0x8C;


sbit TR0= 0x88^4;






#endif