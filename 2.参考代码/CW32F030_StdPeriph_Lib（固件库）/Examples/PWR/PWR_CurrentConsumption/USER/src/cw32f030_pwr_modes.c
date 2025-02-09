/*
 * cw32f030_pwr_modes.c
 *
 *  Created on: 2021��8��09��
 *      Author: Socrates
 */

/**
  ******************************************************************************
  * @file    PWR/PWR_CurrentConsumption/cw32f030_pwr_modes.c
  * @author  MCD Application Team
  * @brief   This file provides firmware functions to manage the following
  *          functionalities of the CW32F030 Low Power Modes:
  *           - Sleep Mode
  *           - DeepSleep mode with or without RTC
  ******************************************************************************/



/* Includes ------------------------------------------------------------------*/
#include "cw32f030_pwr_modes.h"
#include "main.h"
#include "cw32f030_pwr.h"
#include "cw32f030_gpio.h"
#include "cw32f030_systick.h"
#include "cw32f030_rcc.h"
#include "cw32f030_flash.h"

/** @addtogroup CW32F030_HAL_Examples
  * @{
  */

/** @addtogroup PWR_CurrentConsumption
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* RTC handler declaration */
//RTC_HandleTypeDef RTCHandle;

/* Private function prototypes -----------------------------------------------*/
static void SYSCLKConfig_DeepSleep(void);

/* Private functions ---------------------------------------------------------*/

/* External variables --------------------------------------------------------*/
/* USER CODE BEGIN EV */
extern volatile uint8_t gKeyStatus;
/* USER CODE END EV */

/**
  * @brief  This function configures the system to enter Sleep mode for
  *         current consumption measurement purpose.
  *         Sleep Mode
  *         ==========
  *            - System Running at HSI (24 MHz)
  *            - Code running from Internal FLASH
  *            - All peripherals disabled.
  *            - Wakeup using EXTI Line (User push-button)
  * @param  None
  * @return None
  */
void SleepModeTest(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  PWR_InitTypeDef PWR_InitStructure;

  /* Configure all GPIO as analog to reduce current consumption on non used IOs */
  /* Enable GPIOs clock */
  /* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
  //��GPIOʱ��
  REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
    		  SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

  GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_InitStructure.IT = GPIO_IT_NONE;
  GPIO_InitStructure.Pins = GPIO_PIN_All;
  GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
  GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
  GPIO_Init(CW_GPIOF, &GPIO_InitStructure);

  /* Disable GPIOs clock */
  REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
    		  SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

  /* Configure User push-button as external interrupt generator */
  BSP_PB_Init();

  /*Suspend Tick increment to prevent wakeup by Systick interrupt.
    Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
  SuspendTick();

  /* Request to enter SLEEP mode */
  //HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI);
  PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
  PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Disable;
  PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
  PWR_Config(&PWR_InitStructure);
  PWR_GotoLpmMode();

  /* Resume Tick interrupt if disabled prior to sleep mode entry*/
  ResumeTick();

  /* Initialize led */
  LED_Init();
}

/**
  * @brief  This function configures the system to enter Deep-Sleep mode
  *         for current consumption measurement purpose.
  *         Deep-Sleep Mode
  *         ==========
  *           - No IWDG
  *           - Wakeup using EXTI Line (User push-button)
  * @param  None
  * @return None
  */
void DeepSleepModeTest(void)
{
	uint8_t res = 0u;
	GPIO_InitTypeDef GPIO_InitStructure;
	PWR_InitTypeDef PWR_InitStructure;

	/* Configure all GPIO as analog to reduce current consumption on non used IOs */
	/* Warning : Reconfiguring all GPIO will close the connexion with the debugger */
	/* Enable GPIOs clock */
	//��GPIOʱ��
	REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
			  SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

	GPIO_InitStructure.Mode = GPIO_MODE_ANALOG;
	GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
	GPIO_InitStructure.IT = GPIO_IT_NONE;
	GPIO_InitStructure.Pins = GPIO_PIN_All;
	GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
	GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
	GPIO_Init(CW_GPIOC, &GPIO_InitStructure);
	GPIO_Init(CW_GPIOF, &GPIO_InitStructure);

	//�ر�GPIOʱ��
	REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk | SYSCTRL_AHBEN_GPIOB_Msk |\
			  SYSCTRL_AHBEN_GPIOC_Msk | SYSCTRL_AHBEN_GPIOF_Msk);

	/* Configure User Button */
	BSP_PB_Init();

	/* Configures system clock after wake-up from DeepSleep: enable HSI and PLL with HSI as source*/
	// ���Ѻ��Զ�ʹ���ڲ�����ʱ�ӣ�HSI��
	RCC_WAKEUPCLK_Config(RCC_SYSCTRL_WAKEUPCLKEN);

	/* Enter Stop Mode */
	PWR_InitStructure.PWR_Sevonpend = PWR_Sevonpend_Disable;
	PWR_InitStructure.PWR_SleepDeep = PWR_SleepDeep_Enable;
	PWR_InitStructure.PWR_SleepOnExit = PWR_SleepOnExit_Disable;
	PWR_Config(&PWR_InitStructure);

	//��FLASHʱ��
	REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
	REGBITS_SET(CW_FLASH->CR1, FLASH_CR1_STANDBY_Msk); //��FLASH�͹���ʹ�ܿ���
	//�ر�FLASHʱ��
	REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);

