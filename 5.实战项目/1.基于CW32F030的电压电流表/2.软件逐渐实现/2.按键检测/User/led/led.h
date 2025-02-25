#ifndef __LED_H
#define __LED_H

#include "main.h"

#define LED_CLK RCC_AHB_PERIPH_GPIOC
#define LED_PORT CW_GPIOC
#define LED_PIN GPIO_PIN_13

#define LED_OFF() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_RESET)
#define LED_ON() GPIO_WritePin(LED_PORT, LED_PIN, GPIO_Pin_SET)

void LED_Init(void);

#endif /* __LED_H */
