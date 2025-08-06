# 第三章 GPIO-驱动LED

## 1. 硬件设计

开发板关于LED灯的原理图如下图。LED引脚接到了PC13。

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/beginner/led-Principle/led-Principle_20240627_205913.png)

通过上面的原理图可以了解到，LED灯的负极接到了电源地，LED灯的正极连接到了单片机的GPIO口上，通过LED灯的驱动原理，只需要将GPIO配置为高电平即可点亮LED灯。对应用户LED就是使能PC13口输出高电平即可点亮用户LED。

## 2. 软件设计

### 2.1 编程大纲

1. LED相关参数宏定义

2. LED初始化函数

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 LED相关参数宏定义

```c
#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_CLK RCC_AHB_PERIPH_GPIOC
#define LED_PORT CW_GPIOC
#define LED_PIN GPIO_PIN_13

#define LED_OFF() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_SET)
#define LED_ON() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_RESET)

void Led_Init(void);

#endif /* __LED_H */

```

#### 2.2.2 LED初始化

```c
#include "led.h"

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_AHBPeriphClk_Enable(LED_CLK, ENABLE);
    GPIO_InitStructure.Pins = LED_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    // 默认关闭LED
    LED_OFF();
}

```

#### 2.2.3 主函数测试

```c
#include "main.h"

void Delay(uint32_t time)
{
    while (time--)
    {
        volatile uint32_t count = 1000;
        while (count--) {}
    }
}

int main(void)
{
	Led_Init();
	while(1)
	{
		LED_ON();
		Delay(1000);
		LED_OFF();
		Delay(1000);
	}
}

```
