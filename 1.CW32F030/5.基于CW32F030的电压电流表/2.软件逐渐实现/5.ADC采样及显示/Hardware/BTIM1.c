#include "BTIM1.h"

void Btim1_Configuration(void)
{
    BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
    __RCC_BTIM_CLK_ENABLE();

    /* NVIC Configuration */
    NVIC_EnableIRQ(BTIM1_IRQn);

    BTIM_TimeBaseInitStruct.BTIM_Mode      = BTIM_Mode_TIMER;
    BTIM_TimeBaseInitStruct.BTIM_Period    = 8000 - 1;      // 1ms´¥·¢1´Î
    BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV1; // 8MHZ
    BTIM_TimeBaseInitStruct.BTIM_OPMode    = BTIM_OPMode_Repetitive;

    BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct);
    BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
    BTIM_Cmd(CW_BTIM1, ENABLE);
}
