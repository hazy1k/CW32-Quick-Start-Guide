# 第五章 UART-串口通讯

## 1. 硬件设计

我们这里使用PA8（TX引脚）和PA9（RX引脚）的GPIO复用功能

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/beginner/uart-principle/uart-principle_20240627_221333.png)

## 2. 软件设计

### 2.1 编程大纲

1. uart1工作模式配置

2. uart发送数据函数

3. 中断服务函数

4. 主函数测试

### 2.2 代码分析

#### 2.2.1 UART配置

```c
void uart1_init(void)
{
	/* 时钟配置 */
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOA, ENABLE);  	
	RCC_APBPeriphClk_Enable2(RCC_APB2_PERIPH_UART1, ENABLE);	
    /* GPIO配置 */
	GPIO_InitTypeDef GPIO_InitStructure;
    PA08_AFx_UART1TXD();
    PA09_AFx_UART1RXD();
    GPIO_InitStructure.Pins = GPIO_PIN_8;						
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;				
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;					
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);					
    GPIO_InitStructure.Pins = GPIO_PIN_9;						
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;			
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);				
    /* USART配置 */
	USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200U;			
    USART_InitStructure.USART_Over = USART_Over_16;				
    USART_InitStructure.USART_Source = USART_Source_PCLK;	
    USART_InitStructure.USART_UclkFreq = 64000000;  			
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;	 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	
    USART_InitStructure.USART_Parity = USART_Parity_No ;	
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(CW_UART1, &USART_InitStructure);						
	/* NVIC配置 */
    NVIC_SetPriority(UART1_IRQn, 0);
    NVIC_EnableIRQ(UART1_IRQn);
    USART_ITConfig(CW_UART1, USART_IT_RC, ENABLE);
}USART_GetFlagStatus(UARTx, USART_FLAG_TXE) == RESET); 
    return ch;
}


```

#### 2.2.2 发送数据及printf函数重定义

```c
#if !defined(__MICROLIB)

#if (__ARMCLIB_VERSION <= 6000000)

struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}
#endif

void usart_send_data(uint8_t ucch)
{
    USART_SendData_8bit(CW_UART1, (uint8_t)ucch);
	while(RESET == USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE));
}

void usart_send_String(uint8_t *ucstr)
{   
      while(ucstr && *ucstr)
      {     
        usart_send_data(*ucstr++);    
      }
}
int fputc(int ch, FILE *f)
{
    USART_SendData_8bit(CW_UART1, (uint8_t)ch);
	while( RESET == USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE));	
    return ch;
}
```

#### 2.2.3 中断服务函数

```c
void UART1_IRQHandler(void)
{
    uint8_t Buffer;
    if(USART_GetITStatus(CW_UART1, USART_IT_RC) != RESET)
    {
        Buffer = USART_ReceiveData_8bit(CW_UART1);
        USART_SendData_8bit(CW_UART1, Buffer); 
        USART_ClearITPendingBit(CW_UART1, USART_IT_RC);
    }
}
```

#### 2.2.4 主函数测试

```c
#include "main.h"
#include "stdio.h"
#include "systick.h"
#include "led.h"
#include "uart.h"

int32_t main(void)
{
	SysTick_Init();
	Led_Init();
	uart1_init();
	__RCC_GPIOC_CLK_ENABLE();
	while(1)
	{
		LED_ON();
		printf("LED ON\r\n");
		delay_ms(1000);
		LED_OFF();
		printf("LED OFF\r\n");
		delay_ms(1000);		
	}
}


```
