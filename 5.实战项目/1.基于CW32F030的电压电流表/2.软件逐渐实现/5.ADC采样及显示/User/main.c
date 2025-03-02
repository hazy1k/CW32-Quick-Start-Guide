#include "main.h"

uint16_t Volt_Buffer = 0;      //存储ADC采样值
uint16_t Cal_Buffer = 0;   //计算电压时的中间变量
uint16_t Show_TimeCount = 0;//100ms刷新一次数据

int main()
{
  RCC_Configuration();
	Seg_Configuration();
	Btim1_Configuration();
	ADC_Configuration();

	while(1)
	{
		if(Show_TimeCount >= 2000)
		{
			Show_TimeCount = 0;
		  Cal_Buffer = (Volt_Buffer * ADC_REF_VALUE >> 12) * (R2 + R1)/R1;
      if(Cal_Buffer % 10 >= 5)Cal_Buffer = Cal_Buffer / 10 + 1;  // 四舍五入
		  else Cal_Buffer = Cal_Buffer / 10;                         //此时的值为标准值的100倍
		}
		Dynamic_Display(Cal_Buffer);
	}
	
}

void RCC_Configuration(void)
{
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. 设置HCLK和PCLK的分频系数 */
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

  /* 2. 使能PLL，通过HSI倍频到64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     
  // PLL输出频率64MHz

  /*< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
  < 当使用的时钟源HCLK大于48M,小于等于72MHz：设置FLASH 读等待周期为3 cycle */    
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);

  /* 3. 时钟切换到PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);
}
