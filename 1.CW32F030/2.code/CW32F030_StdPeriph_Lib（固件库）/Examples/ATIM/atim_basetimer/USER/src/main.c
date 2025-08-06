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
 * @brief  ATIM���ض���ģʽ�����ϼ�����PCLK��Ϊʱ��Դ
 *         �жϷ�������У���������תPA0
 * @return int32_t 
 */
int32_t main(void)
{
  ATIM_InitTypeDef ATIM_InitStruct;

  /* System Clocks Configuration */
  RCC_Configuration();  

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration(); 

  ATIM_InitStruct.BufferState = ENABLE;               //ʹ�ܻ���Ĵ���   
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;      //ѡ��PCLKʱ�Ӽ���
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;    //���ض���
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;        //���ϼ�����
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;    //��������ģʽ
  ATIM_InitStruct.OverFlowMask = DISABLE;        // �ظ������������������
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV1;    // 1��Ƶ
  ATIM_InitStruct.ReloadValue = 9999;                 // ��������9999+1
  ATIM_InitStruct.RepetitionCounter = 0;              // �ظ�����0
  ATIM_InitStruct.UnderFlowMask = DISABLE;           // �ظ����������������

  ATIM_Init(&ATIM_InitStruct);
  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);             // ���ظ�������������������ж�
  ATIM_Cmd(ENABLE); 
  
  while(1)
  {
    /* �жϷ�������ATIM_IRQHandlerCallBack() */
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);    // ����PCLKΪ8MHz  
  __RCC_ATIM_CLK_ENABLE();
  __RCC_GPIOA_CLK_ENABLE();
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
  GPIO_InitStruct.Pins = GPIO_PIN_0;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
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
    PA00_TOG();
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