#ifdef SLEEP_LSE_EN
	/*Suspend Tick increment to prevent wakeup by Systick interrupt.
	    Otherwise the Systick interrupt will wake up the device within 1ms (HAL time base)*/
	//SuspendTick();	//Comment it if using HSI.

	//Open LSE.
	RCC_LSE_Enable( RCC_LSE_MODE_OSC ,RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_LARGER);                                  //�����ⲿ����ʱ��LSE
	//Switch clock to LSE from HSI
	res = RCC_SysClk_Switch( RCC_SYSCLKSRC_LSE );	//�л�ϵͳʱ�ӵ�LSE
	if( res == 0x00 )	//�л�ϵͳʱ�ӳɹ�
	{
		RCC_HSI_Disable();	//�л�ʱ�ӵ�LSE��ر�Դʱ��HSI
		FirmwareDelay( 1400 );
	}
	else
	{
		Error_Handler();
	}
#endif

	PWR_GotoLpmMode();

#ifdef SLEEP_LSE_EN
	/* Resume Tick interrupt if disabled prior to sleep mode entry*/
	//ResumeTick();

	RCC_LSE_Disable();
#endif

	/* Configures system clock after wake-up from DeepSleep: enable HSI and PLL with HSI as source*/
	SYSCLKConfig_DeepSleep();

	/* Initialize LED on the board */
	LED_Init();

	/* Inserted Delay */
	SysTickDelay(200);

	/* Turn LED1 On */
	PB09_SETHIGH();

}


/**
  * @brief  Configures system clock after wake-up from DeepSleep: enable HSE, PLL
  *         and select PLL as system clock source.
  * @param  None
  * @return None
  */
static void SYSCLKConfig_DeepSleep(void)
{
	uint8_t res = 0u;
	//uint32_t pFLatency = 0;

	/* Get the Oscillators configuration according to the internal RCC registers */

	/* Activate PLL with HSI as source */
	RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 24000000ul, RCC_PLL_MUL_2);

	//��FLASHʱ��
	REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);
	FLASH_SetLatency( FLASH_Latency_3 ); //ϵͳʱ�Ӵ���24M��Ҫ����FlashWait
	//�ر�FLASHʱ��
	REGBITS_CLR( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_FLASH_Msk);

	res = RCC_SysClk_Switch( RCC_SYSCLKSRC_PLL );                    //�л�ϵͳʱ�ӵ�PLL
	if( res == 0x00 )                                                //�л�ϵͳʱ�ӳɹ�
	{
		FirmwareDelay( 100000 );
	}
	else
	{
		Error_Handler();
	}

	/* Select PLL as system clock source and configure the HCLK and PCLK1 clocks dividers */
	RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
	RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
	RCC_SYSCLKSRC_Config(RCC_SYSCLKSRC_PLL);

}

/**
 * @brief ϵͳʱ�Ӳ���
 * 
 */
void HclkOutTest(void)
{
    //��GPIOʱ��
    REGBITS_SET( CW_SYSCTRL->AHBEN, SYSCTRL_AHBEN_GPIOA_Msk );
    //����PA3,PA4Ϊ���
	REGBITS_CLR( CW_GPIOA->ANALOG , bv3 | bv4 );
	REGBITS_CLR( CW_GPIOA->DIR    , bv3 | bv4 );
	PA03_AFx_PCLKOUT(); //����PA03���PCLK
	PA04_AFx_HCLKOUT(); //����PA04���HCLK
}
/**
  * @}
  */

/**
  * @}
  */

/*****************************END OF FILE****/



