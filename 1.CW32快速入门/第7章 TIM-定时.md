# 第七章 TIM-定时

## 1. 定时器基本参数

### 1.1 预分频

预分频器可以将定时器的时钟(TIMER_CK)频率按1到65536之间的任意值分频，分频后的时钟PSC_CLK驱动计数器计数。分频系数受预分频器TIMERx_PSC控制。这个控制寄存器带有缓冲器，它能够在运行时被改变。新的预分频器的参数在下一次更新事件到来时被采用。   

分频器的分频公式为：PSC_CLK = TIMER_CK/ (TIMERx_PSC +1)

### 1.2 计数模式

计数器从0开始向上连续计数到自动加载值（定义在TIMERx寄存器中），一旦计数器计数到自动加载值，会重新从0开始向上计数并产生上溢事件。

### 1.3 更新事件

更新事件是当计数器上溢或者下溢而开始新的计数周期时触发的。更新事件可以触发DMA请求，以用于在下一个计数周期开始时及时更新定时器的运行参数，特别适合用于实时控制。

## 2. 软件设计

### 2.1 编程大纲

1. 定时器配置

2. 中断服务函数

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 定时器配置为每秒中断

```c
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

```

#### 2.2.2 中断服务函数

```c
void BTIM1_IRQHandler(void)
{
  if(BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV) == SET) // 判断BTIM1是否产生溢出中断
  {
    PC13_TOG();
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
  }
}
```

#### 2.2.3 主函数

```c
#include "main.h"
#include "systick.h"
#include "led.h"
#include "tim.h"

int32_t main(void)
{
	Led_Init();
	SysTick_Init();
	TIMx_Init();
	while(1)
	{
	}
}

```


