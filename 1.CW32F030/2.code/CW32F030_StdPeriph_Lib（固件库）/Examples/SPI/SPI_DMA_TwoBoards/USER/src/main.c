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
#define  SPI_MASTER //主机模式
//#define  SPI_SLAVE  //从机模式

//SPI
#define  SPIx                           CW_SPI2
#define  SPIx_CLK                       RCC_APB1_PERIPH_SPI2
#define  SPIx_APBClkENx                 RCC_APBPeriphClk_Enable1

//SPI GPIO
#define  SPIx_SCK_GPIO_CLK              RCC_AHB_PERIPH_GPIOA
#define  SPIx_SCK_GPIO_PORT             CW_GPIOA 
#define  SPIx_SCK_GPIO_PIN              GPIO_PIN_2
#define  SPIx_SCK_AF()                  PA02_AFx_SPI2SCK()

#define  SPIx_MISO_GPIO_CLK             RCC_AHB_PERIPH_GPIOA
#define  SPIx_MISO_GPIO_PORT            CW_GPIOA 
#define  SPIx_MISO_GPIO_PIN             GPIO_PIN_0
#define  SPIx_MISO_AF()                 PA00_AFx_SPI2MISO()

#define  SPIx_MOSI_GPIO_CLK             RCC_AHB_PERIPH_GPIOA
#define  SPIx_MOSI_GPIO_PORT            CW_GPIOA 
#define  SPIx_MOSI_GPIO_PIN             GPIO_PIN_1
#define  SPIx_MOSI_AF()                 PA01_AFx_SPI2MOSI()

#define  SPIx_CS_GPIO_CLK               RCC_AHB_PERIPH_GPIOA
#define  SPIx_CS_GPIO_PORT              CW_GPIOA 
#define  SPIx_CS_GPIO_PIN               GPIO_PIN_3
#define  SPIx_CS_AF()                   PA03_AFx_SPI2CS()

//CS LOW or HIGH
#define  SPIx_CS_LOW()     		          PA03_SETLOW()
#define  SPIx_CS_HIGH()    		          PA03_SETHIGH()

//DMA
#define  SPIx_RX_DMACHANNEL             CW_DMACHANNEL1
#define  SPIx_TX_DMACHANNEL             CW_DMACHANNEL2
#define  SPIx_DMA_RxTrigSource          DMA_HardTrig_SPI2_RXBufferNE
#define  SPIx_DMA_TxTrigSource          DMA_HardTrig_SPI2_TXBufferE
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
void DMA_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                      0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                      0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                      0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                      0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x55};
uint8_t RxBuffer[BufferSize];
volatile TestStatus TransferStatus = FAILED; 
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

  //配置DMA
  DMA_Configuration();

  //配置SPI
  SPI_Configuration(); 

  #ifdef SPI_MASTER
  //拉低CS，选中SPI从机
  SPIx_CS_LOW();
  #endif

  #ifdef SPI_SLAVE
  //清空发送缓冲区和移位寄存器
  SPI_FlushSendBuff(SPIx);
  #endif
    
  //使能SPI DMA TX and RX
  SPI_DMACmd(SPIx, SPI_DMAReq_Tx | SPI_DMAReq_Rx, ENABLE);
  
  while(1)
  {  
    if(RxBuffer[BufferSize-1] == 0x55) //传输完成
    {
      #ifdef SPI_MASTER
      SPIx_CS_HIGH();
      #endif
      //LED1亮
      PB01_SETHIGH(); 
      //检查收发数据一致性
      TransferStatus = Buffercmp(RxBuffer, TxBuffer, BufferSize);
      if(TransferStatus == PASSED) //PASSED
      {
        //LED2亮
        PA07_SETHIGH(); 
      }
    }
  }
}

/**
 * @brief 配置RCC
 * 
 */
void RCC_Configuration(void)
{
  //外设时钟使能
  RCC_AHBPeriphClk_Enable(SPIx_SCK_GPIO_CLK | SPIx_MISO_GPIO_CLK | SPIx_MOSI_GPIO_CLK | SPIx_CS_GPIO_CLK |\
                          RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB | RCC_AHB_PERIPH_FLASH, ENABLE);
  SPIx_APBClkENx(SPIx_CLK, ENABLE);

  //SYSCLK = PLL = 64MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, RCC_PLL_MUL_8);
  FLASH_SetLatency(FLASH_Latency_3); //频率大于48M需要配置FlashWait=3
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL); 
}

