#include "led.h"

void Led_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    __RCC_GPIOC_CLK_ENABLE();
    RCC_AHBPeriphClk_Enable(LED_CLK, ENABLE);
    GPIO_InitStructure.Pins = LED_PIN;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(LED_PORT, &GPIO_InitStructure);
    // 默认关闭LED
    LED_OFF();
}
