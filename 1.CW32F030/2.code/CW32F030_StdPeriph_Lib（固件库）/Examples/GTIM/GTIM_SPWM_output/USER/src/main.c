
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
 * @brief  使用HSI=48MHz，PCLK = 48M，（频率大于24M 需要配置FlashWait=2）
 *         GTIM1定时，48MHz 输入, PA6（CH1）输出 SPWM
 *    
 * @return int32_t 
 */
int32_t main(void)
{
  GTIM_InitTypeDef GTIM_InitStruct; 
  
  /* System Clocks Configuration */
  RCC_Configuration();  

  RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH , ENABLE);   // 打开FLASH时钟
  FLASH_SetLatency(FLASH_Latency_2);    // 频率大于24M需要配置FlashWait=2
	RCC_HSI_Enable(RCC_HSIOSC_DIV1);     // 修改HSI分频为1，PCLK = HSI

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();  

  GTIM_InitStruct.Mode = GTIM_MODE_TIME;
  GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1;
  //GTIM_InitStruct.ReloadValue = 60100UL >> 4;    // PWM频率为 48M/60100*16=12k, SPWM频率 = 12k/2/200=30Hz
  GTIM_InitStruct.ReloadValue = 60100UL - 1;    // PWM频率为 48M/60100=800Hz, SPWM周期 = 800/2/1000= 0.4Hz
  GTIM_InitStruct.ToggleOutState = DISABLE;

  GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
  GTIM_OCInit(CW_GTIM1, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_HIGH);
  GTIM_SetCompare1(CW_GTIM1, 0);
  GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);

  GTIM_Cmd(CW_GTIM1, ENABLE); 
  
  while(1)
  {
    /* 中断服务程序见interrupts_cw32f030.c */
  }
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  __RCC_GTIM1_CLK_ENABLE();   // GTIM1时钟使能
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{  
  /* PA6 PWM 输出 */
  __RCC_GPIOA_CLK_ENABLE();
  PA06_AFx_GTIM1CH1();
  PA06_DIR_OUTPUT();
  PA06_DIGTAL_ENABLE();
  
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

