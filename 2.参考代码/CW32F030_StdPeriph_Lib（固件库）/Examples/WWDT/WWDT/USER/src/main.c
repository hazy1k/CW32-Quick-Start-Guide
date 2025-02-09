/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 本示例，系统时钟由HSI提供，通过PLL倍频到48MHz。
 * -配置WWDT喂狗窗口246~350ms，允许预溢出中断
 * -配置系统节拍配置为每 250 ms 产生一个中断，中断复位程序中闪烁 LED1（PB9），并
 * 喂狗。
 * -PA1 引脚连接的按钮S1配置为在其下降沿上产生中断
 * -在 NVIC 中，将PA1对应中断向量且优先级等于0，并且 SysTick 中断向量优先级设置
 * 为 1（PA1的中断优先于 SysTick 中断）。
 * - 按钮S1（PA1）用于模拟软件故障：S1按下后，进入对应的中断服务程序，且中断标志
 * 位不清零，CPU不会进入入系统节拍中断例程，因此不会重载WWDT计数器。
 * 当WWDT计数器预溢出时，WWDT将产生一个POV中断。
 * 程序发生WWDT中断后，LED1长亮，延后2个喂狗周期后，系统复位。
 * 若系统发生过WWDT复位，则LED2亮。
 * @version 0.1
 * @date 2021-04-22
 * 
 * @copyright Copyright (c) 2021
 *
 *代码许可和免责信息
 *武汉力源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
 *生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉力源半
 *导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
 *含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
 *或条件。
 *无论何种情形，武汉力源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
 *即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
 *或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
 *损失。
 *某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
 *全部上述排除或限制可能并不适用于您。
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
static void WWDT_Config(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
LED_InitTypeDef LED1, LED2;
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
  RCC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  /* LED 初始化 */
  LED1.LED_Port = CW_GPIOB;
  LED1.LED_Pin = GPIO_PIN_9;
  LED_Init(&LED1);
  LED1.LED_On(&LED1);

  LED2.LED_Port = CW_GPIOB;
  LED2.LED_Pin = GPIO_PIN_8;
  LED_Init(&LED2);
  LED2.LED_Off(&LED2);

  /* 节拍定时器初始化 */
  SysTick_Config(SystemCoreClock / 1000 * 250);   // 250ms 产生一次中断

  /* 配置WWDT */
  WWDT_Config();

  /* NVIC Configuration */
  NVIC_Configuration();

  if (RCC_GetRstFlag(RCC_FLAG_WWDTRST))
  {
    LED2.LED_On(&LED2);
  }

  while(1)
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
  /* 0. HSI使能并校准 */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. 设置HCLK和PCLK的分频系数　*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. 使能PLL，通过HSI倍频到48MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 6);     // HSI 默认输出频率8MHz
  RCC_PLL_OUT();
  
  ///< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_2);   
    
   
  /* 3. 时钟切换到PLL */
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
  GPIO_InitTypeDef GPIO_InitStruct;

  __RCC_GPIOA_CLK_ENABLE();

  /* 配置PA1为按键输入 */

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
  NVIC_SetPriority(SysTick_IRQn, 1);
  NVIC_SetPriority(GPIOA_IRQn, 0);
  NVIC_SetPriority(WDT_IRQn, 0);
  NVIC_EnableIRQ(WDT_IRQn);
  NVIC_EnableIRQ(GPIOA_IRQn);
  __enable_irq();  
}

static void WWDT_Config(void)
{
  WWDT_InitTypeDef WWDT_InitStruct = {0};

  __RCC_WWDT_CLK_ENABLE();

  WWDT_InitStruct.WWDT_CounterValue = 0x7F;
  WWDT_InitStruct.WWDT_ITState = ENABLE;
  WWDT_InitStruct.WWDT_Prescaler = WWDT_Prescaler_DIV262144;    // 183Hz,T=5.46ms
  WWDT_InitStruct.WWDT_WindowValue = 82;
  WWDT_Init(&WWDT_InitStruct);

  WWDT_Cmd();
}

void SysTick_Handler(void)
{
  WWDT_FEED(0x7F);
  LED1.LED_Toggle(&LED1);
}

void WDT_IRQHandler(void)
{
  /* USER CODE BEGIN */
  static uint8_t flag = 0;
  WWDT_ClearFlag();
  if (flag < 2)
  {
    WWDT_FEED(0x7F);
    LED1.LED_On(&LED1);    // 首次中断后，喂2次狗，LED1常亮，指示要复位
    flag++;
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
    void
    assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

