# 第四章 SysTick-系统延时

## 1. 硬件设计

SysTick定时器可用作标准的下行计数器，是一个24位向下计数器，有自动重新装载能力，可屏蔽系统中断发生器。Cortex-M0+处理器内部包含了一个简单的定时器，所有基于M0+内核的控制器都带有SysTick定时器,这样就方便了程序在不同的器件之间的移植。SysTick定时器可用于操作系统，提供系统必要的时钟节拍，为操作系统的任务调度提供一个有节奏的“心跳”。

SysTick定时器设定初值并使能之后，每经过1个系统时钟周期，计数值就减1，减到0时，SysTick计数器自动重新装载初值并继续计数，同时内部的COUNTFLAG标志位被置位，触发中断（前提开启中断）。

## 2. 软件设计

### 2.1 编程大纲

1. SysTick初始化

2. 主函数测试

### 2.2 代码分析

#### 2.2.1 SysTick初始化

```c
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

```

#### 2.2.2 主函数

```c
#include "main.h"
#include "led.h"
#include "systick.h"

int32_t main(void)
{
	Led_Init();
	SysTick_Init();
	__RCC_GPIOC_CLK_ENABLE();
	while(1)
	{
		LED_ON();
		delay_ms(1000);
		LED_OFF();
		delay_ms(1000);
	}
}

```
