/*******************************************************************************
*
* ������ɺ�������Ϣ
* �人��Դ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人��Դ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人��Դ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "../inc/main.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void ATIM_IRQHandlerCallBack(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  ����ATIMΪ���豸�� BTIM1Ϊ���豸��
 *         BTIM1ʹ���ڲ�ʱ��PCLK������4��Ƶ��Ƶ��Ϊ1MHz���������У�50ms���������ATIM����
 *         
 * @return int32_t 
 */
int32_t main(void)
{
  ATIM_InitTypeDef ATIM_InitStruct;
  BTIM_TimeBaseInitTypeDef BTIM_TimeBaseStruct;

  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  LogInit();
  printf("Initial is beginning...\n");

  ATIM_InitStruct.BufferState = DISABLE;
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_SINGLE;
  ATIM_InitStruct.OverFlowMask = DISABLE;
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV1; // ����ʱ��4MHz
  ATIM_InitStruct.ReloadValue = 39999;             // �������10ms
  ATIM_InitStruct.RepetitionCounter = 0;
  ATIM_InitStruct.UnderFlowMask = DISABLE;

  ATIM_Init(&ATIM_InitStruct);
  ATIM_SlaverModeConfig(ATIM_SLAVER_TYPE_TRIGGER);
  ATIM_TriggerSelect(ATIM_TRIGGER_SOURCE_ITR);

  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);
  RCC_ATIMITRSelect(RCC_TIMITR_BTIM1);

  BTIM_TimeBaseStruct.BTIM_Mode = BTIM_Mode_TIMER;
  BTIM_TimeBaseStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
  BTIM_TimeBaseStruct.BTIM_Period = 49999;
  BTIM_TimeBaseStruct.BTIM_Prescaler = BTIM_PRS_DIV4;
  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseStruct);

  BTIM_OutputToggleCmd(CW_BTIM1, ENABLE);
  BTIM_Cmd(CW_BTIM1, ENABLE);

  printf("ATIM is running.\n");

  while (1)
  {
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  RCC_HSI_Enable(RCC_HSIOSC_DIV12);
  __RCC_ATIM_CLK_ENABLE();
  __RCC_GPIOB_CLK_ENABLE();
  __RCC_BTIM_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_0 | GPIO_PIN_9;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  PB00_AFx_BTIM1TOGN();

  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

  PB09_SETLOW();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq();
  NVIC_EnableIRQ(ATIM_IRQn);
  __enable_irq();
}

void ATIM_IRQHandlerCallBack(void)
{
  if (ATIM_GetITStatus(ATIM_IT_OVF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_OVF);
    PB09_TOG();
  }

  
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
