#include "seg.h"

// 共阴极数码管段码表
uint8_t seg_table[16] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
// 三位共阴数码管
uint8_t seg_reg[3] = {0, 0, 0};

void SEG_GPIO_Init(void)
{
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.Pins = GPIO_PIN_2 | GPIO_PIN_0 | GPIO_PIN_4 |GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_1 | GPIO_PIN_3 | GPIO_PIN_5 | GPIO_PIN_8 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_15;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.IT = GPIO_IT_NONE;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOB, &GPIO_InitStructure);

    GPIO_WritePin(CW_GPIOA, GPIO_PIN_8,  GPIO_Pin_SET);  // COM1:PA08
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_11, GPIO_Pin_SET); // COM2:PA11
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_12, GPIO_Pin_SET); // COM3:PA12
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_15, GPIO_Pin_SET); // COM4:PA15
	GPIO_WritePin(CW_GPIOB, GPIO_PIN_3,  GPIO_Pin_SET);  // COM5:PB03
	GPIO_WritePin(CW_GPIOB, GPIO_PIN_4,  GPIO_Pin_SET);  // COM6:PB04
}

void SEG_Display(uint8_t Pos, uint8_t Num) // 数码管显示函数，函数参数：位置，数字
{
    int i;
	uint8_t Dis_Value = seg_table[Num];
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
	    case 1:
            GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_RESET);   //PA08,COM1
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);    //PA11,COM2
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);    //PA12,COM3
            break;
        case 2:
            GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);     //PA08,COM1
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_RESET);  //PA11,COM2
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_SET);    //PA12,COM3
            break;
        case 3:
            GPIO_WritePin(CW_GPIOA,GPIO_PIN_8,GPIO_Pin_SET);     //PA08,COM1
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_11,GPIO_Pin_SET);    //PA11,COM2
		    GPIO_WritePin(CW_GPIOA,GPIO_PIN_12,GPIO_Pin_RESET);  //PA12,COM3
            break;
		default:
            break;
	}
}

// 关闭所有公共端
void SEG_Close(void)
{
    GPIO_WritePin(CW_GPIOA, GPIO_PIN_8,  GPIO_Pin_SET);   
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_11, GPIO_Pin_SET);  
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_12, GPIO_Pin_SET);  
	GPIO_WritePin(CW_GPIOA, GPIO_PIN_15, GPIO_Pin_SET);  
	GPIO_WritePin(CW_GPIOB, GPIO_PIN_3,  GPIO_Pin_SET);   
	GPIO_WritePin(CW_GPIOB, GPIO_PIN_4,  GPIO_Pin_SET);   
}

// 分别存储到三位数码管
void Three_Seg_Display(uint32_t value)
{
    uint8_t Hundreds;
    uint8_t Tens;
    uint8_t Units;
    Hundreds = value / 100 % 10;
    Tens = value / 10 % 10;
    Units = value % 10;
    seg_reg[0] = Hundreds;
    seg_reg[1] = Tens;
    seg_reg[2] = Units;
}

// 数码管扫描函数
void SEG_Scan(void)
{
    static uint8_t num;
    SEG_Close();
    SEG_Display(num, seg_reg[num-1]);
    num++;
    if(num > 3)
    {
        num = 1;
    }
}
