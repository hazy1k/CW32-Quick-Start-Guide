//系统时钟配置为64M,ATIMER. 按KEY1时，LED3灯反转；LED2 0.5S闪烁一次。

#include "main.h"

void GPIO_Configuration(void);
void RCC_Configuration(void);
void ATIMER_init(void);	
	
	
int main()
{
	unsigned int key_1=0,i=0;
	RCC_Configuration();    //系统时钟配置为64M
	GPIO_Configuration();   //LED&KEY初始化
	ATIMER_init();        
	while(1)
	{
		 if(GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)==GPIO_Pin_SET)key_1=0; //下降沿检测
		 else if(key_1==0)
		 {
			 for(i=0;i<2000;i++);              //消抖
			 if(GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)==GPIO_Pin_RESET)
			 { key_1=1; PA08_TOG();}
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
	
	__RCC_GPIOA_CLK_ENABLE();
	__RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE;  //KEY1
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_InitStruct.Pins = GPIO_PIN_14;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1  LED2
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
  GPIO_WritePin(CW_GPIOA, GPIO_PIN_7|GPIO_PIN_8,GPIO_Pin_SET);
}



void ATIMER_init(void)
{
	ATIM_InitTypeDef ATIM_InitStruct;
	
	__RCC_ATIM_CLK_ENABLE();
	
	__disable_irq(); 
  NVIC_EnableIRQ(ATIM_IRQn); 
  __enable_irq();
	
	ATIM_InitStruct.BufferState = ENABLE;                               //使能缓存寄存器   
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;                      //选择PCLK时钟计数
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;    //边沿对齐
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;                //向上计数；
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;            //连续运行模式
  ATIM_InitStruct.OverFlowMask = DISABLE;                             //重复计数器上溢出不屏蔽
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV8;                    // 8分频,8MHZ
  ATIM_InitStruct.ReloadValue = 8000;                                 // 重载周期 1MS TIMER
  ATIM_InitStruct.RepetitionCounter = 0;                              // 重复周期0
  ATIM_InitStruct.UnderFlowMask = DISABLE;                            // 重复计数下溢出不屏蔽
  ATIM_Init(&ATIM_InitStruct);
  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);                              // 有重复计数器溢出产生进入中断
  ATIM_Cmd(ENABLE); 	  
}

void ATIM_IRQHandler(void)
{
 static unsigned int count=0;
  if (ATIM_GetITStatus(ATIM_IT_OVF))
  {
    ATIM_ClearITPendingBit(ATIM_IT_OVF);
		count++;
		if(count>=500)//0.5S
		{
				count=0; 
				PA07_TOG();
		}
  }
}

