#include "stm32f10x.h"                  // Device header
#include "PWMServo.h"

void Servo_Init(void)
{
	PWM_Init2();
}

void Servo_SetAngle(float Angle)
{
	PWM_2SetCompare3(Angle / 180 * 2000 + 500);
}
