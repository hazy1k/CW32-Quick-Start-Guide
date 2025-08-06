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
#include "../inc/main.h"


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
void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);
void delay(uint16_t ms);
void ShowTime(void);
void RTC_IRQHandlerCallBack(void);

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
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/

/**
 * @brief  RTCʱ�Ӳ��ԣ���ʼ��������ʹ�ü���ж�0.5��ͨ��Log�������ʱ��
 * @return int32_t 
 */
int32_t main(void)
{
  RTC_InitTypeDef RTC_InitStruct;
	RTC_AlarmTypeDef RTC_AlarmStruct; 

  /* System Clocks Configuration */
  RCC_Configuration();  

  /* GPIO Configuration */
  GPIO_Configuration();

  /* NVIC Configuration */
  NVIC_Configuration();

  LogInit();	

  printf("RTC Init...\r\n");	
  printf("    (RTC CR0:%04x,CR1:%04x,CR2:%04x,RESET FLAG:0x%08x)\r\n",CW_RTC->CR0,CW_RTC->CR1,CW_RTC->CR2,CW_SYSCTRL->RESETFLAG);	
	
  //RCC_HSE_Enable(RCC_HSE_MODE_OSC, RCC_HSE_FREQ_GT24M,RCC_HSE_DRIVER_NORMAL, RCC_HSE_FLT_CLOSE);  //ѡ��HSEΪRTCʱ��
  RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);  // ѡ��LSEΪRTCʱ��
  //RCC_LSI_Enable();                                                           // ѡ��LSIΪRTCʱ��
	
	RTC_InitStruct.DateStruct.Day = 0x21;             //�������ڣ�DAY��MONTH��YEAR����ΪBCD��ʽ������Ϊ0~6�����������գ�����һ��������
	RTC_InitStruct.DateStruct.Month = RTC_Month_June;
	RTC_InitStruct.DateStruct.Week = RTC_Weekday_Monday;
	RTC_InitStruct.DateStruct.Year = 0x21;

	printf("-------Set Date as 20%x/%x/%x\r\n", RTC_InitStruct.DateStruct.Year,RTC_InitStruct.DateStruct.Month,RTC_InitStruct.DateStruct.Day);
	
	RTC_InitStruct.TimeStruct.Hour = 0x2;         //����ʱ�䣬HOUR��MINIUTE��SECOND����ΪBCD��ʽ���û��뱣֤HOUR��AMPM��H24֮��Ĺ�����ȷ��
	RTC_InitStruct.TimeStruct.Minute = 0x58;
	RTC_InitStruct.TimeStruct.Second = 0x59;	
  RTC_InitStruct.TimeStruct.AMPM = 1;
  RTC_InitStruct.TimeStruct.H24 = 0;
	printf("-------Set Time as %02x:%02x:%02x\r\n", RTC_InitStruct.TimeStruct.Hour,RTC_InitStruct.TimeStruct.Minute,RTC_InitStruct.TimeStruct.Second);
  RTC_InitStruct.RTC_ClockSource = RTC_RTCCLK_FROM_LSE;
  RTC_Init(&RTC_InitStruct);    //  �û���ѡ����Ҫʹ�õ�ʱ��Դ	

	printf("=====Set interval period as 0.5s...\r\n");	  
  RTC_SetInterval(RTC_INTERVAL_EVERY_0_5S);
	
	/* ��������Ϊ�����������6��45 */
  printf("=====Set AlarmA at 6:45 on workday...\r\n");	
	
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_WeekMON | RTC_AlarmMask_WeekTUE |
	                                RTC_AlarmMask_WeekWED | RTC_AlarmMask_WeekTHU |
																	RTC_AlarmMask_WeekFRI;
	RTC_AlarmStruct.RTC_AlarmTime.Hour = 6;
	RTC_AlarmStruct.RTC_AlarmTime.Minute = 0x45;
	RTC_AlarmStruct.RTC_AlarmTime.Second = 0;
	RTC_SetAlarm(RTC_Alarm_A, &RTC_AlarmStruct);
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);  
	
  printf("=====Enable ALRAMA and INTERVAL IT...\r\n");		
	
	RTC_ITConfig(RTC_IT_ALARMA | RTC_IT_INTERVAL, ENABLE);

  while(1)
  {
    
  }
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);
  RCC_LSI_Enable();
	RCC_LSE_Enable(RCC_LSE_MODE_OSC, RCC_LSE_AMP_NORMAL, RCC_LSE_DRIVER_NORMAL);
  RCC_APBPeriphClk_Enable1(RCC_APB1_PERIPH_RTC, ENABLE);
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{  
 
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
  __disable_irq(); 
  NVIC_EnableIRQ(RTC_IRQn); 
  __enable_irq();  
}

void delay(uint16_t ms)
{
  uint32_t i;
  for( ; ms >0; ms--)
  { 
    i = 796;
    while(i--);
  }
}

void ShowTime(void)
{
  RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	static uint8_t *WeekdayStr[7]={"SUN","MON","TUE","WED","THU","FRI","SAT"};
	static uint8_t *H12AMPMStr[2][2]={{"AM","PM"},{"",""}};
	
	RTC_GetDate(&RTC_DateStruct);	
	RTC_GetTime(&RTC_TimeStruct);	
	
  printf(".Date is 20%02x/%02x/%02x(%s).Time is %02x%s:%02x:%02x\r\n", RTC_DateStruct.Year, RTC_DateStruct.Month, RTC_DateStruct.Day, WeekdayStr[RTC_DateStruct.Week], RTC_TimeStruct.Hour, H12AMPMStr[RTC_TimeStruct.H24][RTC_TimeStruct.AMPM],RTC_TimeStruct.Minute, RTC_TimeStruct.Second);
}

void RTC_IRQHandlerCallBack(void)
{
  if (RTC_GetITState(RTC_IT_ALARMA))
	{
		RTC_ClearITPendingBit(RTC_IT_ALARMA);		
		printf("*********Alarm!!!!\r\n");
	}
	if (RTC_GetITState(RTC_IT_INTERVAL))
	{
		RTC_ClearITPendingBit(RTC_IT_INTERVAL);
		ShowTime();
		
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
