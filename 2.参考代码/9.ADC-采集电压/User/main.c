#include "main.h"
#include "systick.h"
#include "uart.h"
#include "adc.h"

float32_t ADC_Result;

int32_t main(void)
{
	SysTick_Init();
	uart1_init();
	ADCx_Init();
	while(1)
	{
		ADC_Result = (float32_t)adc_average(10)/4096*3.3;
		printf("ADC Result: %.2f V\r\n", ADC_Result);
		printf("ADC Value: %d\r\n", adc_average(10));
		delay_ms(2000);
	}
}
