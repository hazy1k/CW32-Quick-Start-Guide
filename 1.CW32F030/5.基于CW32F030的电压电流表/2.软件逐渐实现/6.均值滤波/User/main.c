#include "main.h"

uint16_t Volt_Buffer[ADC_SAMPLE_SIZE];      //存储ADC采样值
uint16_t Cal_Buffer = 0;   //计算电压时的中间变量
uint32_t Led_Dis_Time;

int main()
{
  RCC_Configuration();
	Seg_Configuration();
	Btim1_Configuration();
	ADC_Configuration();

	while(1)
	{
		if(GetTick() >= (Led_Dis_Time + 300))          //300ms改变一次数码管显示值
		{
		  Led_Dis_Time = GetTick();
			Cal_Buffer = Mean_Value_Filter(Volt_Buffer,ADC_SAMPLE_SIZE);
			Cal_Buffer = (Cal_Buffer * ADC_REF_VALUE >> 12) * (R2 + R1)/R1;
      if(Cal_Buffer % 10 >= 5)Cal_Buffer = Cal_Buffer / 10 + 1;  // 四舍五入
		  else Cal_Buffer = Cal_Buffer / 10;                         //此时的值为标准值的100倍
		  Dynamic_Display(Cal_Buffer);
		}
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
	
	InitTick(64000000);                // SYSTICK 的工作频率为64MHz
}

uint32_t Mean_Value_Filter(uint16_t *value, uint32_t size)     //均值滤波
{
    uint32_t sum = 0;
    uint16_t max = 0;
    uint16_t min = 0xffff;
    int      i;

    for(i = 0; i < size; i++)
    {
        sum += value[i];
        if(value[i] > max)
        {
            max = value[i];
        }
        if(value[i] < min)
        {
            min = value[i];
        }
    }
    sum -= max + min;
    sum  = sum / (size - 2);
		//if(sum>1)sum+=4; 后期校准
    return sum;
}
