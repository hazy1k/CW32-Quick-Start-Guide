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
#define  DEBUG_USART_UclkFreq           24000000

//UARTx GPIO
#define  DEBUG_USART_GPIO_CLK           RCC_AHB_PERIPH_GPIOA
#define  DEBUG_USART_TX_GPIO_PORT       CW_GPIOA   
#define  DEBUG_USART_TX_GPIO_PIN        GPIO_PIN_8
#define  DEBUG_USART_RX_GPIO_PORT       CW_GPIOA
#define  DEBUG_USART_RX_GPIO_PIN        GPIO_PIN_9

//GPIO AF
#define  DEBUG_USART_AFTX               PA08_AFx_UART1TXD()
#define  DEBUG_USART_AFRX               PA09_AFx_UART1RXD()

#define  BufferSize                     (ARRAY_SZ(TxBuffer) - 1)

#define  FLASH_WriteAddress             0x00000
#define  FLASH_ReadAddress              FLASH_WriteAddress
#define  FLASH_SectorToEraseAddress     FLASH_WriteAddress
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

#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
uint8_t TxBuffer[] = "CW32F030 SPI FLASH Example";
uint8_t RxBuffer[BufferSize];

uint8_t DeviceID = 0;
uint16_t ManufactDeviceID = 0;
uint32_t JedecID = 0;
uint8_t UniqueID[8];

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
  //����RCC
  RCC_Configuration();  

  //����GPIO
  GPIO_Configuration();

  //����UART
  UART_Configuration();

  //����SPI
  SPI_FLASH_Init(); 

  printf("\r\nCW32F030 SPI FLASH Example\r\n");

  DeviceID = SPI_FLASH_DeviceID();
  ManufactDeviceID = SPI_FLASH_ManufactDeviceID();
  JedecID = SPI_FLASH_JedecID();	
  SPI_FLASH_UniqueID(UniqueID);

  printf("\r\nDeviceID = 0x%X\r\nManufactDeviceID = 0x%X\r\nJedecID = 0x%X", DeviceID, ManufactDeviceID, JedecID);	
  printf("\r\nUniqueID = 0x ");
  for(uint8_t i = 0;i<8;i++)
  {
    printf("%X ", UniqueID[i]);
  }
  
  /* Check JEDEC ID */
  if(JedecID == sJedecID)
  {	
	  printf("\r\n\nFLASH Detected\r\n");;
		
    //�������� 4KB
    SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);	 	 
		
    //д����
	  SPI_FLASH_BufferWrite(TxBuffer, FLASH_WriteAddress, BufferSize);		
	  printf("\r\nд�������Ϊ��%s\r\n", TxBuffer);
		
    //������
	  SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);
	  printf("\r\n����������Ϊ��%s\r\n", RxBuffer);
		
    //���д�������������������Ƿ�һ��
    TransferStatus = Buffercmp(TxBuffer, RxBuffer, BufferSize);

    //�������� 4KB
    SPI_FLASH_SectorErase(FLASH_SectorToEraseAddress);

    //������
    SPI_FLASH_BufferRead(RxBuffer, FLASH_ReadAddress, BufferSize);
		
    if(TransferStatus == PASSED)
    { 
      PB09_SETHIGH();
      printf("\r\nFLASH Success\r\n");
    }
    else
    {        
      PB08_SETHIGH();
      printf("\r\nFLASH Error 1\r\n");
    }

    for(uint8_t j = 0; j < BufferSize; j++)
    {
      if(RxBuffer[j] != 0xFF)
      {
        PB08_SETHIGH();
        printf("\r\nFLASH Error 2\r\n");
      }
    }
	}
	else// (FlashID ��= sFLASH_ID)
	{ 
		PB08_SETHIGH();
		printf("\r\nFLASH Error 3\r\n");
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
  RCC_AHBPeriphClk_Enable(DEBUG_USART_GPIO_CLK | RCC_AHB_PERIPH_GPIOB, ENABLE);
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

/**
 * @brief Retargets the C library printf function to the USART.
 * 
 */
PUTCHAR_PROTOTYPE
{
  USART_SendData_8bit(DEBUG_USARTx, (uint8_t)ch);

  while (USART_GetFlagStatus(DEBUG_USARTx, USART_FLAG_TXE) == RESET);

  return ch;
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

