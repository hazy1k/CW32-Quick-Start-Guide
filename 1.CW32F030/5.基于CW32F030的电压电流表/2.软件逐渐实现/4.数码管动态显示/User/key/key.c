#include "key.h"

void KEY_Init(void)
{
  __RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pins = GPIO_PIN_14;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_InitStructure.IT = GPIO_IT_NONE;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}

