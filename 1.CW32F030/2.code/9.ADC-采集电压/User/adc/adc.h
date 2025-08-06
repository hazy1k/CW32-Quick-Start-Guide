#ifndef __ADC_H
#define __ADC_H

#include "main.h"

void ADCx_Init(void);
uint32_t ADC_GET(void);
uint32_t adc_average(uint8_t times);

#endif /* __ADC_H */
