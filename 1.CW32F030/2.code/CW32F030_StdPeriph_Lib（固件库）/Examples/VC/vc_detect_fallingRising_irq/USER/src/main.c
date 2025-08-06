/**
 * @file main.c
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-07-14
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
#include "cw32f030_systick.h"
#include "cw32f030_rcc.h"
#include "cw32f030_gpio.h"
#include "cw32f030_vc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
volatile boolean_t gFlagIrq = FALSE;

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
 * @brief VC I/O��ʼ��
 * 
 */
void VC_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    //��GPIOʱ��
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //��VCʱ��
    REGBITS_SET( CW_SYSCTRL->APBEN2, SYSCTRL_APBEN2_VC_Msk );

    //set PA11 as output
    GPIO_InitStructure.Pins = GPIO_PIN_11;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA , &GPIO_InitStructure);
    //set PA11 as VC output
    PA11_AFx_VC1OUT();
    //set PA00 as VC1_CH0 P-INPUT
    PA00_ANALOG_ENABLE();
    //set PA01 as VC1_CH1 N-INPUT
    PA01_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
    VC_InitTypeDef VC_InitStruct;
    VC_BlankTypeDef VC_BlankStruct;
    VC_OutTypeDef VC_OutStruct;	
        
	//LED��ʼ��
	LED_Init();

    //���ò���IO��
    VC_PortInit();
    //VCͨ����ʼ��
    VC_InitStruct.VC_InputP = VC_InputP_Ch0;
    VC_InitStruct.VC_InputN = VC_InputN_Bgr1P2;	//VC_InputN_Ch1;	//
    VC_InitStruct.VC_Hys = VC_Hys_10mV;
    VC_InitStruct.VC_Resp = VC_Resp_High;
    VC_InitStruct.VC_FilterEn = VC_Filter_Enable;
    VC_InitStruct.VC_FilterClk = VC_FltClk_RC150K;
    VC_InitStruct.VC_FilterTime = VC_FltTime_4095Clk;
    VC_InitStruct.VC_Window = VC_Window_Disable;
    VC_InitStruct.VC_Polarity = VC_Polarity_Low;
    VC1_ChannelInit(&VC_InitStruct);
    //VC Blank���ڳ�ʼ��
    VC1VC2_BlankInit(&VC_BlankStruct);
    VC1_BlankCfg(&VC_BlankStruct);
    //VC������ӳ�ʼ��
    VC1VC2_OutInit(&VC_OutStruct);
    VC1_OutputCfg(&VC_OutStruct);
    //VC�ж�����
    VC1_ITConfig(VC_IT_FALL | VC_IT_RISE, ENABLE);
    VC1_EnableIrq(VC_INT_PRIORITY);
    VC1_ClearIrq();
    VC1_EnableChannel();
    while (1)
    {
        if(gFlagIrq)
		{
			PB09_TOG();
			gFlagIrq = FALSE;
		}
    }
}

/**
 * @brief LED I/O��ʼ��
 *
 */
void LED_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;

	  //��GPIOʱ��
	  REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9;
	  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

	  //LEDs are off.
	  PB08_SETLOW();
	  PB09_SETLOW();

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
  * @return None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

