#include "main.h"
#include "stdio.h"
#include "systick.h"
#include "seg.h"
#include "tim.h"

int32_t main(void)
{
	SysTick_Init();
	SEG_GPIO_Init();
	BTIMx_Init();
	while(1)
	{
		Three_Seg_Display(666);
		delay_ms(5000);
		Three_Seg_Display(333);
		delay_ms(5000);
	}
}
