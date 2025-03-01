#ifndef __SEG_H__
#define __SEG_H__

#include "main.h"

void SEG_GPIO_Init(void);
void SEG_Display(uint8_t Pos, uint8_t Num);
void SEG_Close(void);
void Three_Seg_Display(uint32_t value);
void SEG_Scan(void);

#endif /* __SEG_H__ */
