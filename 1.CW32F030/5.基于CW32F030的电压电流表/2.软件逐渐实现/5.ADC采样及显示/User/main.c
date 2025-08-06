#include "main.h"

uint16_t Volt_Buffer = 0;      //�洢ADC����ֵ
uint16_t Cal_Buffer = 0;   //�����ѹʱ���м����
uint16_t Show_TimeCount = 0;//100msˢ��һ������

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
      if(Cal_Buffer % 10 >= 5)Cal_Buffer = Cal_Buffer / 10 + 1;  // ��������
		  else Cal_Buffer = Cal_Buffer / 10;                         //��ʱ��ֵΪ��׼ֵ��100��
		}
		Dynamic_Display(Cal_Buffer);
	}
	
}

void RCC_Configuration(void)
{
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ�� */
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);

  /* 2. ʹ��PLL��ͨ��HSI��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 8);     
  // PLL���Ƶ��64MHz

  /*< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
  < ��ʹ�õ�ʱ��ԴHCLK����48M,С�ڵ���72MHz������FLASH ���ȴ�����Ϊ3 cycle */    
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);

  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(64000000);
}