/**
 * @brief 配置GPIO
 * 
 */
void GPIO_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
 
  //SPI SCK MOSI MISO 复用
  SPIx_SCK_AF();
  SPIx_MISO_AF();  
  SPIx_MOSI_AF();  

  #ifdef SPI_MASTER
  //推挽输出 SCK MOSI CS
  GPIO_InitStructure.Pins = SPIx_SCK_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = SPIx_MOSI_GPIO_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = SPIx_CS_GPIO_PIN;
  GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);

  //浮空输入 MISO
  GPIO_InitStructure.Pins = SPIx_MISO_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

  //拉高CS
  SPIx_CS_HIGH();
  #endif

  #ifdef SPI_SLAVE
  //SPI CS复用
  SPIx_CS_AF(); 

  //推挽输出 MISO
  GPIO_InitStructure.Pins = SPIx_MISO_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(SPIx_MISO_GPIO_PORT, &GPIO_InitStructure);

  //浮空输入 SCK MOSI CS
  GPIO_InitStructure.Pins = SPIx_SCK_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(SPIx_SCK_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIx_MOSI_GPIO_PIN;
  GPIO_Init(SPIx_MOSI_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIx_CS_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_Init(SPIx_CS_GPIO_PORT, &GPIO_InitStructure);
  #endif

  //PB1 LED1 / PA7 LED2
  GPIO_InitStructure.Pins = GPIO_PIN_1;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
  
  GPIO_InitStructure.Pins = GPIO_PIN_7;
  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
  
  //LED灭
  PB01_SETLOW();
  PA07_SETLOW();  
}

/**
 * @brief 配置DMA
 * 
 */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  //初始化SPI DMA TX 
  DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;
  DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;
  DMA_InitStructure.TrigMode = DMA_HardTrig;
  DMA_InitStructure.HardTrigSource = SPIx_DMA_TxTrigSource;
  DMA_InitStructure.DMA_TransferCnt = BufferSize;
  DMA_InitStructure.DMA_SrcAddress = (uint32_t)TxBuffer;
  DMA_InitStructure.DMA_DstAddress = (uint32_t)&SPIx->DR;
  DMA_Init(SPIx_TX_DMACHANNEL, &DMA_InitStructure);
  DMA_Cmd(SPIx_TX_DMACHANNEL, ENABLE);
    
  //初始化SPI DMA RX
  DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix;
  DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase;
  DMA_InitStructure.TrigMode = DMA_HardTrig;
  DMA_InitStructure.HardTrigSource = SPIx_DMA_RxTrigSource;
  DMA_InitStructure.DMA_TransferCnt = BufferSize;
  DMA_InitStructure.DMA_SrcAddress = (uint32_t)&SPIx->DR;
  DMA_InitStructure.DMA_DstAddress = (uint32_t)RxBuffer;
  DMA_Init(SPIx_RX_DMACHANNEL, &DMA_InitStructure);
  DMA_Cmd(SPIx_RX_DMACHANNEL, ENABLE);
}

/**
 * @brief 配置SPI 16Mbps
 * 
 */
void SPI_Configuration()
{
  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // 双线全双工
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // 主机模式
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // 帧数据长度为8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            // 时钟空闲电平为低
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // 第一个边沿采样
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // 片选信号由SSI寄存器控制
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;    // 波特率为PCLK的4分频
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // 最高有效位 MSB 收发在前
  SPI_InitStructure.SPI_Speed = SPI_Speed_High;                         // 高速SPI

  #ifdef SPI_SLAVE
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                          // 从机模式
  SPI_InitStructure.SPI_NSS = SPI_NSS_Hard;                             // SPI_CS引脚电平决定本机是否被选中
  #endif

  SPI_Init(SPIx, &SPI_InitStructure);
  SPI_Cmd(SPIx, ENABLE); 
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

