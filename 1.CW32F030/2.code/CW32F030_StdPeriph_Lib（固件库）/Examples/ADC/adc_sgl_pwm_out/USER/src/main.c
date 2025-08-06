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
 * @brief PWM I/O��ʼ��
 *
 */
void PWM_PortInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* PA5 PWM ��� */
	__RCC_GPIOA_CLK_ENABLE();
	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.Pins = GPIO_PIN_5;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

	PA05_AFx_GTIM2CH1();
}

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
 * @brief LED I/O��ʼ��
 * 
 */
void LED_Init(void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
          
#ifdef TSSOP20_EN
      //��GPIOʱ��
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
#endif

}

/**
 * @brief GTIM��ʼ��
 * 
 */
void GTIM_Init(void)
{
	GTIM_InitTypeDef GTIM_InitStruct;

	//��GTIM1ʱ��
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

	//��GTIM2ʱ��
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
	//���´�HSI�л���PLL
	RCC_PLL_Enable( RCC_PLLSOURCE_HSI, 8000000UL, RCC_PLL_MUL_8); //����PLL��PLLԴΪHSI
	//��FLASHʱ��
	__RCC_FLASH_CLK_ENABLE();
	FLASH_SetLatency(FLASH_Latency_3);
	res = RCC_SysClk_Switch( RCC_SYSCLKSRC_PLL); //�л�ϵͳʱ�ӵ�PLL��64MHz��
	if (res > 0) //�л�ϵͳʱ��ʧ��
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

	//����ADC����IO��
	ADC_PortInit();

	//LED��ʼ��
	LED_Init();

	PWM_PortInit();
	GTIM_Init();

	//ADCĬ��ֵ��ʼ��
	ADC_StructInit(&ADC_InitStructure);
	//ADCģ�⿴�Ź�ͨ����ʼ��
	ADC_WdtInit(&ADC_WdtStructure);

	ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div128;    //ADCCLK:500KHz.
	ADC_InitStructure.ADC_InBufEn = ADC_BufEnable;
	ADC_InitStructure.ADC_SampleTime = ADC_SampTime10Clk;

	//���õ�ͨ��ת��ģʽ
	ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull;
	ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH1; //ѡ��ADCת��ͨ��
	ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure;
	ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure;

	ADC_SingleChOneModeCfg(&ADC_SingleChStructure);
	ADC_ITConfig(ADC_IT_EOC, ENABLE);

	ADC_EnableIrq(ADC_INT_PRIORITY);
	ADC_ClearITPendingAll();

	//ADCʹ��
	ADC_Enable();

	//ADC�ⲿ�жϴ���Դ����
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
		//�ȴ�ADC�ⲿ�жϴ���Դ������һ��ADCת��
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

