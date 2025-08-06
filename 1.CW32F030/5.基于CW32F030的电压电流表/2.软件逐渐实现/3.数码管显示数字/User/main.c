#include "main.h"
#include "stdio.h"
#include "systick.h"
#include "led.h"
#include "seg.h"

int32_t main(void)
{
	LED_Init();
	SysTick_Init();
	SEG_GPIO_Init();

	SEG_Display(1, 8); // 第1位显示8
}
