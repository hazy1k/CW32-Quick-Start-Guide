//ϵͳʱ������Ϊ64M,ATIMER. ��KEY1ʱ��LED3�Ʒ�ת��LED2 0.5S��˸һ�Ρ�

#include "main.h"

void GPIO_Configuration(void);
void RCC_Configuration(void);
void ATIMER_init(void);	
	
	
int main()
{
	unsigned int key_1=0,i=0;
	RCC_Configuration();    //ϵͳʱ������Ϊ64M
	GPIO_Configuration();   //LED&KEY��ʼ��
	ATIMER_init();        
	while(1)
	{
		 if(GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)==GPIO_Pin_SET)key_1=0; //�½��ؼ��
		 else if(key_1==0)
		 {
			 for(i=0;i<2000;i++);              //����
			 if(GPIO_ReadPin(CW_GPIOB,GPIO_PIN_14)==GPIO_Pin_RESET)
			 { key_1=1; PA08_TOG();}
		 }		
	}
}


void RCC_Configuration(void)
{
  /* 0. HSIʹ�ܲ�У׼ */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. ʹ��PLL��ͨ��PLL��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     // HSI Ĭ�����Ƶ��8MHz
 // RCC_PLL_OUT();  //PC13�����PLLʱ��
  
  ///< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
  ///< ��ʹ�õ�ʱ��ԴHCLK����48MHz������FLASH ���ȴ�����Ϊ3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
    
  /* 3. ʱ���л���PLL */
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
	
	ATIM_InitStruct.BufferState = ENABLE;                               //ʹ�ܻ���Ĵ���   
  ATIM_InitStruct.ClockSelect = ATIM_CLOCK_PCLK;                      //ѡ��PCLKʱ�Ӽ���
  ATIM_InitStruct.CounterAlignedMode = ATIM_COUNT_MODE_EDGE_ALIGN;    //���ض���
  ATIM_InitStruct.CounterDirection = ATIM_COUNTING_UP;                //���ϼ�����
  ATIM_InitStruct.CounterOPMode = ATIM_OP_MODE_REPETITIVE;            //��������ģʽ
  ATIM_InitStruct.OverFlowMask = DISABLE;                             //�ظ������������������
  ATIM_InitStruct.Prescaler = ATIM_Prescaler_DIV8;                    // 8��Ƶ,8MHZ
  ATIM_InitStruct.ReloadValue = 8000;                                 // �������� 1MS TIMER
  ATIM_InitStruct.RepetitionCounter = 0;                              // �ظ�����0
  ATIM_InitStruct.UnderFlowMask = DISABLE;                            // �ظ����������������
  ATIM_Init(&ATIM_InitStruct);
  ATIM_ITConfig(ATIM_CR_IT_OVE, ENABLE);                              // ���ظ�������������������ж�
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

