#include "speed.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"


// === 轮子参数 ===
#define WHEEL_DIAMETER_M 0.065/2     // 轮子半径：7cm
#define PULSES_PER_ROUND 20          // 每圈脉冲数


// === 定义常量 ===
#define DEBOUNCE_MS 5                // 防抖时间（毫秒）
#define TIMER_PERIOD_MS 10           // 定时器中断周期


// === 全局变量 ===
volatile uint32_t ms_ticks = 0;           // 系统运行时间计时（单位ms）
volatile uint32_t pulse_count = 0;        // 1秒内检测到的脉冲数
volatile uint16_t speed_rpm = 0;          // 当前转速（单位RPM）
volatile float total_distance_m = 0.0f;   // 累计行驶距离（单位米）
volatile uint8_t car_moving = 0;          // 标志：是否正在运动

static uint8_t last_PA0_level = 0;        // 上一次PA0电平
static uint32_t last_edge_tick = 0;       // 上一次有效边沿时间

// === 获取当前毫秒时间 ===
uint32_t get_ms_ticks(void)
{
    return ms_ticks;
}

// === 初始化定时器TIM2，每10ms进入一次中断 ===
void Speed_TIM2_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    TIM_TimeBaseStructure.TIM_Period = 100 - 1;         // 自动重装值
    TIM_TimeBaseStructure.TIM_Prescaler = 7200 - 1;     // 预分频 72MHz / 7200 = 10kHz
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    TIM_Cmd(TIM2, ENABLE);
}

// === 初始化速度模块 ===
void Speed_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    // 配置PA0为上拉输入
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    Speed_TIM2_Init();

    // 初始化变量
    last_PA0_level = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
    last_edge_tick = 0;
    ms_ticks = 0;
    pulse_count = 0;
    speed_rpm = 0;
    total_distance_m = 0.0f;
    car_moving = 0;
}

// === TIM2中断函数，每10ms触发一次 ===
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        ms_ticks += TIMER_PERIOD_MS;  // 系统时间递增

        uint8_t current_level = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
        uint32_t now = get_ms_ticks();

        if (current_level != last_PA0_level)
        {
            if ((now - last_edge_tick) >= DEBOUNCE_MS)
            {
                pulse_count++;
                total_distance_m += (3.1415926f * WHEEL_DIAMETER_M) / PULSES_PER_ROUND;
                last_edge_tick = now;
            }
        }

        last_PA0_level = current_level;

        static uint16_t interrupt_count = 0;
        interrupt_count++;
        if (interrupt_count >= (1000 / TIMER_PERIOD_MS))  // 100次 * 10ms = 1s
        {
            speed_rpm = (pulse_count * 60) / PULSES_PER_ROUND;
            pulse_count = 0;
            interrupt_count = 0;
        }

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}

// === 获取当前速度（km/h） ===
float get_speed_kph(void)
{
    return (speed_rpm * 3.1415926f * WHEEL_DIAMETER_M * 60.0f) / 1000.0f;
}

// === 获取总里程（km） ===
float get_total_distance_km(void)
{
    return total_distance_m / 1000.0f;
}