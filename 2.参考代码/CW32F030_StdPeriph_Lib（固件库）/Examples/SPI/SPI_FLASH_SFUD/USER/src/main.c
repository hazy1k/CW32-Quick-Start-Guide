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
 ** Success LED1��
 ** Error   LED2��
 **
 ** ��������
 ** SPI SCK  -- FLASH CLK
 ** SPI MOSI -- FLASH DI
 ** SPI MISO -- FLASH DO
 ** SPI CS   -- FLASH NCS
 **
 ******************************************************************************/
int32_t main(void)
{
  //����RCC
  RCC_Configuration();  

  //����GPIO
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
 * @brief ����RCC ʱ��ԴHSI Ƶ��8MHz PLL 8��Ƶ��SYSCLK=PCLK=HCLK=64MHz
 * 
 * 
 */
void RCC_Configuration(void)
{
  /* 0. HSIʹ�ܲ�У׼ */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     // HSI Ĭ�����Ƶ��8MHz
  RCC_PLL_OUT();
  
  ///< ��ʹ�õ�ʱ��ԴHCLK����48M,С�ڵ���72MHz������FLASH ���ȴ�����Ϊ3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
       
  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);
}

/**
 * @brief ����GPIO
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
 * @brief SPI ����
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

