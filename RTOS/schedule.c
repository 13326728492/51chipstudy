
#include "list.h"
#include "interrupt.h"
#include "schedule.h"


xdata uint8_t stack_ptr = TASK_STACK_START;

extern void Schedule(void);
xdata struct Task* CurrentTask;               //??CurrentTask?????????tcb??



void Switch1()
{
    enum InterruptState interruptStatus =  interruptDisable();
    //
    //
    //
    //
    if(ListEmpty(&ReadyList))
    {
        return;
    }
    else{
			  struct Node* tempNode = listPop(&ReadyList);
        CurrentTask->status = TASK_READY;
        listAppend(&ReadyList,CurrentTask->ReadyListNode);
       
        CurrentTask = elem2entry(struct Task,ReadyListNode,tempNode);
        CurrentTask->status = TASK_RUNNING;
    }
    interruptStatus = interruptEnable();
}


//========================================================================================================
//?51??256??RAM????????
//??????????????stack_ptr???????????????,??????????????
uint8_t* alloc_stack_51(uint8_t size)
{
    uint8_t p;
    if(stack_ptr + size > 0x80)
    {
        return NULL;                    //????
    }
    p = stack_ptr;
    stack_ptr += size;

    return (uint8_t*)p;
}

//???????????TCB,????????
void task_create(struct Task* TCB,void (*thread_func)(void),uint8_t prio)
{
    struct Stack* stack = (struct Stack*)alloc_stack_51(TASK_STACK_SIZE);
    if(stack == NULL){
        return ;            //????
    }
    stack->ACC = 0;
    stack->B   = 0;
    stack->PSW = 0;
    stack->DPL = 0;
    stack->DPH = 0;
    stack->R0 = stack->R1 = stack->R2 = stack->R3 = 0;
    stack->R4 = stack->R5 = stack->R6 = stack->R7 = 0;
    stack->arg1 = stack->arg2 = stack->arg3 = 0;

    stack->PCH = (uint16_t)thread_func >> 8;            //??????????????????????FLASH??????????16?,?????????????
    stack->PCL = (uint16_t)thread_func & 0x00FF;        //???16??????

    TCB->prio = prio;                           //????? 
    TCB->tickTime = prio;                          //??????prio
    TCB->stack_ptr = (uint8_t*)stack;                     //????????TCB
    TCB->status = TASK_READY;                   //????????

    listAppend(&AllList,TCB->allListNode);
    listAppend(&ReadyList,TCB->ReadyListNode);      //??????????

}

//
//
//=================================================================================
//
//






//===================================================================================
//????,???ReadyList??????? ???????????


//???????????TCB??
struct Task* getCurrentTask()
{
    struct Task* currentTask = CurrentTask; 
    return currentTask;
}


//======================================
//????????????????????
void TimerService()
{
    enum InterruptState interruptStatus = interruptDisable();
    //
    //??
    //

    Node* nextNode;
    Node* BlockNode = BlockList.head->next;
    while(BlockNode != NULL){
        struct Task* TCB = elem2entry(struct Task,BlockListNode,BlockNode);
        if(TCB->status == TASK_DELAY){
            TCB->delayTick--;
            if(TCB->delayTick == 0)
            {
                nextNode = BlockNode->next;
                listRemove(&BlockList,BlockNode);        //????????????????,???????,????????next????????
            
                listAppend(&ReadyList,TCB->ReadyListNode);
                TCB->status = TASK_READY;
            }
        }
        //???????????????????????????
        else if(TCB->status == TASK_BLOCK){
            //?????????????delayTick--??
        }
        BlockNode = nextNode;
    }


    interruptStatus = interruptEnable();
    //
    //??
    //
}


//??????????
void TimeService2()
{
    enum InterruptState interruptStatus = interruptDisable();
    struct Task* currentTask = getCurrentTask();
    if(--currentTask->tickTime <=0)
    {
        currentTask->tickTime = currentTask->prio;     //?????
                                   //????????
        Schedule();
        
    }

    interruptStatus = interruptEnable();
}

//====================================






