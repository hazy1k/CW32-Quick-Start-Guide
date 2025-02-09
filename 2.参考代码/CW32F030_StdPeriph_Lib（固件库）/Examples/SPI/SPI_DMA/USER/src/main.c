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
#define  SPIy_MISO_PIN                  GPIO_PIN_11
#define  SPIy_MOSI_PIN                  GPIO_PIN_12
#define  SPIy_AF_SCK                    PA10_AFx_SPI1SCK()
#define  SPIy_AF_MISO                   PA11_AFx_SPI1MISO()
#define  SPIy_AF_MOSI                   PA12_AFx_SPI1MOSI()

//SPIz SLAVE
#define  SPIz                           CW_SPI2
#define  SPIz_CLK                       RCC_APB1_PERIPH_SPI2
#define  SPIz_APBClkENx                 RCC_APBPeriphClk_Enable1

//SPIz GPIO
#define  SPIz_GPIO_CLK                  RCC_APB2Periph_GPIOB 
#define  SPIz_GPIO                      CW_GPIOB
#define  SPIz_SCK_PIN                   GPIO_PIN_13
#define  SPIz_MISO_PIN                  GPIO_PIN_14
#define  SPIz_MOSI_PIN                  GPIO_PIN_15
#define  SPIz_AF_SCK                    PB13_AFx_SPI2SCK()
#define  SPIz_AF_MISO                   PB14_AFx_SPI2MISO()
#define  SPIz_AF_MOSI                   PB15_AFx_SPI2MOSI()

//DMA
#define  SPIz_RX_DMACHANNEL             CW_DMACHANNEL1
#define  SPIz_TX_DMACHANNEL             CW_DMACHANNEL2
#define  SPIz_DMA_RxTrigSource          DMA_HardTrig_SPI2_RXBufferNE
#define  SPIz_DMA_TxTrigSource          DMA_HardTrig_SPI2_TXBufferE
#define  BufferSize                     ARRAY_SZ(TxBuffer1)
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
uint8_t TxBuffer1[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
                       0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
                       0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15,
                       0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C,
                       0x1D, 0x1E, 0x1F, 0x20, 0x21, 0x22, 0x23};
uint8_t TxBuffer2[] = {0x61, 0x62, 0x63, 0x64, 0x65, 0x66, 0x67,
                       0x68, 0x69, 0x6A, 0x6B, 0x6C, 0x6D, 0x6E,
                       0x6F, 0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 
                       0x76, 0x77, 0x78, 0x79, 0x7A, 0x7B, 0x7C,
                       0x7D, 0x7E, 0x7F, 0x80, 0x81, 0x82, 0x83};
uint8_t RxBuffer1[BufferSize];
uint8_t RxBuffer2[BufferSize];
uint8_t TxCounter = 0, RxCounter = 0;
volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = FAILED; 
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
  //����RCC
  RCC_Configuration();  

  //����GPIO
  GPIO_Configuration();

  //����DMA
  DMA_Configuration();

  //����SPI
  SPI_Configuration(); 
    
  /**********************SPIy Loop Send -- SPIz DMA Receive*******************/
  //ʹ��SPIz DMA RX
  SPI_DMACmd(SPIz, SPI_DMAReq_Rx, ENABLE);
  
  //���NSS��ѡ��SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Reset);
  while(TxCounter < BufferSize)
  {
    //SPIy ����
    while(SPI_GetFlagStatus(SPIy, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIy, TxBuffer1[TxCounter++]);

    //SPIZͨ��DMA����
  }
  //�ȴ�DMA�������
  while(DMA_GetFlagStatus(SPIz_RX_DMACHANNEL) != DMA_CHANNEL_STATUS_TRANSCOMPLETE);
  //�ͷ�SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Set);

  /*****************SPIy Loop Send and Receive -- SPIz DMA Send****************/
  TxCounter = 0;
  //��DR�����RXNE��־λ
  SPI_ReceiveData(SPIy);
  //ʧ��SPIz DMA RX
  SPI_DMACmd(SPIz, SPI_DMAReq_Rx, DISABLE);
  //��շ��ͻ���������λ�Ĵ���
  SPI_FlushSendBuff(SPIz);
  //ʹ��SPIz DMA TX
  SPI_DMACmd(SPIz, SPI_DMAReq_Tx, ENABLE);

  //���NSS��ѡ��SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Reset);
  while(TxCounter < BufferSize)
  {
    //SPIy ����
    while(SPI_GetFlagStatus(SPIy, SPI_FLAG_TXE) == RESET);
    SPI_SendData(SPIy, TxBuffer1[TxCounter++]);

    //SPIZͨ��DMA����

    //SPIy ����
    while(SPI_GetFlagStatus(SPIy, SPI_FLAG_RXNE) == RESET);
    RxBuffer1[RxCounter++] = SPI_ReceiveData(SPIy);
  }
  //�ͷ�SPIz
  SPI_NSSInternalSoftwareConfig(SPIz, SPI_NSSInternalSoft_Set);

  //����շ�����һ����
  TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, BufferSize);
  TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, BufferSize);

  if(TransferStatus1 == PASSED && TransferStatus2 == PASSED) //PASSED
  {
    //LED1��
    PB09_SETHIGH();
  }
  else //FAILED
  {
    //LED2��
    PB08_SETHIGH();  
  }
  
  while(1)
  {  

  }
}

