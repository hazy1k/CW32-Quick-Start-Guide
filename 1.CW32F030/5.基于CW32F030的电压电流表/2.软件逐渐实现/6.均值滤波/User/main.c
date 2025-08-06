#include "main.h"

uint16_t Volt_Buffer[ADC_SAMPLE_SIZE];      //�洢ADC����ֵ
uint16_t Cal_Buffer = 0;   //�����ѹʱ���м����
uint32_t Led_Dis_Time;

int main()
{
  RCC_Configuration();
	Seg_Configuration();
	Btim1_Configuration();
	ADC_Configuration();

	while(1)
	{
		if(GetTick() >= (Led_Dis_Time + 300))          //300ms�ı�һ���������ʾֵ
		{
		  Led_Dis_Time = GetTick();
			Cal_Buffer = Mean_Value_Filter(Volt_Buffer,ADC_SAMPLE_SIZE);
			Cal_Buffer = (Cal_Buffer * ADC_REF_VALUE >> 12) * (R2 + R1)/R1;
      if(Cal_Buffer % 10 >= 5)Cal_Buffer = Cal_Buffer / 10 + 1;  // ��������
		  else Cal_Buffer = Cal_Buffer / 10;                         //��ʱ��ֵΪ��׼ֵ��100��
		  Dynamic_Display(Cal_Buffer);
		}
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
	
	InitTick(64000000);                // SYSTICK �Ĺ���Ƶ��Ϊ64MHz
}

uint32_t Mean_Value_Filter(uint16_t *value, uint32_t size)     //��ֵ�˲�
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
		//if(sum>1)sum+=4; ����У׼
    return sum;
}
