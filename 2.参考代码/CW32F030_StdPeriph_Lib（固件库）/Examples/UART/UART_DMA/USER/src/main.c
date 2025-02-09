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
//HCLK for SysTick
#define  HCLKFREQ                       8000000
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

//DMA
#define  DEBUG_USART_TX_DMA             CW_DMACHANNEL2
#define  DEBUG_USART_RX_DMA             CW_DMACHANNEL1
#define  DEBUG_USART_TxTrigSource       DMA_HardTrig_UART1_TXBufferE
#define  DEBUG_USART_RxTrigSource       DMA_HardTrig_UART1_RXBufferNE
#define  DEBUG_DMA_BUFFSIZE             5000
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
void DMA_Configuration(void);
void USART_SendString(UART_TypeDef* USARTx, char *String);
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxRxBuffer[DEBUG_DMA_BUFFSIZE]; 
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
    
  //����UART
  UART_Configuration();

  //����DMA
  DMA_Configuration();

  //��ʼ��SysTick
  InitTick(HCLKFREQ); 

  USART_SendString(DEBUG_USARTx, "\r\nCW32F030 UART DMA Transmission\r\n" );

  //���TxRxBuffer
  for(int i=0; i<DEBUG_DMA_BUFFSIZE; i++)
  {
    TxRxBuffer[i] = i;
  }
  
  //ʹ��UART DMA TXD
  USART_DMACmd(DEBUG_USARTx, USART_DMAReq_Tx, ENABLE); 

  while(1)
  {  
    //CPU���У�LED1��˸
    PB09_TOG();
    SysTickDelay(500);    
  }
}

/**
 * @brief ����RCC
 * 
 */
void RCC_Configuration(void)
{
  //SYSCLK = HSI = 8MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  //����ʱ��ʹ��
  RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK | RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOB, ENABLE);
  DEBUG_USART_APBClkENx(DEBUG_USART_CLK, ENABLE);
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

  GPIO_InitStructure.Pins = DEBUG_USART_TX_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(DEBUG_USART_TX_GPIO_PORT, &GPIO_InitStructure);
    
  GPIO_InitStructure.Pins = DEBUG_USART_RX_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_Init(DEBUG_USART_RX_GPIO_PORT, &GPIO_InitStructure);

  //PB9 LED1
  GPIO_InitStructure.Pins = GPIO_PIN_9;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}

/**
 * @brief ����UART
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
 * @brief ����DMA
 * 
 */
void DMA_Configuration(void)
{
  DMA_InitTypeDef DMA_InitStructure;

  //��ʼ��DMA TX 
  DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Increase;
  DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Fix;
  DMA_InitStructure.TrigMode = DMA_HardTrig;
  DMA_InitStructure.HardTrigSource = DEBUG_USART_TxTrigSource;
  DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;
  DMA_InitStructure.DMA_SrcAddress = (uint32_t)TxRxBuffer;
  DMA_InitStructure.DMA_DstAddress = (uint32_t)&DEBUG_USARTx->TDR;
  DMA_Init(DEBUG_USART_TX_DMA, &DMA_InitStructure);
  DMA_Cmd(DEBUG_USART_TX_DMA, ENABLE);
    
  // //��ʼ��DMA RX
  // DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK;
  // DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_8BIT;
  // DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix;
  // DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase;
  // DMA_InitStructure.TrigMode = DMA_HardTrig;
  // DMA_InitStructure.HardTrigSource = DEBUG_USART_RxTrigSource;
  // DMA_InitStructure.DMA_TransferCnt = DEBUG_DMA_BUFFSIZE;
  // DMA_InitStructure.DMA_SrcAddress = (uint32_t)&DEBUG_USARTx->RDR;
  // DMA_InitStructure.DMA_DstAddress = (uint32_t)TxRxBuffer;
  // DMA_Init(DEBUG_USART_RX_DMA, &DMA_InitStructure);
  // DMA_Cmd(DEBUG_USART_RX_DMA, ENABLE);
}

/**
 * @brief �����ַ���
 * 
 * @param USARTx :USARTx����
 *        ����������:
 *           CW_UART1��CW_UART2��CW_UART3
 * @param String :�����͵��ַ���
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

