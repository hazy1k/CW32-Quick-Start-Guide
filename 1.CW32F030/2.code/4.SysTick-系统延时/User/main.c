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
