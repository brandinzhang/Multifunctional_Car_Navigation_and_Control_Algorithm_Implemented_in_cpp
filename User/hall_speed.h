#ifndef __HALL_SPEED_H
#define __HALL_SPEED_H

#include "stm32f10x.h"

void HallSpeed_Init(void);
uint16_t HallSpeed_GetRPM(void);
float HallSpeed_GetSpeed(void);

#endif
