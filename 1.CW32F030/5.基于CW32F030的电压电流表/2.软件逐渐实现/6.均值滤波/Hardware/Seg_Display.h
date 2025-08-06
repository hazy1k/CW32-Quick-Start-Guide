#ifndef __SEG_DISPLAY_H
#define __SEG_DISPLAY_H

#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"

void Seg_Configuration(void);
void Seg_Dis(uint8_t Pos,uint8_t Num);
void Close_Com(void);
void Display_Refresh(void);
void Dynamic_Display(uint32_t value);

#endif
