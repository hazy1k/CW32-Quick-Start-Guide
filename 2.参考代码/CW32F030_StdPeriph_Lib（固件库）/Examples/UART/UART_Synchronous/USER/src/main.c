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
//UARTx SPIx
#define  DEBUG_USARTx                   CW_UART1
#define  DEBUG_USART_CLK                RCC_APB2_PERIPH_UART1
#define  DEBUG_USART_APBClkENx          RCC_APBPeriphClk_Enable2

#define  DEBUG_SPIx                     CW_SPI1
#define  DEBUG_SPI_CLK                  RCC_APB2_PERIPH_SPI1
#define  DEBUG_SPI_APBClkENx            RCC_APBPeriphClk_Enable2

//UARTx GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9 
// #define  DEBUG_USART_CS_GPIO_PORT       CW_GPIOA
// #define  DEBUG_USART_CS_GPIO_PIN        GPIO_PIN_0

//SPIx GPIO
#define  DEBUG_SPI_GPIO_CLK             RCC_AHB_PERIPH_GPIOA
#define  DEBUG_SPI_SCK_GPIO_PORT        CW_GPIOA
#define  DEBUG_SPI_SCK_GPIO_PIN         GPIO_PIN_10
#define  DEBUG_SPI_MISO_GPIO_PORT       CW_GPIOA
#define  DEBUG_SPI_MISO_GPIO_PIN        GPIO_PIN_11
// #define  DEBUG_SPI_CS_GPIO_PORT         CW_GPIOA
// #define  DEBUG_SPI_CS_GPIO_PIN          GPIO_PIN_15

//GPIO AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()
#define  DEBUG_SPI_AFSCK                PA10_AFx_SPI1SCK()
#define  DEBUG_SPI_AFMISO               PA11_AFx_SPI1MISO()
// #define  DEBUG_SPI_AFCS                 PA15_AFx_SPI1CS()

// #define  SPI_CS_LOW()     						  PA00_SETLOW()
// #define  SPI_CS_HIGH()    						  PA00_SETHIGH()

#define  TxBufferSize1                  (ARRAY_SZ(TxBuffer1) - 1)
#define  TxBufferSize2                  (ARRAY_SZ(TxBuffer2) - 1)

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
void UART_Configuration(void);
void SPI_Configuration(void);
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer1[] = "\r\nCW32F030 UART Synchronous: UART -> SPI\r\n";
uint8_t TxBuffer2[] = "\r\nCW32F030 UART Synchronous: SPI  -> UART\r\n";
uint8_t RxBuffer1[TxBufferSize2];
uint8_t RxBuffer2[TxBufferSize1];
uint32_t NbrOfDataToRead1 = TxBufferSize2;
uint32_t NbrOfDataToRead2 = TxBufferSize1;
uint8_t TxCounter1 = 0, RxCounter1 = 0;
uint8_t TxCounter2 = 0, RxCounter2 = 0;
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

  //����UART ͬ����˫��ģʽ
  UART_Configuration();

  //����SPI ���߰�˫��ģʽ
  SPI_Configuration();

  /*************************** UART -> SPI *************************/
  USART_DirectionModeCmd(DEBUG_USARTx, USART_Mode_Tx, ENABLE);
  //SPI ������
  SPI_BiDirectionalLineConfig(DEBUG_SPIx, SPI_Direction_Rx);
  //����Ƭѡ
  SPI_NSSInternalSoftwareConfig(DEBUG_SPIx, SPI_NSSInternalSoft_Reset);
  // SPI_CS_LOW();
  while(NbrOfDataToRead2--)
  {
    //UART����һ���ֽ�����
    USART_SendData_8bit(DEBUG_USARTx, TxBuffer1[TxCounter1++]);
    while(USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

    //SPI �ȴ�RXNE
    while(SPI_GetFlagStatus(DEBUG_SPIx, SPI_FLAG_RXNE) == RESET);   
    RxBuffer2[RxCounter2++] = SPI_ReceiveData(DEBUG_SPIx);
  }
  //����Ƭѡ
  SPI_NSSInternalSoftwareConfig(DEBUG_SPIx, SPI_NSSInternalSoft_Set);
  // SPI_CS_HIGH();

  /*************************** SPI -> UART **************************/
  //SPI ������
  SPI_BiDirectionalLineConfig(DEBUG_SPIx, SPI_Direction_Tx);
  //SPI ��շ��ͻ���������λ�Ĵ���
  SPI_FlushSendBuff(DEBUG_SPIx);
  SPI_SendData(DEBUG_SPIx, TxBuffer2[TxCounter2++]);
  //����Ƭѡ
  SPI_NSSInternalSoftwareConfig(DEBUG_SPIx, SPI_NSSInternalSoft_Reset);
  // SPI_CS_LOW();
  USART_DirectionModeCmd(DEBUG_USARTx, USART_Mode_Tx, DISABLE);
  USART_DirectionModeCmd(DEBUG_USARTx, USART_Mode_Rx, ENABLE);
  while(NbrOfDataToRead1--)
  { 
    //��ѯTXE��SPI����һ���ֽ�����
    while(SPI_GetFlagStatus(DEBUG_SPIx, SPI_FLAG_TXE) == RESET);
    SPI_SendData(DEBUG_SPIx, TxBuffer2[TxCounter2++]);

    //UART �ȴ�RC
    while(USART_GetFlagStatus(DEBUG_USARTx,USART_FLAG_RC) == RESET);
    RxBuffer1[RxCounter1++] = USART_ReceiveData_8bit(DEBUG_USARTx);
    //���RC��������ʼ������һ���ֽ�����
    USART_ClearFlag(DEBUG_USARTx, USART_FLAG_RC);
  }
  //����Ƭѡ
  SPI_NSSInternalSoftwareConfig(DEBUG_SPIx, SPI_NSSInternalSoft_Set);
  // SPI_CS_HIGH();
  
  //����շ�����һ����
  TransferStatus1 = Buffercmp(TxBuffer1, RxBuffer2, TxBufferSize1);
  TransferStatus2 = Buffercmp(TxBuffer2, RxBuffer1, TxBufferSize2);

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
  RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK | DEBUG_SPI_GPIO_CLK | RCC_AHB_PERIPH_GPIOB, ENABLE);
  DEBUG_USART_APBClkENx(DEBUG_USART_CLK, ENABLE);
  DEBUG_SPI_APBClkENx(DEBUG_SPI_CLK, ENABLE);
}

