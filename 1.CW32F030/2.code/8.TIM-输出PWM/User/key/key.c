#include "key.h"

void Interrupt_Key_Init(void)
{
	GPIO_InitTypeDef	GPIO_InitStruct; 
	__RCC_GPIOA_CLK_ENABLE();	
    GPIO_InitStruct.Pins  = GPIO_PIN_0;					
    GPIO_InitStruct.Mode  = GPIO_MODE_INPUT_PULLDOWN;	
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;		
    GPIO_InitStruct.IT    = GPIO_IT_RISING;			
    GPIO_Init(CW_GPIOA, &GPIO_InitStruct);			
    GPIOA_INTFLAG_CLR(bv0);
    NVIC_EnableIRQ(GPIOA_IRQn);	
}
