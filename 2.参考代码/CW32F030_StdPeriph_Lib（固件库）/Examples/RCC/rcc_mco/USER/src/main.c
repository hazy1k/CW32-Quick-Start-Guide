/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-25
 * 
 * @copyright Copyright (c) 2021
 * 
 */
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
#include "cw32f030_rcc.h"
#include "cw32f030_gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


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
 ** This sample toggle MCO
 **
 ******************************************************************************/
int32_t main(void)
{

	RCC_HSI_Enable( RCC_HSIOSC_DIV2 );   //24M
	/********************************************************************
	1. MCO toggle HCLK Test. MCO_CLK = HCLK 
	 *******************************************************************/
	RCC_MCO_OUT( RCC_MCO_SRC_HCLK, RCC_MCO_DIV1 );
	
	
	
	/********************************************************************
	2. MCO toggle PCLK Test. MCO_CLK = PCLK / 2
	 *******************************************************************/
	// RCC_MCO_OUT( RCC_MCO_SRC_PCLK, RCC_MCO_DIV2 );
	
	
	
	/********************************************************************
	3. MCO toggle HSI Test.  MCO_CLK = HSIOSC / 8
	 *******************************************************************/
	// RCC_MCO_OUT( RCC_MCO_SRC_HSI , RCC_MCO_DIV8 );
	
	
	
	/********************************************************************
	4. MCO toggle LSI Test.  MCO_CLK = LSI / 64
	 *******************************************************************/
	// RCC_LSI_Enable();
	// RCC_MCO_OUT( RCC_MCO_SRC_LSI , RCC_MCO_DIV64 );
	
	
	
	/********************************************************************
	5. MCO toggle HSE Test. MCO_CLK = HSE / 128
	 *******************************************************************/
  // RCC_HSE_Enable( RCC_HSE_MODE_OSC, 16000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE);
 	// RCC_MCO_OUT( RCC_MCO_SRC_HSE , RCC_MCO_DIV128 );
	
	

	/********************************************************************
	6. MCO toggle LSE Test. MCO_CLK = LSE / 256
	 *******************************************************************/
	// RCC_LSE_Enable( RCC_LSE_MODE_OSC ,RCC_LSE_AMP_LARGER, RCC_LSE_DRIVER_LARGER );
	// RCC_MCO_OUT( RCC_MCO_SRC_LSE , RCC_MCO_DIV256 );
	
	
	
	/********************************************************************
	7. MCO toggle PLL Test. MCO_CLK = PLL / 1
	 *******************************************************************/
	// RCC_HSE_Enable( RCC_HSE_MODE_OSC, 16000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE );
	// RCC_PLL_Enable( RCC_PLLSOURCE_HSEOSC, 16000000, RCC_PLL_MUL_2 );
  // RCC_MCO_OUT( RCC_MCO_SRC_PLL , RCC_MCO_DIV1 );
	
	
	
	/********************************************************************
	8. MCO toggle RC150K Test. MCO_CLK = RC150K
	   ��Ҫ�����˿�RC150K�˲�����VC�˲�
	 *******************************************************************/
	// RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOA, ENABLE); //��GPIOAʱ�� 
	// GPIO_ConfigFilter( CW_GPIOA, GPIO_PIN_8, GPIO_FLTCLK_RC150K );  //ѡ��GPIOA.08�˲�ΪRC150K
	// RCC_MCO_OUT( RCC_MCO_SRC_RC150K, RCC_MCO_DIV1 );            
	


	/********************************************************************
	9. MCO toggle RC10K Test. MCO_CLK = RC10K
	   ��Ҫ�����˿�RC10K�˲�����VC�˲�
	 *******************************************************************/
	// RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOA, ENABLE); //��GPIOAʱ�� 
	// GPIO_ConfigFilter( CW_GPIOA, GPIO_PIN_8, GPIO_FLTCLK_RC10K );  //ѡ��GPIOA.08�˲�ΪRC10K
	// RCC_MCO_OUT( RCC_MCO_SRC_RC10K, RCC_MCO_DIV1 ); 
	

    while(1){;}
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

