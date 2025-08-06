#include "Seg_Display.h"

/*  ��������ܱ����
 0x3f   0x06   0x5b  0x4f  0x66  0x6d  0x7d  0x07  0x7f  0x6f 
  0      1      2     3     4     5     6     7     8     9 
 0xbf   0x86   0xdb  0xcf  0xe6  0xed  0xfd  0x87  0xff  0xef           
  0.     1.     2.    3.    4.    5.    6.    7.    8.    9.          */
	
uint8_t Seg_Table[20]     = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f,
                             0xbf, 0x86, 0xdb, 0xcf, 0xe6, 0xed, 0xfd, 0x87, 0xff, 0xef};
uint8_t Seg_Reg[3] = {0,0,0};

void Seg_Configuration(void)
{
  __RCC_GPIOA_CLK_ENABLE();//��GPIOA��ʱ��
	__RCC_GPIOB_CLK_ENABLE();//��GPIOB��ʱ��
	
	GPIO_InitTypeDef GPIO_InitStruct; 
		
	GPIO_InitStruct.Pins = GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_4 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15; 
	//A:PA02;B:PA00;C:PA04;D:PA06;E:PA07;F:PA01;G:PA03;DP:PA05;COM1:PA08;COM2:PA11;COM3:PA12;COM4:PA15
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOA, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pins = GPIO_PIN_3 | GPIO_PIN_4; //COM5:PB03;COM6:PB04
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.IT = GPIO_IT_NONE;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
	
	//��ʼ��ȫ���ر�
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);   //COM1:PA08
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);  //COM2:PA11
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);  //COM3:PA12
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_15,GPIO_Pin_SET);  //COM4:PA15
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_3,GPIO_Pin_SET);   //COM5:PB03
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_4,GPIO_Pin_SET);   //COM6:PB04
}

void Seg_Dis(uint8_t Pos,uint8_t Num)
{
	int i;
	uint8_t Dis_Value = Seg_Table[Num];
	
	for(i = 0; i < 8; i++)
	{
      switch(i)
        {
          case 0:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_2,(Dis_Value >> i) & 0x01);    //PA02,A
              break;
          case 1:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_0,(Dis_Value >> i) & 0x01);    //PA00,B
              break;
          case 2:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_4,(Dis_Value >> i) & 0x01);    //PA04,C
              break;
          case 3:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_6,(Dis_Value >> i) & 0x01);    //PA06,D
              break;
          case 4:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_7,(Dis_Value >> i) & 0x01);    //PA07,E
              break;
          case 5:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_1,(Dis_Value >> i) & 0x01);    //PA01,F
              break;
          case 6:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_3,(Dis_Value >> i) & 0x01);    //PA03,G
              break;
          case 7:
              GPIO_WritePin(CW_GPIOA,GPIO_PIN_5,(Dis_Value >> i) & 0x01);    //PA05,DP
              break;
          default:
              break;
				}
	}
	
	switch(Pos)
	{
	  case 0:
      GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_RESET);   //PA08,COM1
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);    //PA11,COM2
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);    //PA12,COM3
      break;
    case 1:
      GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);     //PA08,COM1
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_RESET);  //PA11,COM2
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);    //PA12,COM3
      break;
    case 2:
      GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);     //PA08,COM1
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);    //PA11,COM2
		  GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_RESET);  //PA12,COM3
      break;
		default:
      break;
	}
}

/**
 * @brief �ر����й�����
 * 
 */
void Close_Com(void)
{
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);   //COM1:PA08
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);  //COM2:PA11
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);  //COM3:PA12
	GPIO_WritePin(CW_GPIOA,GPIO_PIN_15,GPIO_Pin_SET);  //COM4:PA15
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_3,GPIO_Pin_SET);   //COM5:PB03
	GPIO_WritePin(CW_GPIOB,GPIO_PIN_4,GPIO_Pin_SET);   //COM6:PB04
}

/**
 * @brief ���㴢��Ҫ��ʾ��������Seg_Reg��
 *
 */
void Dynamic_Display(uint32_t value)
{
  uint8_t Thousands = 0;
  uint8_t Hundreds = 0;
  uint8_t Tens = 0;
  uint8_t Units = 0; // ��λ��
	
	Thousands = value / 1000;
	
	 if(Thousands > 0)
    {
       Units     = value % 10;
       value     = Units > 5 ? (value + 10) : value; // ���ݺ�һλ��������
       Thousands = value / 1000 % 10;
       Hundreds  = value / 100 % 10;
       Tens      = value / 10 % 10;
			
       // ��ʾxx.x��
       Seg_Reg[0] = Thousands;
       Seg_Reg[1] = Hundreds + 10; // ��dp��ʾ
       Seg_Reg[2] = Tens;
		}
		
	 else
	   {
	     Units     = value % 10;
	     Tens      = value / 10 % 10;
       Hundreds  = value / 100 % 10;
	     
			 // ��ʾx.xx��
	     Seg_Reg[0] = Hundreds + 10;              // ��dp��ʾ
	     Seg_Reg[1] = Tens;
	     Seg_Reg[2] = Units;
	   }
}

/**
 * @brief �����ɨ����ʾ����,��ʱ�������Ե���
 *
 */
void Display_Refresh(void)
{
  static uint8_t num;

	Close_Com();//�ȹرչ�����,��ֹ��Ӱ
	Seg_Dis(num,Seg_Reg[num]);
	num++;
  if(num > 2)
  {
    num = 0;
  }
}
