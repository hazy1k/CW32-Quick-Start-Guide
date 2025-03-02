/*
系统时钟配置为64M
LED1长亮
OLED屏显示字符串
*/

#include "main.h"
#include "OLED_I2C.h"
#include "LCD_calculate.h"
#include "Lcd_Driver.h"

void GPIO_Configuration(void);
void RCC_Configuration(void);


int main()
{	
	RCC_Configuration();  //系统时钟64M
	GPIO_Configuration(); //LED初始化
	Lcd_Init();
	Lcd_Clear(GRAY0);               //清屏
	
	Gui_DrawFont_GBK16(15,5,BLACK,GRAY0,"HELLO world");
	Gui_DrawFont_GBK16(15,25,RED,GRAY0,"LCD OK DISPLAY");
	while(1);
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
	
	__RCC_GPIOA_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
  PB00_SETLOW();
}

