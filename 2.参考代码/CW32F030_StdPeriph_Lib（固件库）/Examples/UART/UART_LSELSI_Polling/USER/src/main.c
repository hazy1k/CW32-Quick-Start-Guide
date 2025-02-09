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
//UARTx
#define  DEBUG_USARTx                   CW_UART1
#define  DEBUG_USART_CLK                RCC_APB2_PERIPH_UART1
#define  DEBUG_USART_APBClkENx          RCC_APBPeriphClk_Enable2
#define  DEBUG_USART_BaudRate           9600
#define  DEBUG_USART_UclkFreq           32800

//UARTx GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9

//GPIO AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()

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
void USART_SendBuf_Polling(UART_TypeDef* USARTx, uint8_t *TxBuf, uint8_t TxCnt);
uint8_t USART_RecvBuf_Polling(UART_TypeDef* USARTx, uint8_t *RxBuf);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t  TxRxBufferSize;
uint8_t  TxRxBuffer[] = "\r\nCW32F030 UART Dedicated Sampling\r\n";
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

  USART_SendBuf_Polling(DEBUG_USARTx, TxRxBuffer, ARRAY_SZ(TxRxBuffer) - 1); 

  while(1)
  {
    //��ѯ�շ�
    TxRxBufferSize = USART_RecvBuf_Polling(DEBUG_USARTx, TxRxBuffer);
    USART_SendBuf_Polling(DEBUG_USARTx, TxRxBuffer, TxRxBufferSize); 
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
  RCC_LSI_Enable();

  //����ʱ��ʹ��
  RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK, ENABLE);
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
}

/**
 * @brief ����UART
 * 
 */
void UART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = DEBUG_USART_BaudRate;
  USART_InitStructure.USART_Over = USART_Over_sp;              //ר�ò���
  USART_InitStructure.USART_Source = USART_Source_LSI;         //����ʱ�� UCLK = LSI
  USART_InitStructure.USART_UclkFreq = DEBUG_USART_UclkFreq;
  USART_InitStructure.USART_StartBit = USART_StartBit_FE;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(DEBUG_USARTx, &USART_InitStructure); 	
}

/**
 * @brief ����8λ����
 * 
 * @param USARTx :USARTx����
 *        ����������:
 *           CW_UART1��CW_UART2��CW_UART3
 * @param TxBuf :�����͵�����
 * @param TxCnt :�����͵�����Ԫ�ظ���
 */
void USART_SendBuf_Polling(UART_TypeDef* USARTx, uint8_t *TxBuf, uint8_t TxCnt)
{
  while(TxCnt)
  {
    USART_SendData_8bit(USARTx, *TxBuf);
    while(USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET);
    TxBuf++;
    TxCnt--;
  }	
  while(USART_GetFlagStatus(USARTx, USART_FLAG_TXBUSY) == SET);
}

/**
 * @brief ����8λ����
 * 
 * @param USARTx :USARTx����
 *        ����������:
 *           CW_UART1��CW_UART2��CW_UART3
 * @param RxBuf :����Buf
 * @return uint8_t :���յ��ַ�����
 */
uint8_t USART_RecvBuf_Polling(UART_TypeDef* USARTx, uint8_t *RxBuf)
{
  uint8_t RxCnt = 0;
  do
  {
    //�ȴ�RC
    while(USART_GetFlagStatus(USARTx, USART_FLAG_RC) == RESET);
    //��RC 
    USART_ClearFlag(USARTx, USART_FLAG_RC); 
    //ERROR: PE or FE
    if(USART_GetFlagStatus(USARTx, USART_FLAG_PE|USART_FLAG_FE))
    {
      USART_ClearFlag(USARTx, USART_FLAG_PE|USART_FLAG_FE);
      RxCnt = 0x00;
    }
    else
    {
      RxBuf[RxCnt] = USART_ReceiveData_8bit(USARTx);
      RxCnt++;            
    }
  }while(RxBuf[RxCnt-1] != '\n');
  return RxCnt; 
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

