/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief ��ʾ��˵����ζ��ڸ��� IWDT ���ؼ��������Լ����ģ��������ϣ�ʹ��̺�
 * ��ʱ��ε���ʱ���� MCU IWDT �жϡ�
 * -�������Ź���ʱ����Ϊ 250 ms
 * -ϵͳ��������Ϊÿ 250 ms ����һ���ж�
 * -��ϵͳ�����жϷ��������У������ض������Ź��������Է�ֹ�������Ź���λ�����ҽ�
 * ��˸ LED1��PB9��
 * -PA1 �������ӵİ�ťS1����Ϊ�����½����ϲ����ж�
 * -�� NVIC �У���PA1��Ӧ�ж����������ȼ�����0������ SysTick �ж��������ȼ�����
 * Ϊ 1��PA1���ж������� SysTick �жϣ���
 * - ��ťS1��PA1������ģ��������ϣ�S1���º󣬽����Ӧ���жϷ���������жϱ�־
 * λ�����㣬CPU���������ϵͳ�����ж����̣���˲������ض������Ź���������
 * ���������Ź����������ʱ���������Ź�������һ���жϡ�
 * ������IWDT�жϺ�LED2��ת��
 * @version 0.1
 * @date 2021-04-22
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
#define IWDT_DURATION 250 // 250ms
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/
LED_InitTypeDef LED1, LED2;

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
 * @brief 
 * @return int32_t 
 */
int32_t main(void)
{
  IWDT_InitTypeDef IWDT_InitStruct = {0};

  /* System Clocks Configuration */
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  SysTick_Config(SystemCoreClock / 1000 * 250); // 250ms

  LED1.LED_Port = CW_GPIOB;
  LED1.LED_Pin = GPIO_PIN_9;
  LED_Init(&LED1);
  LED1.LED_Off(&LED1);

  LED2.LED_Port = CW_GPIOB;
  LED2.LED_Pin = GPIO_PIN_8;
  LED_Init(&LED2);
  LED2.LED_Off(&LED2);

  IWDT_InitStruct.IWDT_ITState = ENABLE;
  IWDT_InitStruct.IWDT_OverFlowAction = IWDT_OVERFLOW_ACTION_INT; //���������жϲ���λ
  IWDT_InitStruct.IWDT_Pause = IWDT_SLEEP_PAUSE;
  IWDT_InitStruct.IWDT_Prescaler = IWDT_Prescaler_DIV4;
  IWDT_InitStruct.IWDT_ReloadValue = (IWDT_FREQ >> 2) / 1000 * 280 - 1; // ����IWDT��ʱ��ΪRC10K, ����Ϊ280ʵ�����ʱ��Ϊ256ms����
  IWDT_InitStruct.IWDT_WindowValue = 0xFFF;

  IWDT_Init(&IWDT_InitStruct);

  IWDT_Cmd();

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
  /* 0. HSIʹ�ܲ�У׼ */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��48MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 6); // HSI Ĭ�����Ƶ��8MHz
  RCC_PLL_OUT();

  ///< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_2);

  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(48000000);

  /* 4. ��������ʱ�� */
  __RCC_IWDT_CLK_ENABLE(); //ʹ��IWDTģ��
  __RCC_GPIOB_CLK_ENABLE();
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

  /* ����PA1Ϊ�������� */

  GPIO_InitStruct.IT = GPIO_IT_FALLING;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pins = GPIO_PIN_1;
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
  NVIC_EnableIRQ(WDT_IRQn);
  NVIC_SetPriority(GPIOA_IRQn, 0);
  NVIC_SetPriority(SysTick_IRQn, 1);
  NVIC_EnableIRQ(GPIOA_IRQn);
  __enable_irq();
}

void GPIOA_IRQHandlerCallback(void)
{
}

void SysTick_HandlerCallback(void)
{
  LED1.LED_Toggle(&LED1);
  IWDT_Refresh();
}

void WDT_IRQHandlerCallback(void)
{
  if (CW_IWDT->SR & IWDT_SR_OV_Msk)
  {
    IWDT_ClearOVFlag();
    LED2.LED_Toggle(&LED2);
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
