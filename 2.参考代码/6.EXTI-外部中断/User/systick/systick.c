#include "systick.h"

void SysTick_Init(void)
{
    /* 时钟树配置，设置系统频率为64MHz */
    RCC_HSE_Enable(RCC_HSE_MODE_OSC, 8000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE); // 开启HSE时钟
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1); // 不分频
    RCC_PLL_Enable(RCC_PLLSOURCE_HSEOSC, 8000000, RCC_PLL_MUL_8); // 开启PLL，输入为HSE，8倍频
    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH, ENABLE); // 使能FLASH时钟
	FLASH_SetLatency(FLASH_Latency_3);
    RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL); // 选择PLL作为系统时钟源
    RCC_SystemCoreClockUpdate(64000000); // 更新系统时钟64MHz
    
    /* 配置SysTick，每毫秒产生一次中断 */
    SysTick->LOAD = 64000 - 1;  // 64MHz时钟，每毫秒中断一次
    SysTick->VAL = 0;  // 清空计数器
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk | SysTick_CTRL_ENABLE_Msk;  // 启用SysTick中断
}
void delay_us(unsigned long __us) 
{
    uint32_t ticks;
    uint32_t told, tnow, tcnt = 38;
    ticks = __us * (64000000 / 1000000);
    told = SysTick->VAL;
    while (1)
    {
        tnow = SysTick->VAL;
        if (tnow != told)
        {
            if (tnow < told)
                tcnt += told - tnow;
            else
                tcnt += SysTick->LOAD - tnow + told;
            told = tnow;
            if (tcnt >= ticks)
                break;
        }
    }
}
void delay_ms(unsigned long ms) 
{
	delay_us( ms * 1000 );
}
void delay_s(unsigned long s) 
{
	delay_ms( s * 1000 );
}
