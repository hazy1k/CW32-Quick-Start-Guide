/**
 * @file main.c
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-09-23
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
#include "cw32f030_gtim.h"
#include "cw32f030_flash.h"

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
float fTsDegree;
//...
uint32_t valuePeriod;
uint32_t valuePosWidth;
uint32_t valueReload = 0xFFFFu;
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
 * @brief PWM I/O初始化
 *
 */
void PWM_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PA5 PWM 输出 */
	__RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.Pins = GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

	PA05_AFx_GTIM2CH1();
}

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
}

/**
 * @brief LED I/O初始化
 * 
 */
void LED_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
          
#ifdef TSSOP20_EN
      //打开GPIO时钟
	  __RCC_GPIOA_CLK_ENABLE();
      /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.Pins = GPIO_PIN_7;
	  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
          
      __RCC_GPIOB_CLK_ENABLE();
	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.Pins = GPIO_PIN_1;
	  //GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	  //GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

	  //LEDs are off.
	  PB01_SETLOW();
	  PA07_SETLOW();          
#else
	  //打开GPIO时钟
	  REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

	  /* Configure the GPIO_LED pin */
	  GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9;
	  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

	  //LEDs are off.
	  PB08_SETLOW();
	  PB09_SETLOW();
#endif

}

/**
 * @brief GTIM初始化
 * 
 */
void GTIM_Init(void)
{
	GTIM_InitTypeDef GTIM_InitStruct;

	//打开GTIM1时钟
	REGBITS_SET(CW_SYSCTRL->APBEN1, SYSCTRL_APBEN1_GTIM1_Msk);

	GTIM_InitStruct.Mode = GTIM_MODE_TIME;
	GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
	GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1024;
	GTIM_InitStruct.ReloadValue = 62499ul;	//T=1s.
	GTIM_InitStruct.ToggleOutState = DISABLE;

	GTIM_TimeBaseInit(CW_GTIM1, &GTIM_InitStruct);
	GTIM_ITConfig(CW_GTIM1, GTIM_IT_OV, ENABLE);

	NVIC_ClearPendingIRQ(GTIM1_IRQn);
	NVIC_EnableIRQ(GTIM1_IRQn);
	NVIC_SetPriority(GTIM1_IRQn, 0x03);

	//GTIM_Cmd(CW_GTIM1, ENABLE);

	//打开GTIM2时钟
	__RCC_GTIM2_CLK_ENABLE();

	//GTIM_InitStruct.Mode = GTIM_MODE_TIME;
	//GTIM_InitStruct.OneShotMode = GTIM_COUNT_CONTINUE;
	//GTIM_InitStruct.Prescaler = GTIM_PRESCALER_DIV1024;
	GTIM_InitStruct.ReloadValue = 0xFFFFu;
	GTIM_InitStruct.ToggleOutState = ENABLE;

	GTIM_TimeBaseInit(CW_GTIM2, &GTIM_InitStruct);

	valuePeriod = GTIM_InitStruct.ReloadValue;
	valuePosWidth = valuePeriod >> 1u;

	GTIM_OCInit(CW_GTIM2, GTIM_CHANNEL1, GTIM_OC_OUTPUT_PWM_HIGH);
	GTIM_SetCompare1(CW_GTIM2, valuePosWidth);

	//GTIM_ITConfig(CW_GTIM2, GTIM_IT_OV, ENABLE);
	GTIM_Cmd(CW_GTIM2, ENABLE);
}

/**
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
	uint8_t res;

	ADC_InitTypeDef ADC_InitStructure;
	ADC_WdtTypeDef ADC_WdtStructure;
	ADC_SingleChTypeDef ADC_SingleChStructure;

	RCC_HSI_Enable(RCC_HSIOSC_DIV6);
	//以下从HSI切换到PLL
	RCC_PLL_Enable( RCC_PLLSOURCE_HSI, 8000000UL, RCC_PLL_MUL_8); //开启PLL，PLL源为HSI
	//打开FLASH时钟
	__RCC_FLASH_CLK_ENABLE();
	FLASH_SetLatency(FLASH_Latency_3);
	res = RCC_SysClk_Switch( RCC_SYSCLKSRC_PLL); //切换系统时钟到PLL：64MHz。
	if (res > 0) //切换系统时钟失败
	{
#ifdef TSSOP20_EN
		PB01_SETHIGH();
		PA07_SETHIGH();
#else
		//LEDs are on.
		PB08_SETHIGH();
		PB09_SETHIGH();
#endif
		while (1);
	}
	else
	{
	}

	//配置ADC测试IO口
	ADC_PortInit();

	//LED初始化
	LED_Init();

	PWM_PortInit();
	GTIM_Init();

	//ADC默认值初始化
	ADC_StructInit(&ADC_InitStructure);
	//ADC模拟看门狗通道初始化
	ADC_WdtInit(&ADC_WdtStructure);

	ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div128;    //ADCCLK:500KHz.
	ADC_InitStructure.ADC_InBufEn = ADC_BufEnable;
	ADC_InitStructure.ADC_SampleTime = ADC_SampTime10Clk;

	//配置单通道转换模式
	ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;
	ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH1; //选择ADC转换通道
	ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;
	ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure;

	ADC_SingleChOneModeCfg(&ADC_SingleChStructure);
	ADC_ITConfig(ADC_IT_EOC, ENABLE);

	ADC_EnableIrq(ADC_INT_PRIORITY);
	ADC_ClearITPendingAll();

	//ADC使能
	ADC_Enable();

	//ADC外部中断触发源配置
	ADC_ExtTrigCfg(ADC_TRIG_GTIM1, ENABLE);
	GTIM_Cmd(CW_GTIM1, ENABLE);

	while (1)
	{
		while (!(gFlagIrq & ADC_ISR_EOC_Msk));
		gFlagIrq = 0u;

#ifdef TSSOP20_EN
		PB01_TOG();
#else
		PB09_TOG();
#endif

		valueAdc = ADC_GetConversionValue();
		//positive slope
        //valueReload =(4095u * 62500ul) / (4999u * valueAdc + 4095u);
        valueReload =((4095u * 125000ul) / (4999u * valueAdc + 4095u) + 1) >> 1;//Rounding.
		valuePosWidth = valueReload >> 1u;
                
		GTIM_SetCounterValue(CW_GTIM2, 0u);     //reset.
		GTIM_SetReloadValue(CW_GTIM2, valueReload);
		GTIM_SetCompare1(CW_GTIM2, valuePosWidth);
		//等待ADC外部中断触发源启动下一次ADC转换
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

