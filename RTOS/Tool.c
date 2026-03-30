#include "Tool.h"
#include "list.h"
#include "schedule.h"
//????????delayTime??
void blockTaskinTime(uint8_t delayTime)
{
    enum InterruptState InterruptStatus = interruptDisable();
		struct Task* currentTask =getCurrentTask();
    currentTask->status = TASK_DELAY;
    currentTask-> delayTick = delayTime;
    listAppend(&BlockList,currentTask->BlockListNode);

    InterruptStatus = interruptEnable();

}

//??????,?????????????????????????????
void blockTask()
{
    enum InterruptState InterruptStatus = interruptDisable();
    //
    //assert
    //
    struct Task* currentTask = getCurrentTask();
    currentTask->status = TASK_BLOCK;
    listAppend(&BlockList, currentTask->BlockListNode);

    InterruptStatus = interruptEnable();
    //
    //assert
    //
}

//?????????????
void wakeup(struct Task* task)
{
    enum InterruptState InterruptStatus = interruptDisable();
    task->status = TASK_READY;
    listRemove(&BlockList,task->BlockListNode);
    listAppend(&ReadyList,task->ReadyListNode);

    InterruptStatus = interruptEnable();
}

