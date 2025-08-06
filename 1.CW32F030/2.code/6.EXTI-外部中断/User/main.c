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
