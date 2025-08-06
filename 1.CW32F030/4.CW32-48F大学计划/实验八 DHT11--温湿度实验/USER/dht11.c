#include "dht11.h"

	
//复位DHT11
void DHT11_Rst(void)	   
{                 
		PB01_DIR_OUTPUT(); 	//PB12配置成输出
		PB01_SETLOW(); 	    //拉低DQ
	  delay1ms(20);
		PB01_SETHIGH(); 	  //DQ=1 
	  delay10us(3);
}
//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
uint8_t DHT11_Check(void) 	   
{   
	uint8_t retry=0;
  PB01_DIR_INPUT();  //PB12配置成输入
  while (PB01_GETVALUE()&&retry<10)//DHT11会拉低40~80us
	{
		retry++;
		delay10us(1);
	};	 
	
	if(retry>=10)return 1;
	else retry=0;
  while (!PB01_GETVALUE()&&retry<10)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		delay10us(1);
	};

	if(retry>=10)return 1;	    
	return 0;
}
//从DHT11读取一个位
//返回值：1/0
uint8_t DHT11_Read_Bit(void) 			 
{
 	uint8_t retry=0;
	while(PB01_GETVALUE()&&retry<10)//等待变为低电平
	{
		retry++;
		delay10us(1);
	}
	retry=0;
	while(!PB01_GETVALUE()&&retry<10)//等待变高电平
	{
		retry++;
		delay10us(1);
	}
	delay10us(4);//等待40us
	if(PB01_GETVALUE())return 1;
	else return 0;		   
}
//从DHT11读取一个字节
//返回值：读到的数据
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
//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
uint8_t DHT11_Read_Data(float *temp,uint8_t *humi)    
{        
// 	uint8_t buf[5];
	char buf[5];
	uint8_t i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
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
//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 

uint8_t DHT11_GPIO_Config ( void )
{		
	GPIO_InitTypeDef GPIO_InitStruct;
	
	__RCC_GPIOB_CLK_ENABLE();

  GPIO_InitStruct.IT = GPIO_IT_NONE; 
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pins = GPIO_PIN_1;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
}
