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
* 代码许可和免责信息
* 武汉力源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉力源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉力源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
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
 * @brief ADC I/O初始化
 * 
 */
void ADC_PortInit(void)
{
    //打开GPIO时钟
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //打开ADC时钟
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

	//LED初始化
	LED_Init();

    //配置ADC测试IO口
    ADC_PortInit();

    //ADC初始化
    ADC_StructInit(&ADC_InitStructure);
    //ADC工作时钟配置
#ifdef SQR_BREAK_MODE_EN
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div1;
#else
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div16;
#endif

    //ADC序列扫描转换模式配置
    ADC_SerialChStructure.ADC_Sqr0Chmux = ADC_SqrVddaDiV3; //ADC_SqrCh1;
    ADC_SerialChStructure.ADC_Sqr1Chmux = ADC_SqrTs; //ADC_SqrCh2; //
    ADC_SerialChStructure.ADC_Sqr2Chmux = ADC_SqrVref1P2; //ADC_SqrCh5;
    ADC_SerialChStructure.ADC_Sqr3Chmux = ADC_SqrCh7;
    ADC_SerialChStructure.ADC_SqrEns = ADC_SqrEns03;
    ADC_SerialChStructure.ADC_InitStruct = ADC_InitStructure;

#ifdef SQR_CONTINUOUS_MODE_EN
    /* 序列通道连续采样模式 */
    ADC_SerialChContinuousModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOS, ENABLE);

#elif defined (SQR_SCAN_MODE_EN)
    /* 序列通道扫描采样模式 */
    ADC_SerialChScanModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOS, ENABLE);

#elif defined (SQR_MORE_MODE_EN)
    /* 序列通道多次采样模式 */
    cntSample = 0xFF;	//转换次数为0xFF + 1次.
    ADC_SerialChMoreModeCfg(&ADC_SerialChStructure, cntSample);
    ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOA, ENABLE);

#elif defined (SQR_BREAK_MODE_EN)
    /* 序列通道断续采样模式 */
    ADC_SerialChBreakModeCfg(&ADC_SerialChStructure);
    ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOS, ENABLE);
#else
    #error "Please select ADC's running mode first in main.h!"
#endif

    //ADC_ITConfig(ADC_IT_EOC | ADC_IT_EOS | ADC_IT_EOA, ENABLE);
    ADC_EnableIrq(ADC_INT_PRIORITY);
    ADC_ClearITPendingAll();

    //ADC使能
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

    while(1)
    {
#ifdef SQR_CONTINUOUS_MODE_EN
        /* 序列通道连续采样模式 */
    	while(!(gFlagIrq & ADC_ISR_EOS_Msk));
    	gFlagIrq = 0u;
        //while(!(gFlagIrq & ADC_ISR_EOS_Msk));

    	ADC_GetSqr0Result(gAdcRestult);
        ADC_GetSqr1Result(&gAdcRestult[1]);
        ADC_GetSqr2Result(&gAdcRestult[2]);
        ADC_GetSqr3Result(&gAdcRestult[3]);

#elif defined (SQR_SCAN_MODE_EN)
		/* 序列通道扫描采样模式 */
		while(!(gFlagIrq & ADC_ISR_EOS_Msk));
		gFlagIrq = 0u;
		ADC_GetSqr0Result(gAdcRestult);
		ADC_GetSqr1Result(&gAdcRestult[1]);
		ADC_GetSqr2Result(&gAdcRestult[2]);
		ADC_GetSqr3Result(&gAdcRestult[3]);

#elif defined (SQR_MORE_MODE_EN)
		/* 序列通道多次采样模式 */
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
		/* 序列通道断续采样模式 */
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
		ADC_SoftwareStartConvCmd(ENABLE);	//启动下一次ADC转换
#endif

    }

}

/**
 * @brief LED I/O初始化
 *
 */
void LED_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStruct;

	  //打开GPIO时钟
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

