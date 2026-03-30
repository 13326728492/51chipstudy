#include "queue.h"
#include "interrupt.h"
#include "Tool.h"
#include "stdint.h"

void queueInit(struct queue* pqueue)
{
    pqueue->head = 0;               //????
    pqueue->tail = 0;               //????
    lockInit(&pqueue->lock);

}

//??pos????????????
static uint8_t queueNextPos(uint8_t pos)
{
    return (pos + 1) % MAX_QUEUE_SIZE;
}

//????????
BOOL queueIsFull(struct queue* pqueue)
{
	return (queueNextPos(pqueue->head) == pqueue->tail) ? 1 : 0;
}

//????????
BOOL queueIsEmpty(struct queue* pqueue)
{
	return (pqueue->head == pqueue->tail) ? 1 : 0; 
}
//????
void ProducerElem(struct queue* pqueue,char queue_elem)
{
    //
    //assert????????
    //
    if(queueIsFull(pqueue))             //???????????????
    {
        return;                         //??0???????
    }
    pqueue->queue[pqueue->head] = queue_elem;
    pqueue->head = queueNextPos(pqueue->head);

}
//??????       //????????
char ConsumeElem(struct queue* pqueue)
{
	  char queue_elem = pqueue->queue[pqueue->tail];
    if(queueIsEmpty(pqueue))            //???????????????
    {
        return 0;                       //??0???????
    }
 
    pqueue->tail = queueNextPos( pqueue->tail);
    return queue_elem;
}
