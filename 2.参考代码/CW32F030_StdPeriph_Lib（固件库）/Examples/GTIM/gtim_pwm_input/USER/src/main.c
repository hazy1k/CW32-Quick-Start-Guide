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

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint32_t PWMPeriod = 0;
static uint32_t PWMWidth = 0;

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  ʹ��GTIM1��CH1��CH2����ͨ����PWM�����źŽ��в�����CH1�������ڣ�
 *         CH2��������
 *         GTIM1_CH1��PA06
 *         GTIM1_CH2: PA07
 *         ��ͬһ��PWM�����ź����뵽PA06��PA07�ϡ�
 * @return int32_t 
 */
int32_t main(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;
  GTIM_ICInitTypeDef GTIM_ICInitStruct;
  /* System Clocks Configuration */
  RCC_Configuration();  

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();  

  GTIM_InitStruct.Mode = GTIM_MODE_TIME;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;
  GTIM_InitStruct.ReloadValue = 0xFFFF;
  GTIM_InitStruct.ToggleOutState = DISABLE;
  GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

  GTIM_ICInitStruct.CHx = GTIM_CHANNEL1;
  GTIM_ICInitStruct.ICFilter = GTIM_CHx_FILTER_NONE;
  GTIM_ICInitStruct.ICInvert = GTIM_CHx_INVERT_ON;
  GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_Rising;
  GTIM_ICInit(CW_GTIM1, &GTIM_ICInitStruct);

  GTIM_ICInitStruct.CHx = GTIM_CHANNEL2;
  GTIM_ICInitStruct.ICPolarity = GTIM_ICPolarity_Falling;
  GTIM_ICInit(CW_GTIM1, &GTIM_ICInitStruct);
	
  GTIM_ITConfig(CW_GTIM1, GTIM_IT_CC1 | GTIM_IT_CC2 | GTIM_IT_OV, ENABLE);
  GTIM_Cmd(CW_GTIM1, ENABLE);  

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
  __RCC_GTIM1_CLK_ENABLE();
  __RCC_GPIOA_CLK_ENABLE();

  RCC_HSI_Enable(RCC_HSIOSC_DIV6);
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
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pins = GPIO_PIN_6 | GPIO_PIN_7;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

  PA06_AFx_GTIM1CH1();
  PA07_AFx_GTIM1CH2();
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq(); 
  NVIC_EnableIRQ(GTIM1_IRQn); 
  __enable_irq(); 
}

void GTIM1_IRQHandlerCallBack(void)
{
  static uint8_t stage = 0;
  static uint32_t cnt = 0;
  if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_OV))
  {
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
    if (stage == 1)
    {
      cnt++;
    }
  }

  if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC1))
  {
    if (stage == 0)
    {
      PWMPeriod = GTIM_GetCapture1(CW_GTIM1);
      stage = 1;
    }
    else if (stage == 1)
    {
      PWMPeriod = GTIM_GetCapture1(CW_GTIM1) + cnt * 65536 - PWMPeriod;
      stage = 0;
      cnt = 0;
    }
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC1);
  }

  if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_CC2))
  {
    if (stage == 1)
    {
          PWMWidth = GTIM_GetCapture2(CW_GTIM1) + cnt * 65536 - PWMPeriod;
          (void) PWMWidth;
    }
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_CC2);
  }
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
    /**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
    void
    assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

