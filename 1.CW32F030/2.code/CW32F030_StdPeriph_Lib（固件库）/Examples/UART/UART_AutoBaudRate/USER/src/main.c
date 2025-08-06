/******************************************************************************/
/** \file main.c
 **
 ** A detailed description is available at
 ** @link Sample Group Some description @endlink
 **
 **   - 2021-03-12  1.0  xiebin First version for Device Driver Library of Module.
 **
 ******************************************************************************/
/*******************************************************************************
*
* 代码许可和免责信息
* 武汉力源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉力源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉力源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
//UARTx
#define  DEBUG_USARTx                   CW_UART1
#define  DEBUG_USART_CLK                RCC_APB2_PERIPH_UART1
#define  DEBUG_USART_APBClkENx          RCC_APBPeriphClk_Enable2
#define  DEBUG_USART_BaudRate           9600
#define  DEBUG_USART_UclkFreq           8000000

//UARTx GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9

//GPIO AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()

//中断
#define  DEBUG_USART_IRQ                UART1_IRQn

//GTIM
#define  DEBUG_GTIMx                    CW_GTIM1
#define  DEBUG_GTIM_CLK                 RCC_APB1_PERIPH_GTIM1
#define  DEBUG_GTIM_APBClkENx           RCC_APBPeriphClk_Enable1
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
void UART_Configuration(void);
void GTIM_Configuration(void);
void NVIC_Configuration(void);
void USART_SendString(UART_TypeDef* USARTx, char *String);
void AutoBaudRate(UART_TypeDef* USARTx, GTIM_TypeDef* GTIMx);

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
 ******************************************************************************/
int32_t main(void)
{
  //配置RCC
  RCC_Configuration();
  
  //配置GPIO
  GPIO_Configuration();
    
  //配置UART
  UART_Configuration();

  //配置GTIM
  GTIM_Configuration();

  //配置NVIC
  NVIC_Configuration();

  //使能UARTx RC中断
  USART_ITConfig(DEBUG_USARTx, USART_IT_RC, ENABLE);

  USART_SendString(DEBUG_USARTx, "\r\nCW32F030 UART AutoBaudRate\r\n");
  USART_SendString(DEBUG_USARTx, "\r\nPC发送以0xF8作为数据头的数据流\r\n");

  //自动波特率检测
  AutoBaudRate(DEBUG_USARTx, DEBUG_GTIMx);

  while(1)
  {  
    //中断收发
  }
}

/**
 * @brief 配置RCC
 * 
 */
void RCC_Configuration(void)
{
  //SYSCLK = HSI = 8MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  //外设时钟使能
  RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK, ENABLE);
  DEBUG_USART_APBClkENx(DEBUG_USART_CLK, ENABLE);
  DEBUG_GTIM_APBClkENx(DEBUG_GTIM_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //UART TX RX 复用 
  DEBUG_USART_AFTX;                     
  DEBUG_USART_AFRX;                     

  GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);
}

/**
 * @brief 配置UART
 * 
 */
void UART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
  USART_InitStructure.USART_Over = USART_Over_16;
  USART_InitStructure.USART_Source = USART_Source_PCLK;
  USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;
  USART_InitStructure.USART_StartBit = USART_StartBit_FE;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USARTx, &USART_InitStructure); 	
}

/**
 * @brief 配置GTIM
 * 
 */
void GTIM_Configuration(void)
{
  GTIM_InitTypeDef GTIM_InitStructure; 

  GTIM_InitStructure.Mode = GTIM_MODE_GATE;              //门控模式
  GTIM_InitStructure.OneShotMode = GTIM_COUNT_CONTINUE;
  GTIM_InitStructure.Prescaler = GTIM_PRESCALER_DIV1;    //PCLK计数
  GTIM_InitStructure.ReloadValue = 0xFFFF;
  GTIM_InitStructure.ToggleOutState = DISABLE;
  GTIM_TimeBaseInit(DEBUG_GTIMx, &GTIM_InitStructure);

  //ETR来源为DEBUG_USARTx的RXD信号
  CW_SYSCTRL->GTIMETR_f.GTIM1ETR = 1;

  //ETR极性及滤波器：低电平、无滤波
  GTIM_ETRConfig(DEBUG_GTIMx, GTIM_ETR_POL_FALL_LOW, GTIM_ETR_FILTER_NONE);
}

/**
 * @brief 配置NVIC
 * 
 */
void NVIC_Configuration(void)
{
  //优先级，无优先级分组
  NVIC_SetPriority(DEBUG_USART_IRQ, 0);
  //UARTx中断使能
  NVIC_EnableIRQ(DEBUG_USART_IRQ);
}

/**
 * @brief 发送字符串
 * 
 * @param USARTx :USARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、CW_UART3
 * @param String :待发送的字符串
 */
void USART_SendString(UART_TypeDef* USARTx, char *String)
{
  while(*String != '\0')
  {
    USART_SendData_8bit(USARTx, *String);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    String++;
  }
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXBUSY) == SET);
}

/**
 * @brief 自动波特率检测
 * 
 * @param USARTx :USARTx外设
 *        参数可以是:
 *           CW_UART1、CW_UART2、CW_UART3
 * @param USARTx :USARTx外设
 *        参数可以是:
 *           CW_GTIM1、CW_GTIM2、CW_GTIM3、CW_GTIM4
 * @note
 *    -16倍采样
 *    -GTIMx.CNT未作溢出处理
 */
void AutoBaudRate(UART_TypeDef* USARTx, GTIM_TypeDef* GTIMx)
{
  uint32_t temp;
  //失能RXEN
  USARTx->CR1_f.RXEN = 0;
  //使能RXD上升沿中断
  REGBITS_SET(DEBUG_USART_RX_GPIO_PORT->RISEIE, DEBUG_USART_RX_GPIO_PIN);
  //开启GTIM
  GTIMx->CR0_f.EN = 1;
  while(REGBITS_GET(DEBUG_USART_RX_GPIO_PORT->ISR, DEBUG_USART_RX_GPIO_PIN) == RESET);
  //失能RXD上升沿中断
  REGBITS_CLR(DEBUG_USART_RX_GPIO_PORT->RISEIE, DEBUG_USART_RX_GPIO_PIN);
  //关闭GTIM
  GTIMx->CR0_f.EN = 0;
  //清除中断标志
  REGBITS_CLR(DEBUG_USART_RX_GPIO_PORT->ICR, DEBUG_USART_RX_GPIO_PIN); 
  //计算BRRI、BRRF
  temp = ((GTIMx->CNT) + 2) / 4;
  USARTx->BRRI = (uint16_t)(temp >> 4);
  USARTx->BRRF = (uint16_t)(temp & 0x0F);
  //使能RXEN
  USARTx->CR1_f.RXEN = 1;
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

