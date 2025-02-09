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
 * @brief  ʹ��ATIM��CH1ͨ���������PWM�ź�
 *         ATIM_CH1A�� PB2
 *         ATIM_CH1B:  PB13        
 *         PCLK��Ƶ��Ϊ8MHz
 *         
 * @return int32_t 
 */
int32_t main(void)
{
  ATIM_InitTypeDef ATIM_InitStruct;
  ATIM_OCInitTypeDef ATIM_OCInitStruct;

  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  LogInit();
  printf("ATIM Initial...\n");

  ATIM_InitStruct.BufferState = DISABLE;
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;
  ATIM_InitStruct.OverFlowMask = DISABLE;
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV8; // ����ʱ��1MHz
  ATIM_InitStruct.ReloadValue = 99;                // �������100us
  ATIM_InitStruct.RepetitionCounter = 0;
  ATIM_InitStruct.UnderFlowMask = DISABLE;

  ATIM_Init(&ATIM_InitStruct);

  ATIM_OCInitStruct.BufferState = DISABLE;
  ATIM_OCInitStruct.OCDMAState = DISABLE;
  ATIM_OCInitStruct.OCInterruptSelect = ATIM_OC_IT_UP_COUNTER;
  ATIM_OCInitStruct.OCInterruptState = ENABLE;
  ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;
  ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;
  ATIM_OC1AInit(&ATIM_OCInitStruct);
  ATIM_OC1BInit(&ATIM_OCInitStruct);

  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);
  ATIM_CH1Config(ATIM_CHxB_CIE | ATIM_CHxA_CIE, ENABLE);
  ATIM_SetCompare1A(50);
  ATIM_SetCompare1B(90);
  ATIM_PWMOutputConfig(OCREFA_TYPE_SINGLE, OUTPUT_TYPE_COMP, 1);
  ATIM_CtrlPWMOutputs(ENABLE);
  ATIM_Cmd(ENABLE);

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
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);
  __RCC_ATIM_CLK_ENABLE();
  __RCC_GPIOB_CLK_ENABLE();
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
  GPIO_InitStruct.Pins = GPIO_PIN_2 | GPIO_PIN_13;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  PB02_AFx_ATIMCH1A();
  PB13_AFx_ATIMCH1B();

  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
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
  }

  if (ATIM_GetITStatus(ATIM_IT_C1BF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_C1BF);
  }

  if (ATIM_GetITStatus(ATIM_IT_C1AF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_C1AF);
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
