#include "tim.h"

void TIMx_Init(void)
{
    __RCC_BTIM_CLK_ENABLE();
    __disable_irq();
    NVIC_EnableIRQ(BTIM1_IRQn); // enable BTIM1 interrupt
    __enable_irq();
    BTIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.BTIM_Mode = BTIM_Mode_TIMER; // 设置为定时器模式
    TIM_TimeBaseStructure.BTIM_Period = 62500-1; // 设置周期，配置定时器每秒溢出中断
    TIM_TimeBaseStructure.BTIM_Prescaler = BTIM_PRS_DIV1024; 
    BTIM_TimeBaseInit(CW_BTIM1, &TIM_TimeBaseStructure); // 初始化定时器
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}
/* 计算过程
    系统时钟：64MHz
    定时器频率 = 系统时钟/分频系数 = 64 000 000 / 1024 = 62 500
    定时器溢出时间 = 定时器频率 / 定时器周期 = 62 500 / 62500 = 1000ms = 1s
*/
