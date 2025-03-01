#include "tim.h"

void BTIMx_Init(void)
{
    BTIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    __RCC_BTIM_CLK_ENABLE();
    NVIC_EnableIRQ(BTIM1_IRQn);
    TIM_TimeBaseStructure.BTIM_Mode = BTIM_Mode_TIMER;
    TIM_TimeBaseStructure.BTIM_Period = 8000-1; // 64MHz/8MHz/8000 = 1ms
    TIM_TimeBaseStructure.BTIM_Prescaler = BTIM_PRS_DIV1; // 8MHz
    BTIM_TimeBaseInit(CW_BTIM1, &TIM_TimeBaseStructure);
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}
