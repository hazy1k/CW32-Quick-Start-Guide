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
uint16_t gAdcRestult[4];
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
    //set PA02 as AIN2 INPUT
    PA02_ANALOG_ENABLE();
    //set PA05 as AIN5 INPUT
    PA05_ANALOG_ENABLE();
    //set PA07 as AIN7 INPUT
    PA07_ANALOG_ENABLE();
}

/**
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
    ADC_InitTypeDef ADC_InitStructure;
    ADC_SerialChTypeDef ADC_SerialChStructure;

	//LED��ʼ��
	LED_Init();

    //����ADC����IO��
    ADC_PortInit();

    //ADC��ʼ��
    ADC_StructInit(&ADC_InitStructure);
    //ADC����ʱ������
#ifdef SQR_BREAK_MODE_EN
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
#else
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div16;
#endif

    //ADC����ɨ��ת��ģʽ����
    ADC_SerialChStructure.ADC_Sqr0Chmux = ADC_SqrVddaDiV3; //ADC_SqrCh1;
    ADC_SerialChStructure.ADC_Sqr1Chmux = ADC_SqrTs; //ADC_SqrCh2; //
    ADC_SerialChStructure.ADC_Sqr2Chmux = ADC_SqrVref1P2; //ADC_SqrCh5;
    ADC_SerialChStructure.ADC_Sqr3Chmux = ADC_SqrCh7;
    ADC_SerialChStructure.ADC_SqrEns = ADC_SqrEns03;
    ADC_SerialChStructure.ADC_InitStruct = ADC_InitStructure;

#ifdef SQR_CONTINUOUS_MODE_EN
    /* ����ͨ����������ģʽ */
    ADC_SerialChContinuousModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOS, ENABLE);

#elif defined (SQR_SCAN_MODE_EN)
    /* ����ͨ��ɨ�����ģʽ */
    ADC_SerialChScanModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOS, ENABLE);

#elif defined (SQR_MORE_MODE_EN)
    /* ����ͨ����β���ģʽ */
    cntSample = 0xFF;	//ת������Ϊ0xFF + 1��.
    ADC_SerialChMoreModeCfg(&ADC_SerialChStructure, cntSample);
    ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);

#elif defined (SQR_BREAK_MODE_EN)
    /* ����ͨ����������ģʽ */
    ADC_SerialChBreakModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOS, ENABLE);
#else
    #error "Please select ADC's running mode first in main.h!"
#endif

    //ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOS | ADC_IT_EOA, ENABLE);
    ADC_EnableIrq(ADC_INT_PRIORITY);
    ADC_ClearITPendingAll();

    //ADCʹ��
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while(1)
    {
#ifdef SQR_CONTINUOUS_MODE_EN
        /* ����ͨ����������ģʽ */
    	while(!(gFlagIrq & ADC_ISR_EOS_Msk));
    	gFlagIrq = 0u;
        //while(!(gFlagIrq & ADC_ISR_EOS_Msk));

    	ADC_GetSqr0Result(gAdcRestult);
        ADC_GetSqr1Result(&gAdcRestult[1]);
        ADC_GetSqr2Result(&gAdcRestult[2]);
        ADC_GetSqr3Result(&gAdcRestult[3]);

#elif defined (SQR_SCAN_MODE_EN)
		/* ����ͨ��ɨ�����ģʽ */
		while(!(gFlagIrq & ADC_ISR_EOS_Msk));
		gFlagIrq = 0u;
		ADC_GetSqr0Result(gAdcRestult);
		ADC_GetSqr1Result(&gAdcRestult[1]);
		ADC_GetSqr2Result(&gAdcRestult[2]);
		ADC_GetSqr3Result(&gAdcRestult[3]);

#elif defined (SQR_MORE_MODE_EN)
		/* ����ͨ����β���ģʽ */
		while(!(gFlagIrq & ADC_ISR_EOA_Msk));
		gFlagIrq = 0u;
		ADC_GetSqr0Result(gAdcRestult);	//Get data.
		ADC_GetSqr1Result(&gAdcRestult[1]);
		ADC_GetSqr2Result(&gAdcRestult[2]);
		ADC_GetSqr3Result(&gAdcRestult[3]);
		ADC_GetAccResult(&valueAdcAcc);

		if(gCntEoc != (cntSample + 1))
		{
			PB08_SETHIGH();
			PB09_SETHIGH();
			while(1);	//error issue.
		}
		gCntEoc = 0u;

#elif defined (SQR_BREAK_MODE_EN)
		/* ����ͨ����������ģʽ */
		while(!(gFlagIrq & ADC_ISR_EOC_Msk));
		gFlagIrq &= (~ADC_ISR_EOC_Msk);

		if(gFlagIrq & ADC_ISR_EOS_Msk)
		{
			gFlagIrq = 0u;
			ADC_GetSqr0Result(gAdcRestult);
			ADC_GetSqr1Result(&gAdcRestult[1]);
			ADC_GetSqr2Result(&gAdcRestult[2]);
			ADC_GetSqr3Result(&gAdcRestult[3]);
		}
#endif

        PB09_TOG();

#ifndef SQR_CONTINUOUS_MODE_EN
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

