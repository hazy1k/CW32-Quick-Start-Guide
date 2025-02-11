#ifndef __SYSTICK_H
#define __SYSTICK_H

#include "main.h"

void SysTick_Init(void);
void delay_us(unsigned long __us);
void delay_ms(unsigned long ms);
void delay_s(unsigned long s);

#endif /* __SYSTICK_H */
