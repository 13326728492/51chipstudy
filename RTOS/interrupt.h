#ifndef __INTERRUPT_H__
#define __INTERRUPT_H__
#include "stdint.h"


enum InterruptState{
    InterruptClose,
    InterruptOpen
};

struct Stack{
    uint8_t ACC;
    uint8_t B;
    uint8_t PSW;
    uint8_t DPL;
    uint8_t DPH;
    uint8_t R0;
    uint8_t R1;
    uint8_t R2;
    uint8_t R3; 
    uint8_t R4;
    uint8_t R5;
    uint8_t R6;
    uint8_t R7;
    uint8_t arg1;
    uint8_t arg2;
    uint8_t arg3;
    uint8_t PCH;
    uint8_t PCL;
};







static void IInterruptEnable();
static void IInterruptDisablle();
void interruptTimerInit();
enum InterruptState getInterruptState();
enum InterruptState interruptDisable();
enum InterruptState interruptEnable();
void interruptInit();
#endif