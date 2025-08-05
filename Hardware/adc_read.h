#ifndef __ADC_READ_H
#define __ADC_READ_H

#include "stm32f10x.h"

void ADC_Read_Init(void);
uint16_t ADC_Read_Value(void);  // 原始值 0~4095
float ADC_Read_Voltage(void);   // 返回电压值（单位：V）

#endif
