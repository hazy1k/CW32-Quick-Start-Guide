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
