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
	FLASH_SetLatency(FLASH_Latency_2); // ������ƵΪ48MHZ��Ҫע�⣬Flah�ķ���������Ҫ����ΪFLASH_Latency_2��
  RCC_HSI_Enable(RCC_HSIOSC_DIV1);   // ����Ƶ��Ϊ48M
	RCC_SYSCLKSRC_Config(RCC_SYSCLKSRC_HSI);  //ѡ��SYSCLKʱ��Դ  48MHz
	RCC_HCLKPRS_Config(RCC_HCLK_DIV1);  //����SYSTICK��HCLK��Ƶϵ��  48MHz
	RCC_PCLKPRS_Config(RCC_PCLK_DIV8); //����HCLK �� PCLK�ķ�Ƶϵ��  6MHz
	InitTick(48000000);                // SYSTICK �Ĺ���Ƶ��Ϊ48MHz��ÿms�ж�һ��
}

void RCC_Configuration(void)
{
  /* 0. HSIʹ�ܲ�У׼ */
  RCC_HSI_Enable(RCC_HSIOSC_DIV6);

  /* 1. ����HCLK��PCLK�ķ�Ƶϵ����*/
  RCC_HCLKPRS_Config(RCC_HCLK_DIV1);
  RCC_PCLKPRS_Config(RCC_PCLK_DIV1);
  
  /* 2. ʹ��PLL��ͨ��PLL��Ƶ��64MHz */
  RCC_PLL_Enable(RCC_PLLSOURCE_HSI, 8000000, 6);     // HSI Ĭ�����Ƶ��8MHz
 // RCC_PLL_OUT();  //PC13�����PLLʱ��
  
  ///< ��ʹ�õ�ʱ��ԴHCLK����24M,С�ڵ���48MHz������FLASH ���ȴ�����Ϊ2 cycle
  ///< ��ʹ�õ�ʱ��ԴHCLK����48MHz������FLASH ���ȴ�����Ϊ3 cycle
  __RCC_FLASH_CLK_ENABLE();
  FLASH_SetLatency(FLASH_Latency_3);   
    
  /* 3. ʱ���л���PLL */
  RCC_SysClk_Switch(RCC_SYSCLKSRC_PLL);
  RCC_SystemCoreClockUpdate(48000000);	
	RCC_PCLKPRS_Config(RCC_PCLK_DIV8); //����HCLK �� PCLK�ķ�Ƶϵ��  6MHz
}


void KEYGPIO_Init(void)
{
	__RCC_GPIOB_CLK_ENABLE();//��GPIOB��ʱ��	
	__RCC_GPIOC_CLK_ENABLE();//��GPIOC��ʱ��	
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
	RCC_Configuration();   //ϵͳʱ��64M
	KEYGPIO_Init();
	GPIO_WritePin(CW_GPIOC,GPIO_PIN_13,GPIO_Pin_RESET); 
	Seg_Init();
	Btim1_Init();
	ADC_init(); 
		
	while(1)
	{ 
		if(BrushFlag==1)
			{
				DisplayValue(V_Buffer);  //������ʾ
				BrushFlag=0;
			}
			
			if(timecount>= 300)     //300ms�ı�һ���������ʾֵ//	
			{
				timecount=0;
				Volt_Cal();  //��ѹ�ɼ�
				BrushFlag=1;
			}			
	}
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

void Volt_Cal(void)
{ 	
	float t,KT1;
	
	V_Buffer = Mean_Value_Filter(Volt_Buffer,ADC_SAMPLE_SIZE);//ʹ�þ�ֵ�˲�
	
	V_Buffer = V_Buffer * ADC_REF_VALUE * 10>> 12; //0.1mVΪ��λ  Ad����ӦΪ1*4096*15000 ��0.3mV  һ������ƫ��0.3mV 
}	

void BTIM1_IRQHandler(void)
{
  static uint32_t Cnt=0,keytime=0,timecount1=0,ledcount=0;
	
  /* USER CODE BEGIN */
  if (BTIM_GetITStatus(CW_BTIM1, BTIM_IT_OV))
  {
    BTIM_ClearITPendingBit(CW_BTIM1, BTIM_IT_OV);
    Get_ADC_Value();
		
		ledcount++;  //LED��
		if(ledcount>=1000)
		{PC13_TOG();ledcount=0;}
		
		
		timecount++;
    Dis_Refresh();//�����ɨ����ʾ		
		 
  }
  /* USER CODE END */
}






