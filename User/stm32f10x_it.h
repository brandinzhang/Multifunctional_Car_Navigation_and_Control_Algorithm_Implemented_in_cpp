/**
  ******************************************************************************
  * @file    stm32f10x_it.h
  * @author  用户
  * @brief   头文件，用于中断处理函数声明。
  ******************************************************************************
  */

#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f10x.h"

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

void EXTI0_IRQHandler(void);    // 霍尔传感器（PA0）
void TIM2_IRQHandler(void);     // 定时器 TIM2：测速

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */
