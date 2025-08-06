#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_CLK RCC_AHB_PERIPH_GPIOC
#define LED_PORT CW_GPIOC
#define LED_PIN GPIO_PIN_13

#define LED_OFF() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_SET)
#define LED_ON() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_RESET)

void Led_Init(void);

#endif /* __LED_H */
