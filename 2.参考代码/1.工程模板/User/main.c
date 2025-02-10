#include "main.h"

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct; // GPIO Struct
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOC, ENABLE); // RCC ENABLE
	GPIO_InitStruct.Pins = GPIO_PIN_13; // GPIO Pin
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; // GPIO Mode
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; // GPIO Speed
	GPIO_Init(CW_GPIOC, &GPIO_InitStruct); // GPIO Init
	GPIO_WritePin(CW_GPIOC, GPIO_PIN_13, GPIO_Pin_SET); // High->LED
	while(1);
}
