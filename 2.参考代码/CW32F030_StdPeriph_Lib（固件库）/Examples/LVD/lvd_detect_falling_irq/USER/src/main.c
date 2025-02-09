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
#include "cw32f030_lvd.h"

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
 * @brief LVD I/O��ʼ��
 * 
 */
void LVD_PortInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    //��GPIOʱ��
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk );
    //��LVDʱ��:PCLK.
    //...

    //set PA08 as output
    GPIO_InitStructure.Pins = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA , &GPIO_InitStructure);
    //set PA08 as LVD output
    PA08_AFx_LVDOUT();
    //set PB11 as LVD source
    PB11_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
    LVD_InitTypeDef LVD_InitStruct = {0};

	//LED��ʼ��
	LED_Init();

    //���ò���IO��
    LVD_PortInit();

    LVD_InitStruct.LVD_Action = LVD_Action_Irq; //�����жϹ���
    LVD_InitStruct.LVD_Source = LVD_Source_PB11; //����LVD�����
    LVD_InitStruct.LVD_Threshold = LVD_Threshold_2p0V; //����LVD��׼��ѹ
    LVD_InitStruct.LVD_FilterEn = LVD_Filter_Enable;
    LVD_InitStruct.LVD_FilterClk = LVD_FilterClk_RC150K;
    LVD_InitStruct.LVD_FilterTime = LVD_FilterTime_4095Clk;
    LVD_Init(&LVD_InitStruct);

    LVD_TrigConfig(LVD_TRIG_FALL, ENABLE);
    LVD_EnableIrq(LVD_INT_PRIORITY);
    LVD_ClearIrq();
    FirmwareDelay(4800);	//delay100us(1);
    LVD_Enable(); //LVDʹ��

    while (1)
    {
        if (gFlagIrq)
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

