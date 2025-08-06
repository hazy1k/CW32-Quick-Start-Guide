/*
系统时钟配置为64M
AD电位器所用IO口：PB0
OLED屏幕显示AD电压值，V为单位
*/

#include "main.h" 
#include "Lcd_Driver.h"
#include "LCD_calculate.h"

void GPIO_Configuration(void);
void RCC_Configuration(void);
void ADC_Configuration(void);
void BTIM_init(void);

unsigned int timecount=0;
uint16_t adcvalue;           


int main()
{	
	float temp;
	char temp_buff[10];
  char temp_buff1[10];
	
	RCC_Configuration();   //系统时钟64M
	GPIO_Configuration();  //LED初始化
	ADC_Configuration();   //ADC初始化
	BTIM_init();           //定时器初始化 
	
	Lcd_Init();
	Lcd_Clear(RED);               //清屏
	Gui_DrawFont_GBK16(0,0,WHITE,RED,"AD TEST:\n");
	Gui_DrawFont_GBK16(0,25,WHITE,RED,"VOL:");
	
	while(1)
	{ 
			if(timecount>200)//每200ms单次转换一次，并通过OLED显示
			{
				  timecount=0;		
			    ADC_SoftwareStartConvCmd(ENABLE);
					while(ADC_GetITStatus(ADC_IT_EOC))
					{
						ADC_ClearITPendingBit(ADC_IT_EOC);      
						adcvalue=ADC_GetConversionValue();		
						temp=(float)adcvalue*(3.3/4096);       //电压值换算
					  sprintf(temp_buff1,"%0.1f V  ",temp);
						Gui_DrawFont_GBK16(40,25,WHITE,RED,temp_buff1);	
					 
					} 					 	
			}				
	}
}

void RCC_Configuration(void)
{
  /* 0. HSI使能并校准 */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. 设置HCLK和PCLK的分频系数　*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. 使能PLL，通过PLL倍频到64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     // HSI 默认输出频率8MHz
 // RCC_PLL_OUT();  //PC13脚输出PLL时钟
  
  ///< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
  ///< 当使用的时钟源HCLK大于48MHz：设置FLASH 读等待周期为3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
    
  /* 3. 时钟切换到PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);	
}



void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStruct;
	
	__RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1 LED2
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_0;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_0|GPIO_PIN_1,GPIO_Pin_SET);
}


void ADC_Configuration(void)
{
   ADC_SingleChTypeDef ADC_SingleInitStruct;

  __RCC_ADC_CLK_ENABLE();    // ADC时钟使能
	__RCC_GPIOB_CLK_ENABLE();

	//配置ADC测试IO口  电位器接口
  PB00_ANALOG_ENABLE();
  ADC_SingleInitStruct.ADC_Chmux = ADC_ExInputCH8;         //通道4输入PB00
  ADC_SingleInitStruct.ADC_DiscardEn = ADC_DiscardNull;
  ADC_SingleInitStruct.ADC_InitStruct.ADC_AccEn = ADC_AccDisable;   //转换结果累加不使能
  ADC_SingleInitStruct.ADC_InitStruct.ADC_Align = ADC_AlignRight;   //ADC转换结果右对齐
  ADC_SingleInitStruct.ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div16;   //PCLK  
  ADC_SingleInitStruct.ADC_InitStruct.ADC_DMAEn = ADC_DmaDisable;   //关闭DMA传输
  ADC_SingleInitStruct.ADC_InitStruct.ADC_InBufEn = ADC_BufEnable;  //开启跟随器
  ADC_SingleInitStruct.ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode;   
  ADC_SingleInitStruct.ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk; //5个ADC时钟周期
  ADC_SingleInitStruct.ADC_InitStruct.ADC_TsEn = ADC_TsDisable;    //内置温度传感器禁用
  ADC_SingleInitStruct.ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA;//VDDA参考电压
  ADC_SingleInitStruct.ADC_WdtStruct.ADC_WdtAll = ADC_WdtDisable;    

  ADC_SingleChOneModeCfg(&ADC_SingleInitStruct);
  ADC_Enable();    // 使能ADC
	
	ADC_SoftwareStartConvCmd(ENABLE);
}

void BTIM_init(void)
{
	BTIM_TimeBaseInitTypeDef BTIM_InitStruct;
	
	__RCC_BTIM_CLK_ENABLE();
	
	__disable_irq(); 
  NVIC_EnableIRQ(BTIM1_IRQn); 
  __enable_irq();
	
	BTIM_InitStruct.BTIM_Mode = BTIM_Mode_TIMER;
  BTIM_InitStruct.BTIM_OPMode = BTIM_OPMode_Repetitive;
  BTIM_InitStruct.BTIM_Period = 8000;
  BTIM_InitStruct.BTIM_Prescaler = BTIM_PRS_DIV8;
  BTIM_TimeBaseInit(CW_BTIM1, &BTIM_InitStruct);
	
  BTIM_ITConfig(CW_BTIM1, BTIM_IT_OV, ENABLE);
  BTIM_Cmd(CW_BTIM1, ENABLE);
}

void BTIM1_IRQHandler(void)
{
  /* USER CODE BEGIN */
 static unsigned int count2=0;
 if(BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
  {
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
		count2++;
		timecount++;
		if(count2>500)  //500MS
		{
			count2=0;
		  PB00_TOG();
		}
	}
  /* USER CODE END */
}






