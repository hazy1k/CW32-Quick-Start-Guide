/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-05-18
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
#include "cw32f030_awt.h"
#include "cw32f030_rcc.h"
#include "cw32f030_gpio.h"
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
void GPIO_Configuration(void);
void NVIC_Configuration(void);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
 AWT_TimeCntInitTypeDef AWT_TimeCntInitStruct;
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
 ** This sample toggle PB08��PB09
 **
 ******************************************************************************/
int32_t main(void)
{
  RCC_APBPeriphClk_Enable2(RCC_APB2_PERIPH_AWT, ENABLE);  //Open AWT Clk
  RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB, ENABLE);    //Open GPIOA &GPIOB  Clk

  /* NVIC Configuration */
  NVIC_Configuration();

  /* GPIO Configuration */
  GPIO_Configuration();

  AWT_TimeCntStructInit( &AWT_TimeCntInitStruct );
  AWT_TimeCntInitStruct.AWT_ClkSource = AWT_CLKSOURCE_HSIOSC;
  AWT_TimeCntInitStruct.AWT_Prescaler = AWT_PRS_DIV32768;       
  AWT_TimeCntInitStruct.AWT_Mode = AWT_MODE_TIMECNT;
  AWT_TimeCntInitStruct.AWT_Period = 145;
  AWT_TimeCntInit(&AWT_TimeCntInitStruct);
  
  AWT_Cmd(ENABLE);

  while(1)
  {
		//��ѯ�����־�����־�����ƣ���ʾ�����۲�PB08��PB09���߲��Σ�������ʱʱ���Ƿ���ȷ
		if(SET == AWT_GetITStatus(AWT_IT_UD) )
		{
			AWT_ClearITPendingBit(AWT_IT_UD);			
			PB08_TOG();    
			PB09_TOG(); 			
		}
  } 
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;

  //PB8/PB9 ������� 
  GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	
	PB08_SETLOW();
	PB09_SETLOW();
}


/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{

}



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

