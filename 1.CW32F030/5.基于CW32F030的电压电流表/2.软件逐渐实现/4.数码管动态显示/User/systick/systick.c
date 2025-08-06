#include "systick.h"

void SysTick_Init(void)
{
    /* 时钟树配置，设置系统频率为64MHz */
    RCC_HSI_Enable(RCC_HSIOSC_DIV6);
    RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
    RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
    RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);       
    __RCC_FLASH_CLK_ENABLE();
    FLASH_SetLatency(FLASH_Latency_3);
    RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
    RCC_SystemCoreClockUpdate(64000000);
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
