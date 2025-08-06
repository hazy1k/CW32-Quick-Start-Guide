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

static void GTIM1_Config(void);
static void GTIM2_Config(void);
static void LED2_Init(void);
void IrDA_Send(const uint8_t *pData, uint16_t len);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
LED_InitTypeDef LED2;
uint32_t DelayTime;
static uint8_t DataBuffer[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief  
 * @return int32_t 
 */
int32_t main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();      // HSIʱ��Դ��PLL��Ƶ64MHz��PCLK=HCLK=64MHz
  
  /* SWD �ڽ�����ͨIO���ã���δ����ֹ�����ճ��� */
  LED2_Init();
  LED2.LED_On(&LED2);
  // for (DelayTime=0x1ffffff;DelayTime; --DelayTime);
  // LED2.LED_Off(&LED2);

  /* �ر�SWD�� */
  // GPIO_SWD2GPIO();

  /* GPIO Configuration */
  GPIO_Configuration();
  
  GTIM1_Config();
  GTIM2_Config();

  CW_SYSCTRL->IRMOD_f.MOD = 0;

  /* NVIC Configuration */
  NVIC_Configuration();

  while(1)
  {
    IrDA_Send(DataBuffer, 10);
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
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     // HSI Ĭ�����Ƶ��8MHz
  RCC_PLL_OUT();
  
  ///< ��ʹ�õ�ʱ��ԴHCLK����48M,С�ڵ���72MHz������FLASH ���ȴ�����Ϊ3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
       
  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);
}

/**
  * @brief  Configure the GPIO Pins.
  * ���ԣ�ͬʱ����PA13��PB9ΪIR_OUT,�Ƿ����ͬʱ���
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __RCC_GPIOA_CLK_ENABLE();
  __RCC_GPIOB_CLK_ENABLE();

  // PA13_AFx_IROUT();
  PB09_AFx_IROUT();

  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_13;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  // GPIO_Init(CW_GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pins = GPIO_PIN_9;
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
 
  __enable_irq();  
}

/**
 * @brief GTIM1����ΪPWM������38KHz���ز�
 * 
 */
static void GTIM1_Config(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;

  __RCC_GTIM1_CLK_ENABLE();

  GTIM_InitStruct.Mode = GTIM_MODE_TIME;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV4;    // DCLK = 16MHz
  GTIM_InitStruct.ReloadValue = 16000 / 38 - 1;
  GTIM_InitStruct.ToggleOutState = DISABLE;

  GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

  GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);
  GTIM_SetCompare1(CW_GTIM1, GTIM_InitStruct.ReloadValue / 3);

  GTIM_Cmd(CW_GTIM1, ENABLE);
}

static void GTIM2_Config(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;

  __RCC_GTIM2_CLK_ENABLE();

  GTIM_InitStruct.Mode = GTIM_MODE_TIME;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV64; // DCLK = 1MHz
  GTIM_InitStruct.ReloadValue = 2240-1;
  GTIM_InitStruct.ToggleOutState = DISABLE;

  GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);

  GTIM_OCInit(CW_GTIM2, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_LOW);
  GTIM_SetCompare1(CW_GTIM2, 560);

  // GTIM_Cmd(CW_GTIM2, ENABLE);
}

static void LED2_Init(void)
{
  LED2.LED_Port = CW_GPIOB;
  LED2.LED_Pin = GPIO_PIN_8;

  LED_Init(&LED2);
}

void IrDA_Send(const uint8_t *pData, uint16_t len)
{
  uint8_t i;

  CW_GTIM1->CR0_f.EN = 1;
  while (len--)
  {
    for (i = 0; i < 8; i++)
    {
      if (*pData & (1 << i))
      {
        CW_GTIM2->CR0_f.EN = 0;
        CW_GTIM2->ICR_f.OV = 0;
        CW_GTIM2->CNT = 0;
        CW_GTIM2->CCR1 = 560;
        CW_GTIM2->ARR = 2239;
        CW_GTIM2->CR0_f.EN = 1;
        while(!CW_GTIM2->ISR_f.OV);
      }
      else
      {
        CW_GTIM2->CR0_f.EN = 0;
        CW_GTIM2->ICR_f.OV = 0;
        CW_GTIM2->CNT = 0;
        CW_GTIM2->CCR1 = 560;
        CW_GTIM2->ARR = 1119;
        CW_GTIM2->CR0_f.EN = 1;
        while (!CW_GTIM2->ISR_f.OV)
          ;
      }
    }
  }
  CW_GTIM1->CNT = 0;
  
  CW_GTIM1->CR0_f.EN = 0;
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

