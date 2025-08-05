#include "motor.h"

// 占空比等级（仅两档：全速 = 100，减速 = 70）
#define SPEED_FULL     50
#define SPEED_TURN     30

// 当前左右电机速度（用于更新）
static uint16_t left_speed = SPEED_FULL;
static uint16_t right_speed = SPEED_FULL;

void Motor_Init(void)
{
    // 开启时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 初始化GPIO (PA6 - TIM3_CH1, PA7 - TIM3_CH2)
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  // ?? 你原来写成了 GPIO_InitStructuare，有拼写错误

    // 配置定时器
    TIM_InternalClockConfig(TIM3);

    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_Period = 100 - 1;      // ARR = 100
    TIM_TimeBaseStructure.TIM_Prescaler = 36 - 1;    // PSC = 36
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);

    // 初始化PWM输出
    TIM_OCInitTypeDef TIM_OCInitStructure;
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;  // 初始占空比为0

    TIM_OC1Init(TIM3, &TIM_OCInitStructure); // PA6 - 左电机
    TIM_OC2Init(TIM3, &TIM_OCInitStructure); // PA7 - 右电机

    TIM_Cmd(TIM3, ENABLE);
}

// 设置左右电机占空比（0~100）
void Motor_SetSpeed(uint16_t left, uint16_t right)
{
    if (left > 100) left = 100;
    if (right > 100) right = 100;

    left_speed = left;
    right_speed = right;

    TIM_SetCompare1(TIM3, left_speed);
    TIM_SetCompare2(TIM3, right_speed);
}
