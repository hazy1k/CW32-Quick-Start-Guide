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
#include "..\inc\main.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint32_t Period = 500;    // ���ڣ���λus
uint32_t PosWidth = 0;    // ��������λus
uint8_t Dir = 1;    // �������� 1���ӣ�0 ����
/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void PWM_OutputConfig(void);

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
 * @brief  GTIM1������CH3ͨ���Ƚϣ�PB08��PWM���
 *         �жϷ�������У�ÿ4000�����ڣ�����1/4
 * @return int32_t 
 */
int32_t main(void)
{ 
  /* System Clocks Configuration */
  RCC_Configuration();  

  /* GPIO Configuration */
  GPIO_Configuration();

  PWM_OutputConfig();

  /* NVIC Configuration */
  NVIC_Configuration(); 
  
  while(1)
  {
    /* �жϷ�������GTIM1_IRQHandler() */

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
  
  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��48MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 6);     // HSI Ĭ�����Ƶ��8MHz
  RCC_PLL_OUT();
  
  ///< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_2);      
   
  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(48000000); 
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{  
  /* PB8(LED2) ��ΪGTIM1��CH3 PWM ��� */
  __RCC_GPIOB_CLK_ENABLE();

  PB08_AFx_GTIM1CH3();
  PB08_DIGTAL_ENABLE();
  PB08_DIR_OUTPUT();
  PB08_PUSHPULL_ENABLE();
  PB08_SPEED_HIGH();
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

void PWM_OutputConfig(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;

  __RCC_GTIM1_CLK_ENABLE();
  
  GTIM_InitStruct.Mode = GTIM_MODE_TIME;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV16;    // DCLK = PCLK / 16 = 48MHz/16 = 3MHz
  GTIM_InitStruct.ReloadValue = Period * 3 - 1;
  GTIM_InitStruct.ToggleOutState = DISABLE; 

  GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
  GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL3, GTIM_OC_OUTPUT_PWM_HIGH);
  GTIM_SetCompare3(CW_GTIM1, PosWidth);
  GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);

  GTIM_Cmd(CW_GTIM1, ENABLE);
}

void GTIM1_IRQHandler(void)
{
  /* USER CODE BEGIN */
  // �ж�ÿ500us����һ�Σ�ÿ50ms�ı�һ��PosWidth
  static uint16_t TimeCnt = 0;
  GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
  if (TimeCnt++ >= 100)    // 50ms
  {
    TimeCnt = 0;
    if (Dir)
    {
      PosWidth += 15;    // 5us
    }
    else
    {
      PosWidth -= 15;
    }
    if (PosWidth >= Period * 3)
    {
      Dir = 0;
    }
    if (0 == PosWidth)
    {
      Dir = 1;
    }
    GTIM_SetCompare3(CW_GTIM1, PosWidth);
  }
  /* USER CODE END */
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

