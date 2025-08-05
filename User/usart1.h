#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"

void USART1_Init(void);
void USART1_SendChar(char c);
void USART1_SendString(char* str);

#endif
