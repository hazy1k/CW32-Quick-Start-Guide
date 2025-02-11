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

