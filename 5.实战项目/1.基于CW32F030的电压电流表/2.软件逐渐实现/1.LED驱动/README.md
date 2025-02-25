# 实验一：LED灯驱动

## 1. 硬件设计

CW32F030电压电流表中关于LED灯的原理图如图所示：

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/training/voltammeter-bootcamp/voltammeter/voltammeter_20240805_194913.png)

LED1的正极经限流电阻 R10 接到电源正极，LED1 的负极连接到单片机的 GPIO 口上，通过 LED 灯的驱动原理，只需要将相应 GPIO（PC13）配置为低电平即可点亮LED1。

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/training/voltammeter-bootcamp/voltammeter/voltammeter_20240805_195522.png)

## 2. 软件设计

### 2.1 编程大纲

1. 时钟初始化

2. GPIO输出模式配置

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 时钟初始化

```c
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
```

#### 2.2.2 GPIO配置

```c
#include "led.h"

void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClk_Enable(LED_CLK, ENABLE);
		__RCC_GPIOC_CLK_ENABLE();
    GPIO_InitStructure.Pins = LED_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    // 默认关闭LED
    LED_OFF();
}

```

#### 2.2.3 主函数测试

```c
#include "main.h"
#include "systick.h"
#include "led.h"

int main(void)
{
	LED_Init();
	while(1)
	{
		LED_ON();
		delay_ms(1000);
		LED_OFF();
		delay_ms(1000);
	}
}

```