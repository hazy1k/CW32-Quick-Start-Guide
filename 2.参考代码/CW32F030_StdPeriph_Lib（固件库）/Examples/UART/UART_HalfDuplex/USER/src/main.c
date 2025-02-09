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
//UARTy
#define  USARTy                         CW_UART1
#define  USARTy_CLK                     RCC_APB2_PERIPH_UART1
#define  USARTy_APBClkENx               RCC_APBPeriphClk_Enable2

#define  USARTy_GPIO_CLK                RCC_AHB_PERIPH_GPIOA
#define  USARTy_GPIO                    CW_GPIOA
#define  USARTy_TxPin                   GPIO_PIN_8
#define  USARTy_AFTX                    PA08_AFx_UART1TXD()
#define  USARTy_TXPUR                   PA08_PUR_ENABLE();

//UARTz
#define  USARTz                         CW_UART2
#define  USARTz_CLK                     RCC_APB1_PERIPH_UART2
#define  USARTz_APBClkENx               RCC_APBPeriphClk_Enable1

#define  USARTz_GPIO_CLK                RCC_AHB_PERIPH_GPIOA
#define  USARTz_GPIO                    CW_GPIOA
#define  USARTz_TxPin                   GPIO_PIN_6
#define  USARTz_AFTX                    PA06_AFx_UART2TXD()
#define  USARTz_TXPUR                   PA06_PUR_ENABLE()          

#define  USARTyz_BaudRate               9600
#define  USARTyz_UclkFreq               8000000

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
TestStatus Buffercmp(uint8_t* pBuffer1, uint8_t* pBuffer2, uint16_t BufferLength);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer1[] = "\r\nCW32F030 UART HalfDuplex: USARTy -> USARTz\r\n";
uint8_t TxBuffer2[] = "\r\nCW32F030 UART HalfDuplex: USARTz -> USARTy\r\n";
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

  //����UART
  UART_Configuration();

  //���߰�˫�� USARTy
  USART_HalfDuplexCmd(USARTy, ENABLE);

  //���߰�˫�� USARTz
  USART_HalfDuplexCmd(USARTz, ENABLE);

  //USARTy -> USARTz
  while(NbrOfDataToRead2--)
  {
    //USARTy����һ���ֽ�����
    USART_SendData_8bit(USARTy, TxBuffer1[TxCounter1++]);
    while(USART_GetFlagStatus(USARTy, USART_FLAG_TXE) == RESET);

    //USARTz �ȴ�RC
    while(USART_GetFlagStatus(USARTz, USART_FLAG_RC) == RESET);
    USART_ClearFlag(USARTz, USART_FLAG_RC);
    RxBuffer2[RxCounter2++] = USART_ReceiveData_8bit(USARTz);
  }

  //USARTz -> USARTy
  while(NbrOfDataToRead1--)
  { 
    //USARTz����һ���ֽ�����
    USART_SendData_8bit(USARTz, TxBuffer2[TxCounter2++]);
    while(USART_GetFlagStatus(USARTz, USART_FLAG_TXE)== RESET);

    //USARTy �ȴ�RC
    while(USART_GetFlagStatus(USARTy,USART_FLAG_RC) == RESET);
    USART_ClearFlag(USARTy, USART_FLAG_RC);
    RxBuffer1[RxCounter1++] = USART_ReceiveData_8bit(USARTy);
  }
  
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
  //SYSCLK = HSI = 8MHz = HCLK = PCLK
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  //����ʱ��ʹ��
  RCC_AHBPeriphClk_Enable(USARTy_GPIO_CLK | USARTz_GPIO_CLK | RCC_AHB_PERIPH_GPIOB, ENABLE);
  USARTy_APBClkENx(USARTy_CLK, ENABLE);
  USARTz_APBClkENx(USARTz_CLK, ENABLE);
}

/**
 * @brief ����GPIO
 * 
 */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  //UART TX ���� 
  USARTy_AFTX;                     
  USARTz_AFTX;  

  //UART TX PUR
  USARTy_TXPUR;
  USARTz_TXPUR;                

  GPIO_InitStructure.Pins = USARTy_TxPin;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD; //��©���
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(USARTy_GPIO, &GPIO_InitStructure);

  GPIO_InitStructure.Pins = USARTz_TxPin;
  GPIO_Init(USARTz_GPIO, &GPIO_InitStructure);

  //PB9 LED1 / PB8 LED2 
  GPIO_InitStructure.Pins = GPIO_PIN_9 | GPIO_PIN_8;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

  //LED��
  PB09_SETLOW();
  PB08_SETLOW();
}

/**
 * @brief ����UART
 * 
 */
void UART_Configuration(void)
{
  USART_InitTypeDef USART_InitStructure;

  USART_InitStructure.USART_BaudRate = USARTyz_BaudRate;
  USART_InitStructure.USART_Over = USART_Over_16;
  USART_InitStructure.USART_Source = USART_Source_PCLK;
  USART_InitStructure.USART_UclkFreq = USARTyz_UclkFreq;
  USART_InitStructure.USART_StartBit = USART_StartBit_FE;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No ;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USARTy, &USART_InitStructure); 
  USART_Init(USARTz, &USART_InitStructure); 
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

