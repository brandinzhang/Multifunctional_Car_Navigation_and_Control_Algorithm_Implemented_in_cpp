#ifndef __HALL_SPEED_H
#define __HALL_SPEED_H

#include "stm32f10x.h"

void HallSpeed_Init(void);     // 初始化霍尔测速模块
uint16_t HallSpeed_GetRPM(void); // 获取当前转速（单位 RPM）
float HallSpeed_GetSpeed(void);  // 获取线速度（单位 m/s）

#endif
