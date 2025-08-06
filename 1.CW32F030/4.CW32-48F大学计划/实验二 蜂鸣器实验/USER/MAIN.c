/*
系统时钟配置为64M,蜂鸣器响
BEEP_IO:PB03
*/

#include "main.h"

void GPIO_Configuration(void);  
void RCC_Configuration(void);  
	
int main()
{
	uint32_t i;
	RCC_Configuration(); //系统时钟64M
	GPIO_Configuration();//LED&BEEP初始化
	while(1)
	{
		PB03_SETHIGH(); 
    for(i=0;i<1000000;i++);
	  PB03_SETLOW();
		for(i=0;i<1000000;i++);
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
	
  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1  LED2  
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_3;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_3,GPIO_Pin_RESET);
}

