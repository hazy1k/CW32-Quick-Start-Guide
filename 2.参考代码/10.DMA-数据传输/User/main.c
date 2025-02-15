#include "main.h"
#include "systick.h"
#include "uart.h"
#include "adc.h"

float32_t ADC_Result;
extern uint16_t ADC_Value[10];

int32_t main(void)
{
	SysTick_Init();
	uart1_init();
	ADC_DMA_Init();
	while(1)
	{
		ADC_Result = (float32_t)adc_average()/4096*3.3;
		printf("ADC Value: %d\r\n", adc_average());
		printf("ADC Result: %.2f\r\n", ADC_Result);
		delay_ms(2000);
	}
}
