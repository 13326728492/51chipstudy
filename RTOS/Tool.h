#ifndef __TOOL_H__
#define __TOOL_H__
#include "List.h"
#include "interrupt.h"

void blockTaskinTime(uint8_t delayTime);
void blockTask();
void wakeup(struct Task* task);
#endif