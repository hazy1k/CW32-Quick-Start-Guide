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
typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
void RCC_Configuration(void);
void GPIO_Configuration(void);
void SPI_Configuration(void);


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
 ** SPI FLASH(W25Q64)
 ** Success LED1亮
 ** Error   LED2亮
 **
 ** 引脚连接
 ** SPI SCK  -- FLASH CLK
 ** SPI MOSI -- FLASH DI
 ** SPI MISO -- FLASH DO
 ** SPI CS   -- FLASH NCS
 **
 ******************************************************************************/
int32_t main(void)
{
  //配置RCC
  RCC_Configuration();  

  //配置GPIO
  GPIO_Configuration();
  
  LogInit();
  
  printf("Log print is OK.\r\n");
  SPI_Configuration();
  SPI_NSSInternalSoftwareConfig(CW_SPI2, SPI_NSSInternalSoft_Reset);
  SPI_SendData(CW_SPI2, 0x66);
  for (uint32_t i = 1; i<150; i++);
  SPI_NSSInternalSoftwareConfig(CW_SPI2, SPI_NSSInternalSoft_Set);
  for (uint32_t i = 1; i<500; i++);
  SPI_NSSInternalSoftwareConfig(CW_SPI2, SPI_NSSInternalSoft_Reset);
  SPI_SendData(CW_SPI2, 0x99);
  for (uint32_t i = 1; i<150; i++);
  SPI_NSSInternalSoftwareConfig(CW_SPI2, SPI_NSSInternalSoft_Set);
    
 
  
  if (sfud_init() == SFUD_SUCCESS)
  {
  }
  

  while(1)
  {  

  }
}

/**
 * @brief 配置RCC 时钟源HSI 频率8MHz PLL 8倍频，SYSCLK=PCLK=HCLK=64MHz
 * 
 * 
 */
void RCC_Configuration(void)
{
  /* 0. HSI使能并校准 */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. 设置HCLK和PCLK的分频系数　*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. 使能PLL，通过HSI倍频到64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     // HSI 默认输出频率8MHz
  RCC_PLL_OUT();
  
  ///< 当使用的时钟源HCLK大于48M,小于等于72MHz：设置FLASH 读等待周期为3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
       
  /* 3. 时钟切换到PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);
}

/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure = {0};

  __RCC_GPIOA_CLK_ENABLE();
  /*
  * SPI2_CS:   PA03
  * SPI2_MISO: PA00
  * SPI2_MOSI: PA01
  * SPI2_SCK:  PA02
  */

  PA03_AFx_SPI2CS();
  PA00_AFx_SPI2MISO();
  PA01_AFx_SPI2MOSI();
  PA02_AFx_SPI2SCK();

  GPIO_InitStructure.IT = GPIO_IT_NONE;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Pins = GPIO_PIN_3 | GPIO_PIN_1 | GPIO_PIN_2;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_InitStructure.Pins = GPIO_PIN_0;
  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
}

/**
 * @brief SPI 配置
 * 
 */
void SPI_Configuration(void)
{
  SPI_InitTypeDef SPI_InitStructure;

  __RCC_SPI2_CLK_ENABLE();

  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
  SPI_InitStructure.SPI_Speed = SPI_Speed_High;

  SPI_Init(CW_SPI2, &SPI_InitStructure);
  SPI_Cmd(CW_SPI2, ENABLE);
}



/**
 * @brief Compares two buffers
 * 
 * @param pBuffer1 :buffers to be compared
 * @param pBuffer2 :buffers to be compared
 * @param BufferLength :buffer's length
 * @return TestStatus 
 *     @arg PASSED: pBuffer1 identical to pBuffer2
 *     @arg FAILED: pBuffer1 differs from pBuffer2
 */
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength)
{
  while(BufferLength--)
  {
    if(*pBuffer1 != *pBuffer2)
    {
      return FAILED;
    }
    
    pBuffer1++;
    pBuffer2++;
  }

  return PASSED;  
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

