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
void GenerateTestSignal(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint32_t PWMPeriod = 0;
static uint32_t PWMWidth = 0;
static uint8_t ProcessState = 0;

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  ʹ��ATIM��CH1Aͨ����PWM�����źŽ��в�����CH1A˫���ز����������������
 *         ATIM_CH1A��PB02        
 *         ��ͬһ��PWM�����ź����뵽PB02�ϡ�
 *         PCLK��Ƶ��Ϊ8MHz
 *         ʹ��PA7��ΪBTIM1��TOGP��������������ź�
 * @return int32_t 
 */
int32_t main(void)
{
  ATIM_InitTypeDef ATIM_InitStruct;
  ATIM_ICInitTypeDef ATIM_ICInitStruct;
  ProcessState = 0;
  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  LogInit();
  GenerateTestSignal();
  printf("Generating test signal\n");
  printf("ATIM Initial...\n");

  ATIM_InitStruct.BufferState = DISABLE;
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;
  ATIM_InitStruct.OverFlowMask = DISABLE;
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV1;
  ATIM_InitStruct.ReloadValue = 0xFFFF;
  ATIM_InitStruct.RepetitionCounter = 0;
  ATIM_InitStruct.UnderFlowMask = DISABLE;

  ATIM_Init(&ATIM_InitStruct);

  ATIM_ICInitStruct.ICFilter = ATIM_ICFILTER_NONE;
  ATIM_ICInitStruct.ICPolarity = ATIM_ICPOLARITY_BOTHEDGE;
  ATIM_IC1AInit(&ATIM_ICInitStruct);

  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);
  ATIM_CH1Config(ATIM_CHxA_CIE, ENABLE);
  ATIM_Cmd(ENABLE);
  printf("ATIM is running.\n");

  while (1)
  {
    if (ProcessState == 1)
    {
      printf("Input PWM period   is %dns\n", PWMPeriod * 125);
      printf("Input PWM positive is %dns\n", PWMWidth * 125);
      ProcessState = 0;
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
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pins = GPIO_PIN_2;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  PB02_AFx_ATIMCH1A();

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
  static uint8_t stage = 0;
  static uint32_t cnt = 0;
  if (ATIM_GetITStatus(ATIM_IT_OVF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_OVF);
    if (stage)
    {
      cnt++;
    }
  }

  if (ATIM_GetITStatus(ATIM_IT_C1AF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_C1AF);
    if (stage == 0)
    {
      PWMPeriod = ATIM_GetCapture1A();
      cnt = 0;
      stage++;
    }
    else if (stage == 1)
    {
      PWMWidth = ATIM_GetCapture1A() + cnt * 0x10000UL - PWMPeriod;
      stage++;
    }
    else if (stage == 2)
    {
      PWMPeriod = ATIM_GetCapture1A() + cnt * 0x10000UL - PWMPeriod;
      stage = 0;
      ProcessState = 1;
    }
  }
}

void GenerateTestSignal(void)
{
  BTIM_TimeBaseInitTypeDef BTIM_InitStruct = {0};
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  BTIM_InitStruct.BTIM_Mode = BTIM_Mode_TIMER;
  BTIM_InitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
  BTIM_InitStruct.BTIM_Period = 49;
  BTIM_InitStruct.BTIM_Prescaler = BTIM_PRS_DIV16;

  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7;

  __RCC_BTIM_CLK_ENABLE();
  __RCC_GPIOA_CLK_ENABLE();

  PA07_AFx_BTIM1TOGP();

  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_InitStruct);

  BTIM_OutputToggleCmd(CW_BTIM1, ENABLE);
  BTIM_Cmd(CW_BTIM1, ENABLE);
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
