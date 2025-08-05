#ifndef __SPEED_H
#define __SPEED_H

#include "stm32f10x.h"

void Speed_Init(void);
float get_speed_kph(void);
float get_total_distance_km(void);
uint32_t get_ms_ticks(void);
extern volatile uint16_t speed_rpm;
extern volatile float total_distance_m;
extern volatile uint8_t car_moving;

#endif
