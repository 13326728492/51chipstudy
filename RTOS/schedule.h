#ifndef __SCHEDULE_H__
#define __SCHEDULE_H__
#define TASK_STACK_SIZE sizeof(struct Stack)
#define MAX_TASK_NUM 5
#define TASK_STACK_START 0x30

extern xdata struct Task* CurrentTask;               //??CurrentTask?????????tcb??



uint8_t* alloc_stack_51(uint8_t size);
void task_create(struct Task* TCB,void (*thread_func)(void),uint8_t prio);
 void Switch1();
struct Task* getCurrentTask();
void TimerService();
void TimeService2();
struct Task* getCurrentTask();

#endif