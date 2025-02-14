#include "tim.h"

void TIMx_Init(void)
{  
    /* 通道GPIO配置 */
    __RCC_GPIOB_CLK_ENABLE();
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pins = GPIO_PIN_0;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    PB00_AFx_GTIM1CH3(); // PB0复用为GTIM1_CH3
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
    /* 定时器配置 */
    __RCC_GTIM1_CLK_ENABLE();
    GTIM_InitTypeDef GTIM_InitStructure;
    GTIM_InitStructure.Mode = GTIM_MODE_TIME;
    GTIM_InitStructure.OneShotMode = GTIM_COUNT_CONTINUE; // 循环计数模式
    GTIM_InitStructure.Prescaler = GTIM_PRESCALER_DIV64; // 64Mhz/64 = 1MHz
    GTIM_InitStructure.ReloadValue = 5000; // 重载值设置
    GTIM_InitStructure.ToggleOutState = DISABLE; // 禁止输出翻转
    GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStructure);
    GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL3, GTIM_OC_OUTPUT_PWM_HIGH); // 使能GTIM1_CH3输出高电平PWM
    GTIM_SetCompare3(CW_GTIM1, 2500); // 设置比较值，占空比为50%
    GTIM_Cmd(CW_GTIM1, ENABLE);   
}
/* 计算过程
    系统时钟：64MHz
    定时器频率(PWM) = 系统时钟/分频系数 = 64 000 000 / 64 = 1 000 000 = 1MHz
    PWM周期：重载值*定时器频率 = 5000 * 1 = 5000us = 5ms
    PWM的频率为：1/T = 1/5ms = 200Hz
*/
