#include "interrupt.h"
#include "ioPort.h"

//?????????
void IInterruptEnable()
{
    IE = 0x82;    //EA?,?????
    TR0 = 1;      //?????0
}

//???????
static void interruptTimerInit()
{
    TMOD=0x01;
    TL0 = 0x18;
    TH0 = 0xFC;
}

//?????????
static void IInterruptDisablle()
{
    IE = 0x00;    //EA?,?????
    TR0 = 0;
}

//?????????
enum InterruptState getInterruptState()
{
    return (IE & 0x80)? InterruptOpen : InterruptClose;
}
//????????????
enum InterruptState interruptDisable()
{
    enum InterruptState OldState = getInterruptState();
    if(OldState!=InterruptOpen)
    {
        return -1;
    }
    else {
        interruptDisable();
    }

    return InterruptClose;
}
//??????????
enum InterruptState interruptEnable()
{
    enum InterruptState OldState = getInterruptState();
    if(OldState != InterruptClose)
    {
        return -1;
    }
    else{
        IInterruptEnable();
    }
    return InterruptOpen;
}


void interruptInit()
{
    interruptTimerInit();
}