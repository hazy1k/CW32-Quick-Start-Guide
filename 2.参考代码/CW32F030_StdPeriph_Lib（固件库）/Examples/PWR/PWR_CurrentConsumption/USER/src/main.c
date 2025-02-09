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
#include "cw32f030_pwr.h"

#include "cw32f030_flash.h"
//
#include "cw32f030_pwr_modes.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/


/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
volatile uint8_t gKeyStatus;  /* set to 1 after User Button interrupt  */
volatile uint32_t gFlagWakeUpIrq = 0;  /* set to 1 after RTC alarm interrupt    */

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
 * @brief Main function of project
 * 
 * @return int 
 */
int main(void)
{
	RCC_HSI_Enable( RCC_HSIOSC_DIV2); //����ϵͳʱ��ΪHSI 24M
	//Enable Systick Clk
	InitTick(24000000ul); //��ʼ��SysTick

	//Test only.
	HclkOutTest();

	//LED��ʼ��
	LED_Init();

	while (1)
	{
		/* Configure User push-button as external interrupt generator */
		BSP_PB_Init();
		gKeyStatus = 0;

		/* Wait until User push-button is pressed to enter the Low Power mode.
		 In the meantime, LED is blinks */
		do
		{
			/* Toggle LED */
			PB09_TOG();
			SysTickDelay(1000);	//1s.
		} while (gKeyStatus == 0);

		/* Make sure LED is turned off to
		 reduce low power mode consumption */
		PB09_SETLOW();

#if defined (SLEEP_MODE)
		/* Sleep Mode Entry
		 - System Running at HSI (24 MHz)
		 - Code running from Internal FLASH
		 - All peripherals disabled.
		 - Wakeup using EXTI Line (User push-button PA.01)
		 */
		SleepModeTest();

#elif defined (DEEPSLEEP_MODE)
	    /* DeepSleep Mode Entry
	      - No IWDG
	      - Wakeup using EXTI Line (User push-button PA.01)
	    */
	    DeepSleepModeTest();
#else
        #error "Please select low power mode first!"
#endif /* SLEEP_MODE */

		//Test only.
		HclkOutTest();
	}

}

/**
 * @brief Configure User push-button as external interrupt generator
 * 
 */
void BSP_PB_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//��GPIOʱ��
	REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk);

	GPIO_InitStructure.Pins = GPIO_PIN_1;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;

	//Configure Button pin as input with External interrupt
	GPIO_InitStructure.IT = GPIO_IT_FALLING;
	GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

	//Enable and set Button EXTI Interrupt to the lowest priority
	NVIC_SetPriority(GPIOA_IRQn, 0x03);
	//����жϱ�־��ʹ��NVIC
	GPIOA_INTFLAG_CLR(GPIOx_ICR_PIN1_Msk);
	NVIC_EnableIRQ(GPIOA_IRQn);
}

/**
 * @brief LED I/O��ʼ��
 * 
 */
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	//��GPIOʱ��
	REGBITS_SET(CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOB_Msk);

	/* Configure the GPIO_LED pin */
	GPIO_InitStructure.Pins = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

	//LEDs are off.
	PB08_SETLOW();
	PB09_SETLOW();
}

 /**
  * @brief This function is executed in case of error occurrence.
  * 
  */
void Error_Handler(void)
{
	while (1)
	{
		/* Turn LED1,LED2 on */
		PB08_SETHIGH();
		PB09_SETHIGH();
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

