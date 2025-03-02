#include "main.h" 
#include "Seg_Dis.h"
#include "BTIM1.h"
#include "ADC.h"
#include "flash.h"

extern uint16_t Volt_Buffer[ADC_SAMPLE_SIZE];
extern uint16_t Curr_Buffer[ADC_SAMPLE_SIZE];
extern uint8_t Seg_Reg[6];
uint16_t V_Buffer,I_Buffer;


unsigned int timecount=0;

unsigned char BrushFlag=0;  

void RCC1_Configuration(void)
{	
	FLASH_SetLatency(FLASH_Latency_2); // 设置主频为48MHZ需要注意，Flah的访问周期需要更改为FLASH_Latency_2。
  RCC_HSI_Enable(RCC_HSIOSC_DIV1);   // 设置频率为48M
	RCC_SYSCLKSRC_Config(RCC_SYSCLKSRC_HSI);  //选择SYSCLK时钟源  48MHz
	RCC_HCLKPRS_Config(RCC_HCLK_DIV1);  //配置SYSTICK到HCLK分频系数  48MHz
	RCC_PCLKPRS_Config(RCC_PCLK_DIV8); //配置HCLK 到 PCLK的分频系数  6MHz
	InitTick(48000000);                // SYSTICK 的工作频率为48MHz，每ms中断一次
}

void RCC_Configuration(void)
{
  /* 0. HSI使能并校准 */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. 设置HCLK和PCLK的分频系数　*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. 使能PLL，通过PLL倍频到64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 6);     // HSI 默认输出频率8MHz
 // RCC_PLL_OUT();  //PC13脚输出PLL时钟
  
  ///< 当使用的时钟源HCLK大于24M,小于等于48MHz：设置FLASH 读等待周期为2 cycle
  ///< 当使用的时钟源HCLK大于48MHz：设置FLASH 读等待周期为3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
    
  /* 3. 时钟切换到PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(48000000);	
	RCC_PCLKPRS_Config(RCC_PCLK_DIV8); //配置HCLK 到 PCLK的分频系数  6MHz
}


void KEYGPIO_Init(void)
{
	__RCC_GPIOB_CLK_ENABLE();//打开GPIOB的时钟	
	__RCC_GPIOC_CLK_ENABLE();//打开GPIOC的时钟	
	GPIO_InitTypeDef GPIO_InitStruct; 
		
	GPIO_InitStruct.Pins = GPIO_PIN_12; // 
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	
	
	GPIO_InitStruct.Pins = GPIO_PIN_13; // 
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP; 
  GPIO_Init(CW_GPIOC, &GPIO_InitStruct);
}


void Volt_Cal(void);

int main()
{	
	RCC_Configuration();   //系统时钟64M
	KEYGPIO_Init();
	GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_RESET); 
	Seg_Init();
	Btim1_Init();
	ADC_init(); 
		
	while(1)
	{ 
		if(BrushFlag==1)
			{
				DisplayValue(V_Buffer);  //更新显示
				BrushFlag=0;
			}
			
			if(timecount>= 300)     //300ms改变一次数码管显示值//	
			{
				timecount=0;
				Volt_Cal();  //电压采集
				BrushFlag=1;
			}			
	}
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

void Volt_Cal(void)
{ 	
	float t,KT1;
	
	V_Buffer = Mean_Value_Filter(Volt_Buffer,ADC_SAMPLE_SIZE);//使用均值滤波
	
	V_Buffer = V_Buffer * ADC_REF_VALUE * 10>> 12; //0.1mV为单位  Ad精度应为1*4096*15000 即0.3mV  一个代码偏差0.3mV 
}	

void BTIM1_IRQHandler(void)
{
  static uint32_t Cnt=0,keytime=0,timecount1=0,ledcount=0;
	
  /* USER CODE BEGIN */
  if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
  {
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    Get_ADC_Value();
		
		ledcount++;  //LED闪
		if(ledcount>=1000)
		{PC13_TOG();ledcount=0;}
		
		
		timecount++;
    Dis_Refresh();//数码管扫描显示		
		 
  }
  /* USER CODE END */
}