/**
 * @brief ����GPIO
 * 
 */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //UART TX RX ���� 
  DEBUG_USART_AFTX;                     
  DEBUG_USART_AFRX;   

  //SPI SCK MISO CS ����
  DEBUG_SPI_AFSCK;
  DEBUG_SPI_AFMISO;  
  // DEBUG_SPI_AFCS;     

  //UART TX RX CS �������
  GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

  // GPIO_InitStructure.Pins = DEBUG_USART_CS_GPIO_PIN;
  // GPIO_Init(DEBUG_USART_CS_GPIO_PORT, &GPIO_InitStructure);

  //SPI MISO �������
  GPIO_InitStructure.Pins = DEBUG_SPI_MISO_GPIO_PIN;
  GPIO_Init(DEBUG_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  //SPI SCK ��������
  GPIO_InitStructure.Pins = DEBUG_SPI_SCK_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(DEBUG_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  //SPI CS ��������
  // GPIO_InitStructure.Pins = DEBUG_SPI_CS_GPIO_PIN;
  // GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  // GPIO_Init(DEBUG_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  //PB9 LED1 / PB8 - LED2 
  GPIO_InitStructure.Pins = GPIO_PIN_9 | GPIO_PIN_8;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

  //LED��
  PB09_SETLOW();
  PB08_SETLOW();
}

/**
 * @brief ����UART ͬ��ģʽ  2Mbps
 * 
 */
void UART_Configuration(void)
{
  USART_ClockInitTypeDef USART_ClockInitStructure;

  USART_ClockInitStructure.USART_Clock = USART_Clock_Enable;
  USART_ClockInitStructure.USART_Source = USART_Source_PCLK;

  USART_ClockInit(DEBUG_USARTx, &USART_ClockInitStructure); 
}

/**
 * @brief ����SPI ���߰�˫�� �ӻ�
 * 
 */
void SPI_Configuration(void)
{
  SPI_InitTypeDef SPI_InitStructure;

  SPI_InitStructure.SPI_Direction = SPI_Direction_1Lines_HalfDuplex;    // ���߰�˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Slave;                          // �ӻ�ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // ֡���ݳ���Ϊ8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           // ʱ�ӿ��е�ƽΪ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          // �ڶ������ز���
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // SSI�Ĵ�����ֵ���������Ƿ�ѡ��
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;    // ������ΪPCLK��2��Ƶ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_LSB;                    // �����Чλ LSB �շ���ǰ
  SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // ����SPI
  
  SPI_Init(DEBUG_SPIx, &SPI_InitStructure);
  SPI_Cmd(DEBUG_SPIx, ENABLE); 
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

