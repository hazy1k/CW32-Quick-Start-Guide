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
static void BTIM_Config(void);
static void LED1_Config(void);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
BTIM_TimeBaseInitTypeDef BTIM_InitStructure;
LED_InitTypeDef LED1;
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
  RCC_Configuration(); //

  /* GPIO Configuration */
  GPIO_Configuration();

  LED1_Config();

  BTIM_Config();
  /* NVIC Configuration */
  NVIC_Configuration();

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
  /* 0. HSEʹ�� */
  __RCC_GPIOF_CLK_ENABLE();
  PF00_ANALOG_ENABLE();
  PF01_ANALOG_ENABLE();

  RCC_HSE_Enable(RCC_HSE_MODE_OSC, 16000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE);
  RCC_HSE_OUT();

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSEOSC, 16000000, 4); //
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
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  __RCC_GPIOA_CLK_ENABLE();

  PA07_AFx_BTIM1TOGP();
  PA09_AFx_BTIM1TOGP();
  PA10_AFx_BTIM1TOGN();
  GPIO_InitStruct.IT = GPIO_IT_NONE;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7 | GPIO_PIN_9 | GPIO_PIN_10;
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
  NVIC_EnableIRQ(BTIM1_IRQn);
  __enable_irq();
}

static void BTIM_Config(void)
{
  __RCC_BTIM_CLK_ENABLE();

  BTIM_InitStructure.BTIM_Mode = BTIM_Mode_TIMER;
  BTIM_InitStructure.BTIM_OPMode = BTIM_OPMode_Repetitive;
  BTIM_InitStructure.BTIM_Prescaler = BTIM_PRS_DIV1024; // 64MHz / 1024��Ƶ=62500Hz
  BTIM_InitStructure.BTIM_Period = 62500 / 20 - 1;      // 20Hz���һ�Σ�����TOG�õ�10Hz���

  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_InitStructure);
  BTIM_OutputToggleCmd(CW_BTIM1, ENABLE);
  BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
  BTIM_Cmd(CW_BTIM1, ENABLE);
}

static void LED1_Config(void)
{
  LED1.LED_Port = CW_GPIOB;
  LED1.LED_Pin = GPIO_PIN_9;

  LED_Init(&LED1);
}
void BTIM1_IRQHandleCallback(void)
{
  if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
  {
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    LED1.LED_Toggle(&LED1);
  }
}
void BTIM2_IRQHandleCallback(void)
{
}
void BTIM3_IRQHandleCallback(void)
{
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
