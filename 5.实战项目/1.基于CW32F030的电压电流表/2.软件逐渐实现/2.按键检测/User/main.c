#include "main.h"
#include "stdio.h"
#include "systick.h"
#include "led.h"
#include "key.h"

uint8_t KEY_Flag = 0;
uint8_t LED_Flag = 0;

void KEY_Scan(void)
{
	if(GPIO_ReadPin(CW_GPIOB, GPIO_PIN_14) == GPIO_Pin_RESET)
	{
		KEY_Flag = 1;
	}
	if(KEY_Flag)
	{
		if(GPIO_ReadPin(CW_GPIOB, GPIO_PIN_14))
		{
			KEY_Flag = 0;
			LED_Flag = 1 - LED_Flag;
		}
	}
}

void LED_Ctrl(void)
{
	if(LED_Flag)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
}

int32_t main(void)
{
	LED_Init();
	SysTick_Init();
	KEY_Init();
	LED_ON();
	while(1)
	{
		KEY_Scan();
		LED_Ctrl();
	}
}
