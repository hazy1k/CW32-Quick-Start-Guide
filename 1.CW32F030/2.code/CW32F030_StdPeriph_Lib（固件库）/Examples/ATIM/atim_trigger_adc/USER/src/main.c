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
static void ATIM_Config(void);
static void ADC_Config(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint16_t ADC_ConvertedValue = 0;
float ADC_ConvertedVoltage = 0.0;
uint8_t ADC_ConvertedFlag = 0;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  �����У�ATIM��PWMģʽ�����ã�ATIM��OC3Bʱ�����ڴ���ADC
 * ADC����Ϊ��ͨ������ת��
 * @return int32_t 
 */
int32_t main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration(); // HSIʱ��Դ��PLL��Ƶ64MHz��PCLK=HCLK=64MHz

  /* GPIO Configuration */
  GPIO_Configuration();

  ATIM_Config();
  ADC_Config();

  /* NVIC Configuration */
  NVIC_Configuration();

  while (1)
  {
    if (ADC_ConvertedFlag)
    {
      ADC_ConvertedFlag = 0;
      ADC_ConvertedVoltage = ADC_ConvertedValue * 3.3 / 0xfff;
    }
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  /* 0. HSIʹ�ܲ�У׼ */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8); // HSI Ĭ�����Ƶ��8MHz
  RCC_PLL_OUT();

  ///< ��ʹ�õ�ʱ��ԴHCLK����48M,С�ڵ���72MHz������FLASH ���ȴ�����Ϊ3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);

  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);

  /* 4.ʹ������ʱ�� */
  __RCC_GPIOA_CLK_ENABLE();
  __RCC_GPIOB_CLK_ENABLE();
  __RCC_ATIM_CLK_ENABLE();
  __RCC_ADC_CLK_ENABLE();
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
  GPIO_InitStruct.Pins = GPIO_PIN_9;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

  PB00_ANALOG_ENABLE();
  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pins = GPIO_PIN_0;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
}

/**
 * @brief ATIM���ض��룬�ϼ�����64��Ƶ��DCLK=1MHz��period=10ms
 *        OC3Bƥ���¼�����ADC
 */
static void ATIM_Config(void)
{
  ATIM_InitTypeDef ATIM_InitStruct;
  ATIM_OCInitTypeDef ATIM_OCInitStruct;

  __RCC_ATIM_CLK_ENABLE();

  ATIM_InitStruct.BufferState = ENABLE;                            //ʹ�ܻ���Ĵ���
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;                   //ѡ��PCLKʱ�Ӽ���
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN; //���ض���
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;             //���ϼ�����
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;         //��������ģʽ
  ATIM_InitStruct.OverFlowMask = DISABLE;                          // �ظ������������������
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV64;                // 1��Ƶ
  ATIM_InitStruct.ReloadValue = 9999;                              // ��������9999+1
  ATIM_InitStruct.RepetitionCounter = 0;                           // �ظ�����0
  ATIM_InitStruct.UnderFlowMask = DISABLE;                         // �ظ����������������

  ATIM_Init(&ATIM_InitStruct);

  ATIM_OCInitStruct.BufferState = DISABLE;
  ATIM_OCInitStruct.OCDMAState = DISABLE;
  ATIM_OCInitStruct.OCInterruptSelect = ATIM_OC_IT_UP_COUNTER;
  ATIM_OCInitStruct.OCInterruptState = DISABLE;
  ATIM_OCInitStruct.OCMode = ATIM_OCMODE_PWM1;
  ATIM_OCInitStruct.OCPolarity = ATIM_OCPOLARITY_NONINVERT;
  ATIM_OC3BInit(&ATIM_OCInitStruct);

  ATIM_ADCTriggerConfig(ATIM_ADC_TRIGGER_CH3B, ENABLE);
  ATIM_SetCompare3B(4999);
  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);

  ATIM_Cmd(ENABLE);
}

/**
 * @brief ADC ʹ��ͨ��8����ת����ת�������󴥷��ж�
 * 
 */
static void ADC_Config(void)
{
  ADC_InitTypeDef ADC_InitStruct;

  __RCC_ADC_CLK_ENABLE();

  ADC_InitStruct.ADC_AccEn = DISABLE;
  ADC_InitStruct.ADC_Align = ADC_AlignRight;
  ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div4; // ADCCLK=16MHz
  ADC_InitStruct.ADC_DMAEn = DISABLE;
  ADC_InitStruct.ADC_InBufEn = DISABLE;
  ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;
  ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk;
  ADC_InitStruct.ADC_TsEn = DISABLE;
  ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;

  ADC_Init(&ADC_InitStruct);

  CW_ADC->CR1_f.CHMUX = 8; // PB0
  ADC_ExtTrigCfg(ADC_TRIG_ATIM, ENABLE);
  ADC_ITConfig(ADC_IT_EOC, ENABLE);
  ADC_Enable();
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
  NVIC_EnableIRQ(ADC_IRQn);
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

void ADC_IRQHandlerCallBack(void)
{
  if (ADC_GetITStatus(ADC_IT_EOC))
  {
    ADC_ClearITPendingBit(ADC_IT_EOC);
    ADC_ConvertedValue = ADC_GetConversionValue();
    ADC_ConvertedFlag = 1;
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
