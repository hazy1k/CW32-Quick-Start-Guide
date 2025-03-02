#include "dht11.h"

	
//��λDHT11
void DHT11_Rst(void)	   
{                 
		PB01_DIR_OUTPUT(); 	//PB12���ó����
		PB01_SETLOW(); 	    //����DQ
	  delay1ms(20);
		PB01_SETHIGH(); 	  //DQ=1 
	  delay10us(3);
}
//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
  PB01_DIR_INPUT();  //PB12���ó�����
  while (PB01_GETVALUE()&&retry<10)//DHT11������40~80us
	{
		retry++;
		delay10us(1);
	};	 
	
	if(retry>=10)return 1;
	else retry=0;
  while (!PB01_GETVALUE()&&retry<10)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		delay10us(1);
	};

	if(retry>=10)return 1;	    
	return 0;
}
//��DHT11��ȡһ��λ
//����ֵ��1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(PB01_GETVALUE()&&retry<10)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		delay10us(1);
	}
	retry=0;
	while(!PB01_GETVALUE()&&retry<10)//�ȴ���ߵ�ƽ
	{
		retry++;
		delay10us(1);
	}
	delay10us(4);//�ȴ�40us
	if(PB01_GETVALUE())return 1;
	else return 0;		   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
uint8_t DHT11_Read_Byte(void)    
{        
    uint8_t i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}
//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
uint8_t DHT11_Read_Data(float *temp,uint8_t *humi)    
{        
// 	uint8_t buf[5];
	char buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}
//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 

uint8_t DHT11_GPIO_Config ( void )
{		
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_1;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
}
