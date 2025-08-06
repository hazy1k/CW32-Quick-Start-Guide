#ifndef _ADC_H_
#define _ADC_H_

#define ADC_REF_VALUE (1500)
#define R2            (220) //µ¥Î»£ºK¦¸
#define R1            (10)

#include "stdint.h"
#include "cw32f030_gpio.h"
#include "cw32f030_adc.h"
#include "cw32f030_rcc.h"

void ADC_Configuration(void);
	
#endif
