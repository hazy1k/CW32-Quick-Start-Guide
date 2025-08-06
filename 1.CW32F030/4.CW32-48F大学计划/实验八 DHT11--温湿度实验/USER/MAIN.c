/*
DHT11 IO口：PB1
运行下载之后OLED屏幕显示DHT11温湿度实时值
注意：温湿度DHT11模块的S端接PB1
*/

#include "main.h"
#include "Lcd_Driver.h"
#include "LCD_calculate.h"
#include "dht11.h"

unsigned int counttime=0;
float temperature;  	    
uint8_t humidity; 
char buff_1[15];
char buff_2[15];

void GPIO_Configuration(void);
void RCC_Configuration(void);
void BTIM_init(void);

int main()
{	
	RCC_Configuration();  //系统时钟64M
	GPIO_Configuration(); //LED初始化
	BTIM_init();          //定时器初始化
	
	Lcd_Init();
	Lcd_Clear(GRAY0);               //清屏
	Redraw_Mainmenu();
	
	while(DHT11_GPIO_Config())	//DHT11初始化
	{
	}
	
	while(1)
	{
		if(counttime>200)                           //200毫秒采集一次数据并更新屏幕
		{
			counttime=0;
		 	DHT11_Read_Data(&temperature,&humidity);	//读取温湿度	
			sprintf(buff_1,"%0.1f",temperature);
			sprintf(buff_2,"%d",humidity);  
			  Gui_DrawFont_GBK16(90,25,BLUE,GRAY0,buff_1);		//更新显示
		    Gui_DrawFont_GBK16(90,47,BLUE,GRAY0,buff_2);
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

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_0;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_1,GPIO_Pin_SET);
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
  BTIM_InitStruct.BTIM_Period = 9000;
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
		counttime++;
		if(count2>=500)
		{
				count2=0; 
				PB00_TOG();//0.5S
		}
	}
  /* USER CODE END */
}


