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
//SPIy MASTER
#define  SPIy                           CW_SPI1
#define  SPIy_CLK                       RCC_APB2_PERIPH_SPI1
#define  SPIy_APBClkENx                 RCC_APBPeriphClk_Enable2

//SPIy GPIO
#define  SPIy_GPIO_CLK                  RCC_AHB_PERIPH_GPIOA
#define  SPIy_GPIO                      CW_GPIOA 
#define  SPIy_SCK_PIN                   GPIO_PIN_10
#define  SPIy_MOSI_PIN                  GPIO_PIN_12
#define  SPIy_AF_SCK                    PA10_AFx_SPI1SCK()
#define  SPIy_AF_MOSI                   PA12_AFx_SPI1MOSI()

//SPIz SLAVE
#define  SPIz                           CW_SPI2
#define  SPIz_CLK                       RCC_APB1_PERIPH_SPI2
#define  SPIz_APBClkENx                 RCC_APBPeriphClk_Enable1

//SPIz GPIO
#define  SPIz_GPIO_CLK                  RCC_APB2Periph_GPIOB 
#define  SPIz_GPIO                      CW_GPIOB
#define  SPIz_SCK_PIN                   GPIO_PIN_13
#define  SPIz_MOSI_PIN                  GPIO_PIN_15 
#define  SPIz_AF_SCK                    PB13_AFx_SPI2SCK()
#define  SPIz_AF_MOSI                   PB15_AFx_SPI2MOSI()

//中断
#define  SPIy_IRQ                       SPI1_IRQn
#define  SPIz_IRQ                       SPI2_IRQn

#define  BufferSize                     ARRAY_SZ(TxBuffer)
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
void NVIC_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                      0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
                      0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 
                      0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
                      0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83,
                      0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A};
uint8_t RxBuffer[BufferSize];
uint8_t TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus = FAILED; 
/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
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

  //配置SPI
  SPI_Configuration(); 

  //配置NVIC
  NVIC_Configuration();

  //使能 SPIz RXNE 中断
  SPI_ITConfig(SPIz, SPI_IT_RXNE, ENABLE);

  //软件NSS，选中SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Reset);

  //使能 SPIy TXE 中断
  SPI_ITConfig(SPIy, SPI_IT_TXE, ENABLE);
    
  //等待收发完成
  while(RxCounter < BufferSize);

  //释放SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Set);

  //检查收发数据一致性
  TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

  if(TransferStatus == PASSED) //PASSED
  {
    //LED1亮
    PB09_SETHIGH();
  }
  else //FAILED
  {
    //LED2亮
    PB08_SETHIGH();  
  }
  
  while(1)
  {  

  }
}

/**
 * @brief 配置RCC
 * 
 */
void RCC_Configuration(void)
{
  //SYSCLK = HSI = 24MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV2);

  //外设时钟使能
  RCC_AHBPeriphClk_Enable(SPIy_GPIO_CLK | SPIy_GPIO_CLK | RCC_AHB_PERIPH_GPIOB, ENABLE);
  SPIy_APBClkENx(SPIy_CLK, ENABLE);
  SPIz_APBClkENx(SPIz_CLK, ENABLE);
}

/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
 
  //SPI SCK MOSI 复用
  SPIy_AF_SCK;
  SPIy_AF_MOSI; 
  SPIz_AF_SCK;
  SPIz_AF_MOSI;     

  //推挽输出
  GPIO_InitStructure.Pins = SPIy_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = SPIy_MOSI_PIN;
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);

  //浮空输入
  GPIO_InitStructure.Pins = SPIz_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIz_MOSI_PIN;
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  //PB9 LED1 / PB8 LED2
  GPIO_InitStructure.Pins = GPIO_PIN_9 | GPIO_PIN_8;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

  //LED灭
  PB09_SETLOW();
  PB08_SETLOW();  
}

/**
 * @brief 配置RCC 3Mbit/s
 * 
 */
void SPI_Configuration()
{
  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_TxOnly;         // 单工单发
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            // 时钟空闲电平为低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // 第一个边沿采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // 波特率为PCLK的8分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
  SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // 低速SPI
  
  SPI_Init(SPIy, &SPI_InitStructure);

  SPI_InitStructure.SPI_Direction = SPI_Direction_1Line_RxOnly;         // 单工单收
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                          // 从机模式
  SPI_Init(SPIz, &SPI_InitStructure);

  SPI_Cmd(SPIy, ENABLE); 
  SPI_Cmd(SPIz, ENABLE); 
}

/**
 * @brief 配置NVIC
 * 
 */
void NVIC_Configuration(void)
{
  //优先级，无优先级分组
  NVIC_SetPriority(SPIy_IRQ, 1);
  NVIC_SetPriority(SPIz_IRQ, 0);
  //SPI中断使能
  NVIC_EnableIRQ(SPIy_IRQ);
  NVIC_EnableIRQ(SPIz_IRQ);
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

