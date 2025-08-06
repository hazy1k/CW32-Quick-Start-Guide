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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  ʹ��BTIM1��ΪGTIM1�ķ�Ƶ������PCLK����48000��Ƶ
 * @return int32_t 
 */
int32_t main(void)
{
  GTIM_InitTypeDef GTIM_InitStruct;
  BTIM_TimeBaseInitTypeDef BTIM_InitStruct;

  /* System Clocks Configuration */
  RCC_Configuration();  

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();  

  GTIM_InitStruct.Mode = GTIM_MODE_COUNTER;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;
  GTIM_InitStruct.ReloadValue = 999;
  GTIM_InitStruct.ToggleOutState = DISABLE;
  GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);

  GTIM1_ITRConfig(ITR_SOURCE_BTIM1);

  BTIM_InitStruct.BTIM_Mode = BTIM_Mode_TIMER;
  BTIM_InitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
  BTIM_InitStruct.BTIM_Period = 47999;
  BTIM_InitStruct.BTIM_Prescaler = BTIM_PRS_DIV1;
  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_InitStruct);

  GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);

  GTIM_Cmd(CW_GTIM1, ENABLE);
  BTIM_Cmd(CW_BTIM1, ENABLE);

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
  //    CW_SYSCTRL->APBEN1_f.GTIM1 = 1U;    //
  __RCC_FLASH_CLK_ENABLE();    // ��FLASHʱ��
  FLASH_SetLatency(FLASH_Latency_2);    // Ƶ�ʴ���24M��Ҫ����FlashWait=2
  RCC_HSI_Enable(RCC_HSIOSC_DIV1);
  
  __RCC_BTIM_CLK_ENABLE();
  __RCC_GTIM1_CLK_ENABLE();
  __RCC_GPIOB_CLK_ENABLE();
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  PB09_DIGTAL_ENABLE();
  PB09_DIR_OUTPUT();
  PB09_PUSHPULL_ENABLE();

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
  if (GTIM_GetITStatus(CW_GTIM1, GTIM_IT_OV))
  {
    GTIM_ClearITPendingBit(CW_GTIM1, GTIM_IT_OV);
    PB09_TOG();
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
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

