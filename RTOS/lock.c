#include "lock.h"
#include "interrupt.h"
#include "schedule.h"
#include "list.h"

void semaInit(struct semaphore* psema,uint8_t Value)
{
    psema->value = Value;
    listInit(&psema->waiters,"semaWaiters");
}

void lockInit(struct lock* plock)
{
    plock->holder = NULL;
    plock->holder_repeat_nr = 0;
    semaInit(&plock->semaphore,1);  //?????,???????1,??0?1????
}

//down??
void sema_down(struct semaphore* psema)
{
    enum InterruptState nowState = interruptDisable();     //???????????
    //
		//please use assert
		//
    while(psema->value == 0){
        struct Task* currentTask = getCurrentTask();
        listAppend(&psema->waiters,&currentTask->ReadyListNode);
        currentTask->status = TASK_WAITTING;

    }   
    psema->value--;
    nowState = interruptEnable();   //???
    //
		//please use assert
		//
}

//up??
void sema_up(struct semaphore* psema)
{
    enum InterruptState nowState = interruptDisable();     //???????????
    //
		//please use assert
		//
    if(ListEmpty(&psema->waiters))
    {
        psema->value++;
    }
    else {
        struct Node* tempNode = listPop(&psema->waiters);
        struct Task* tempTask = elem2entry(struct Task,ReadyListNode,tempNode);
        tempTask->status = TASK_READY;
    }
    nowState = interruptEnable();
    //
		//please use assert
		//
}
//?????
void lock_acquire(struct lock* plock)
{
    if(plock->holder != getCurrentTask())   //???????????????,??????,???????????
    {
        sema_down(&plock->semaphore);
        plock->holder = getCurrentTask();
        //
				//assert
				//
        plock->holder_repeat_nr = 1;
    }
    else{
        plock->holder_repeat_nr++;
    }

}

//?????
void lock_release(struct lock* plock){
    
    if(plock->holder_repeat_nr >1){
        plock->holder_repeat_nr--;
        return;
    }
    plock->holder = NULL;           //??????????up????
    plock->holder_repeat_nr = 0;    //?sema_up???????,????????,???????????holder_repeat_nr??1,???????
    sema_up(&plock->semaphore);
}