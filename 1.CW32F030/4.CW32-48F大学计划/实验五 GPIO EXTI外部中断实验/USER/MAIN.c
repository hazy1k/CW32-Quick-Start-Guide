//ϵͳʱ������Ϊ64M,�ϵ磬LED2,LED3������һ��key1 ��LED2�Ʒ�תһ�Ρ�
//KEY_UP:PB13 �½����жϴ���Ʒ�ת���ܡ�

#include "main.h"

void GPIO_Configuration(void);
void RCC_Configuration(void);
	
int main()
{
	RCC_Configuration();  //ϵͳʱ��64M
	GPIO_Configuration(); //LED&KEY��ʼ��
	while(1);
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
	
	 GPIO_InitTypeDef GPIO_InitStruct = {0};
	 
	__RCC_GPIOA_CLK_ENABLE();
	__RCC_GPIOB_CLK_ENABLE();
	 
  __disable_irq();
  NVIC_EnableIRQ(GPIOB_IRQn);
  __enable_irq();
	 
  GPIO_InitStruct.IT = GPIO_IT_FALLING;  //KEY_UP 
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_InitStruct.Pins = GPIO_PIN_13;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.IT = GPIO_IT_NONE; //LED1  LED2 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_7|GPIO_PIN_8;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_7|GPIO_PIN_8,GPIO_Pin_RESET);
}

/**
 * @brief This funcation handles GPIOA
 */
void GPIOA_IRQHandler(void)
{
  /* USER CODE BEGIN */	
   /* USER CODE END */
}

/**
 * @brief This funcation handles GPIOB
 */
void GPIOB_IRQHandler(void)
{
  /* USER CODE BEGIN */
 if (CW_GPIOB->ISR_f.PIN13) //pB13 test interrupt
  {
    CW_GPIOB->ICR_f.PIN13 = 0; // ���жϱ�־λ
		PA07_TOG();	
  }
  /* USER CODE END */
}


