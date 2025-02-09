/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-08-24
 * 
 * @copyright Copyright (c) 2021
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
BTIM_TimeBaseInitTypeDef BTIM_TimeBaseInitStruct;
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

/**
 * @brief 
 * 
 * @return int32_t 
 */
int32_t main(void)
{
  /* System Clocks Configuration */
  RCC_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();    // ����PA12��ΪETR����������

  /* �����ô����źŵļ��ԣ������ö�ʱ����ģʽ������������󴥷� */
  BTIM_SelectInputTrigger(CW_BTIM1, BTIM_TS_ETR);
  BTIM_ETRConfig(CW_BTIM1, BTIM_ETR_Polarity_Negative, BTIM_EXT_FILTER_NONE);

  BTIM_TimeBaseInitStruct.BTIM_Mode = BTIM_Mode_TRIGGER;
  BTIM_TimeBaseInitStruct.BTIM_Period = 15624;
  BTIM_TimeBaseInitStruct.BTIM_Prescaler = BTIM_PRS_DIV512;
  BTIM_TimeBaseInitStruct.BTIM_OPMode = BTIM_OPMode_Single;

  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_TimeBaseInitStruct); 
  BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
 
  

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
  //   CW_SYSCTRL->APBEN2_f.BTIM = 1U;
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);
  __RCC_BTIM_CLK_ENABLE();
  __RCC_GPIOA_CLK_ENABLE();
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

  PA12_AFx_BTIMETR();
  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLDOWN;
  GPIO_InitStruct.Pins = GPIO_PIN_12;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);    // PA12

  PB09_SETLOW();
  PB08_SETHIGH();
  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_9 | GPIO_PIN_8;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.IT = GPIO_IT_FALLING;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pins = GPIO_PIN_1;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct); // PA1 -> KEY1
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq();
  NVIC_EnableIRQ(BTIM1_IRQn);
  NVIC_EnableIRQ(GPIOA_IRQn);
  __enable_irq();
}

void BTIM1_IRQHandleCallback(void)
{ 
	if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
	{
		BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    PB09_SETLOW();
  }
}
void BTIM2_IRQHandleCallback(void)
{
}
void BTIM3_IRQHandleCallback(void)
{
}

void GPIOA_IRQHandleCallback(void)
{
  if (CW_GPIOA->ISR_f.PIN1)    // �ж��У�ֱ�Ӳ����Ĵ������ٶȿ�
  {
    CW_GPIOA->ICR_f.PIN1 = 0;    // ���־
    PB09_SETHIGH();
  }
}

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
