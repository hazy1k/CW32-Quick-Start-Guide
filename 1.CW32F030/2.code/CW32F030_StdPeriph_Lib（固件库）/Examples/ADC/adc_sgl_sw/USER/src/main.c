/**
 * @file main.c
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-09-22
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
#include "cw32f030_adc.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
uint16_t valueAdc;
uint32_t valueAdcAcc;
volatile uint8_t gFlagIrq;
uint16_t gCntEoc = 0;
uint8_t cntSample;

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
 * @brief ADC I/O��ʼ��
 * 
 */
void ADC_PortInit(void)
{
    //��GPIOʱ��
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //��ADCʱ��
    REGBITS_SET( CW_SYSCTRL->APBEN2, SYSCTRL_APBEN2_ADC_Msk );

    //set PA01 as AIN1 INPUT
    PA01_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_WdtTypeDef ADC_WdtStructure;
    ADC_SingleChTypeDef ADC_SingleChStructure;
    
	
	//LED��ʼ��
	LED_Init();

    //����ADC����IO��
    ADC_PortInit();

    //ADCĬ��ֵ��ʼ��
    ADC_StructInit(&ADC_InitStructure);
    //ADC����ʱ������
#ifdef SGL_MORE_MODE_EN
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4;
#else
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
#endif

    //ADCģ�⿴�Ź�ͨ����ʼ��
    ADC_WdtInit(&ADC_WdtStructure);
    
    //���õ�ͨ��ת��ģʽ
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;
    ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH1;	//ѡ��ADCת��ͨ����AIN1:PA01
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;
    ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure;

#ifdef SGL_ONE_MODE_EN
    ADC_SingleChOneModeCfg(&ADC_SingleChStructure);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);

#elif defined (SGL_MORE_MODE_EN)
    cntSample = 0xFF; //ת������Ϊ0xFF + 1��.
    ADC_SingleChMoreModeCfg(&ADC_SingleChStructure, cntSample);
	ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);

#elif defined (SGL_CONTINUOUS_MODE_EN)
    ADC_SingleChContinuousModeCfg(&ADC_SingleChStructure);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
#else
    #error "Please select ADC's running mode first in main.h!"    
#endif

    //ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);
    ADC_EnableIrq(ADC_INT_PRIORITY);
    ADC_ClearITPendingAll();

    //ADCʹ��
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while(1)
    {
#ifdef SGL_MORE_MODE_EN
    	while(!(gFlagIrq & ADC_ISR_EOA_Msk));
    	gFlagIrq = 0u;
    	ADC_GetAccResult(&valueAdcAcc);
			
		if(gCntEoc != (cntSample + 1))
		{
			PB08_SETHIGH();
			PB09_SETHIGH();
			while(1);	//error issue.
		}
		gCntEoc = 0u;

#else
        while(!(gFlagIrq & ADC_ISR_EOC_Msk));
        gFlagIrq = 0u;
        valueAdc = ADC_GetConversionValue();
#endif

        PB09_TOG();

#ifndef SGL_CONTINUOUS_MODE_EN
    	ADC_SoftwareStartConvCmd(ENABLE);	//������һ��ADCת��
#endif

    }

}

/**
 * @brief LED I/O��ʼ��
 *
 */
void LED_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  //��GPIOʱ��
	  REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

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

