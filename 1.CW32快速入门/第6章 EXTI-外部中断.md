# 第六章 EXTI-外部中断

## 1. 硬件设计

KEY引脚接到了单片机的PA0引脚上。按键的一端接到了3.3V，另一端通过一个10K的电阻接到了地。通过检测PA0引脚的电平状态，判断按键是否按下。当按键松开的时候，PA0检测到的电平为低电平，当按键按下的时候，PA0检测到的电平为高电平。在程序里使能PA0引脚，配置为输入模式，然后以一定时间去获取PA0引脚的状态，判断按键是否按下，如果按键按下，就执行对应的功能。

## 2. 软件设计

### 2.1 编程大纲

1. key对应的引脚初始化

2. 中断服务函数

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 中断引脚初始化

```c
#include "key.h"

void Interrupt_Key_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct; 
	__RCC_GPIOA_CLK_ENABLE();	
    GPIO_InitStruct.Pins  = GPIO_PIN_0;					
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT_PULLDOWN;	
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		
    GPIO_InitStruct.IT    = GPIO_IT_RISING;			
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);			
    GPIOA_INTFLAG_CLR(bv0);
    NVIC_EnableIRQ(GPIOA_IRQn);	
}

```

#### 2.2.2 中断服务函数

```c
extern volatile uint8_t LED_FLAG;

void GPIOA_IRQHandler(void)
{
    if(CW_GPIOA->ISR_f.PIN0) // 判断PA0是否产生中断
    {
        GPIOA_INTFLAG_CLR(bv0); // 清除中断标志
        LED_FLAG = 1; // 产生中断标志
    }
}
```

#### 2.2.3 主函数测试

```c
#include "main.h"
#include "stdio.h"
#include "systick.h"
#include "led.h"
#include "uart.h"
#include "key.h"

volatile uint8_t LED_FLAG = 0;

int32_t main(void)
{
	Led_Init();
	SysTick_Init();
	uart1_init();
	Interrupt_Key_Init();
	LED_ON();
	while(1)
	{
		if(LED_FLAG == 1)
		{
			PC13_TOG();
			LED_FLAG = 0;
		}
		delay_ms(300);
	}
}

```
