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
#include "cw32f030_flash.h"
#include "cw32f030_systick.h"

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
 ** This sample HSI switch to HSE, HSE switch to HSI.
 **
 ******************************************************************************/
int32_t main(void)
{
	uint8_t res = 0;
	GPIO_InitTypeDef GPIO_InitStruct;
	
	RCC_HSI_Enable( RCC_HSIOSC_DIV2 );     //����ϵͳʱ��ΪHSI 24M		
	
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_FLASH , ENABLE); //��FLASHʱ��

	//PB8/PB9 ������� 
	CW_SYSCTRL->AHBEN_f.GPIOB = 1;
	GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	PB08_SETLOW();
	PB09_SETLOW();	
   
	RCC_PCLK_OUT();                                                          //ͨ��PA03�۲�PCLKƵ��
	RCC_HCLK_OUT();                                                          //ͨ��PA04�۲�HCLKƵ��
	
	while(1)
	{
		//============================================================================
		//���´�HSI�л���HSE
		RCC_HSE_Enable( RCC_HSE_MODE_OSC, 16000000, RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE); //�����ⲿ����ʱ�ӣ�ʵ��Ƶ����Ҫ����ʵ�ʾ���Ƶ����������
		FLASH_SetLatency(FLASH_Latency_2);    																//Ƶ�ʴ���24M��Ҫ����FlashWait=2
		res = RCC_SysClk_Switch( RCC_SYSCLKSRC_HSE );                        	//�л�ϵͳʱ�ӵ��ⲿ����ʱ��
		if( res == 0x00 )                                                    	//�л�ϵͳʱ�ӳɹ�
		{
			PB08_SETHIGH();
			PB09_SETHIGH();
			RCC_HSI_Disable();
			FirmwareDelay( 50000 );                    
		}
		else
		{
		  while(1)
			{
				PB08_TOG();
				PB09_TOG();
				FirmwareDelay( 10000000 );  
			}
		}
		RCC_HSE_OUT();             //ͨ��PB02�۲�HSEƵ��
		
		//============================================================================
		//���´�HSE�л���HSI
		RCC_HSI_Enable( RCC_HSIOSC_DIV1 );                                  //�����ڲ�����ʱ��HSI = HSIOSC / 1                                     
		FLASH_SetLatency(FLASH_Latency_2);    															//Ƶ�ʴ���24M��Ҫ����FlashWait=2
		res = RCC_SysClk_Switch( RCC_SYSCLKSRC_HSI );                       //�л�ϵͳʱ�ӵ��ڲ�����ʱ��
		if( res == 0x00 )                                                       //�л�ϵͳʱ�ӳɹ�
		{
			FLASH_SetLatency(FLASH_Latency_2);    //Ƶ�ʴ���24M��Ҫ����FlashWait=2
			PB08_SETLOW();
			PB09_SETLOW();
			RCC_HSE_Disable();
			FirmwareDelay( 64000 );                                             
		}
		else
		{
		  while(1)
			{
				PB08_TOG();
				PB09_TOG();
				FirmwareDelay( 10000000 );  
			}
		}
		RCC_HSI_OUT();     //ͨ��PB00�۲�HSIƵ��
	}
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

