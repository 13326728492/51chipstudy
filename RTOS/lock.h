#ifndef __LOCK_H__
#define __LOCK_H__
#include "stdint.h"
#include "list.h"



struct semaphore{
    uint8_t value;
    struct List waiters;
};

struct lock{
    struct TASK* holder;
    struct semaphore semaphore;
    uint8_t holder_repeat_nr;
};

void semaInit(struct semaphore* psema,uint8_t Value);
void lockInit(struct lock* plock);
void sema_down(struct semaphore* psema);
void sema_up(struct semaphore* psema);
void lock_acquire(struct lock* plock);
void lock_release(struct lock* plock);
#endif