/**
 * @brief ����RCC
 * 
 */
void RCC_Configuration(void)
{
  //SYSCLK = HSI = 24MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV2);

  //����ʱ��ʹ��
  RCC_AHBPeriphClk_Enable(SPIy_GPIO_CLK | SPIy_GPIO_CLK | RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOB, ENABLE);
  SPIy_APBClkENx(SPIy_CLK, ENABLE);
  SPIz_APBClkENx(SPIz_CLK, ENABLE);
}

/**
 * @brief ����GPIO
 * 
 */
void GPIO_Configuration(void)
{  
  GPIO_InitTypeDef GPIO_InitStructure;
 
  //SPI SCK MOSI MISO ����
  SPIy_AF_SCK;
  SPIy_AF_MISO;  
  SPIy_AF_MOSI; 
  SPIz_AF_SCK;
  SPIz_AF_MISO;  
  SPIz_AF_MOSI;     

  //�������
  GPIO_InitStructure.Pins = SPIy_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = SPIy_MOSI_PIN;
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIz_MISO_PIN;
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  //��������
  GPIO_InitStructure.Pins = SPIz_SCK_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIz_MOSI_PIN;
  GPIO_Init(SPIz_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = SPIy_MISO_PIN;
  GPIO_Init(SPIy_GPIO, &GPIO_InitStructure);

  //PB9 LED1 / PB8 LED2
  GPIO_InitStructure.Pins = GPIO_PIN_9 | GPIO_PIN_8;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

  //LED��
  PB09_SETLOW();
  PB08_SETLOW();  
}

/**
 * @brief ����DMA
 * 
 */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  //��ʼ��SPIz DMA TX 
  DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;
  DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;
  DMA_InitStructure.TrigMode = DMA_HardTrig;
  DMA_InitStructure.HardTrigSource = SPIz_DMA_TxTrigSource;
  DMA_InitStructure.DMA_TransferCnt = BufferSize;
  DMA_InitStructure.DMA_SrcAddress = (uint32_t)TxBuffer2;
  DMA_InitStructure.DMA_DstAddress = (uint32_t)&SPIz->DR;
  DMA_Init(SPIz_TX_DMACHANNEL, &DMA_InitStructure);
  DMA_Cmd(SPIz_TX_DMACHANNEL, ENABLE);
    
  //��ʼ��SPIz DMA RX
  DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix;
  DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase;
  DMA_InitStructure.TrigMode = DMA_HardTrig;
  DMA_InitStructure.HardTrigSource = SPIz_DMA_RxTrigSource;
  DMA_InitStructure.DMA_TransferCnt = BufferSize;
  DMA_InitStructure.DMA_SrcAddress = (uint32_t)&SPIz->DR;
  DMA_InitStructure.DMA_DstAddress = (uint32_t)RxBuffer2;
  DMA_Init(SPIz_RX_DMACHANNEL, &DMA_InitStructure);
  DMA_Cmd(SPIz_RX_DMACHANNEL, ENABLE);
}

/**
 * @brief ����SPI 3Mbit/s
 * 
 */
void SPI_Configuration()
{
  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // ˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // ����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // ֡���ݳ���Ϊ8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                            // ʱ�ӿ��е�ƽΪ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                          // ��һ�����ز���
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // Ƭѡ�ź���SSI�Ĵ�������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // ������ΪPCLK��8��Ƶ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // �����Чλ MSB �շ���ǰ
  SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // ����SPI
  
  SPI_Init(SPIy, &SPI_InitStructure);

  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                         // �ӻ�ģʽ
  SPI_Init(SPIz, &SPI_InitStructure);

  SPI_Cmd(SPIy, ENABLE); 
  SPI_Cmd(SPIz, ENABLE); 
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

