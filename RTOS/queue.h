#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "List.h"
#include "lock.h"
#define MAX_QUEUE_SIZE 5



struct queue{
    uint8_t queue[MAX_QUEUE_SIZE];
    struct lock lock;
    uint8_t head;
    uint8_t tail;
};

void queueInit(struct queue* pqueue);
static uint8_t queueNextPos(uint8_t pos);
BOOL queueIsFull(struct queue* pqueue);
BOOL queueIsEmpty(struct queue* pqueue);
void ProducerElem(struct queue* pqueue,char queue_elem);
char ConsumeElem(struct queue* pqueue);
#endif