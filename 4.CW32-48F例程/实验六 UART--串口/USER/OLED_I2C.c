
#include "OLED_I2C.h"

#include "font.h"


sFONT *Fonttemp = &Font8x16;
//sFONT *Fonttemp = &Font16x24;
//sFONT *Fonttemp = &Font24x32;

short I2C_OFFSET_W = I2C_PIC_W / 2 - I2C_OLED_W / 2;
short I2C_OFFSET_H = I2C_PIC_H / 2 - I2C_OLED_H / 2; //显示器与画布偏移量

uint8_t I2C_P_x = 0, I2C_P_y = 0; //OLED_Print函数中字符坐标
uint8_t I2C_OLED_GRAM[I2C_PIC_H][I2C_PIC_W]; //画布256*128，显示区域(64,32)~(191,95)



void I2C_init(void)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
	  I2C_InitTypeDef I2C_InitStruct;
	
		__RCC_I2C1_CLK_ENABLE();
		__RCC_GPIOB_CLK_ENABLE();	
    
	  PB06_AFx_I2C1SCL();
	  PB07_AFx_I2C1SDA(); 

		GPIO_InitStructure.Pins = GPIO_PIN_6 | GPIO_PIN_7;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;   ////I2C必须开漏输出
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
	
		I2C_InitStruct.I2C_BaudEn = ENABLE;
		I2C_InitStruct.I2C_Baud = 0x04;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA = DISABLE;  //DISABLE ENABLE
    
		I2C1_DeInit(); 
    I2C_Master_Init(CW_I2C1,&I2C_InitStruct);//初始化模块
		I2C_Cmd(CW_I2C1,ENABLE);  //模块使能
}

void I2C_MasterWriteEepromData1(I2C_TypeDef *I2Cx,uint8_t u8Addr,uint8_t *pu8Data,uint32_t u32Len)
{
    uint8_t u8i=0,u8State;
	I2C_GenerateSTART(I2Cx, ENABLE);
	while(1)
	{
		while(0 == I2C_GetIrq(I2Cx))
		{;}
		u8State = I2C_GetState(I2Cx);
		switch(u8State)
		{
			case 0x08:   //发送完START信号
				I2C_GenerateSTART(I2Cx, DISABLE);
				I2C_Send7bitAddress(I2Cx, OLED_ADDRESS,0X00);  //从设备地址发送
				break;
			case 0x18:   //发送完SLA+W信号,ACK已收到
				I2C_SendData(I2Cx,u8Addr);//从设备内存地址发送
				break;
			case 0x28:   //发送完1字节数据：发送EEPROM中memory地址也会产生，发送后面的数据也会产生	
				I2C_SendData(I2Cx,pu8Data[u8i++]);
				break;
			case 0x20:   //发送完SLA+W后从机返回NACK
			case 0x38:    //主机在发送 SLA+W 阶段或者发送数据阶段丢失仲裁  或者  主机在发送 SLA+R 阶段或者回应 NACK 阶段丢失仲裁
				I2C_GenerateSTART(I2Cx, ENABLE);
				break;
			case 0x30:   //发送完一个数据字节后从机返回NACK
				I2C_GenerateSTOP(I2Cx, ENABLE);
				break;
			default:
				break;
		}			
		if(u8i>u32Len)
		{
			I2C_GenerateSTOP(I2Cx, ENABLE);//此顺序不能调换，出停止条件
			I2C_ClearIrq(I2Cx);
			break;
		}
		I2C_ClearIrq(I2Cx);			
	}
}


void WriteCmd(unsigned char I2C_Command)//写命令
{
	I2C_MasterWriteEepromData1(CW_I2C1,COM,&I2C_Command,1);
}

void WriteDat(unsigned char I2C_Data)//写数据
{
	I2C_MasterWriteEepromData1(CW_I2C1,DAT,&I2C_Data,1);
}

//开启OLED显示
void I2C_OLED_DisPlay_On(void)
{
	WriteCmd(0x8D); //电荷泵使能
	WriteCmd(0x14); //开启电荷泵
	WriteCmd(0xAF); //点亮屏幕
}

//关闭OLED显示
void I2C_OLED_DisPlay_Off(void)
{
	WriteCmd(0x8D); //电荷泵使能
	WriteCmd(0x10); //关闭电荷泵
	WriteCmd(0xAF); //关闭屏幕
}


//更新显存到OLED
void I2C_OLED_UPdata(void)
{
	uint8_t i, n;
	//水平模式下设置
	WriteCmd(0x21); //设置列
	WriteCmd(0x00); //设置列起始地址
	WriteCmd(0x7f); //设置列结束地址
	WriteCmd(0x22); //设置页
	WriteCmd(0x00); //设置页起始地址
	WriteCmd(0x07); //设置页结束地址
	for (i = 0; i < I2C_OLED_H; i++)
	{
		for (n = 0; n < I2C_OLED_W; n++)
		{
			WriteDat(I2C_OLED_GRAM[i + I2C_OFFSET_H][n + I2C_OFFSET_W]);
		}
	}
}

/**
  * @brief  清除某条线的数据
  * @param  Line:选择要清除的行，0、1、2、3、4、5、6、7
  * @param  Start_x:x轴开始坐标
  * @param  End_x:  x轴结束坐标
  * @retval 无
  */
void I2C_OLED_Clear_Line(uint8_t Line,uint8_t Start_x,uint8_t End_x)
{
	for(uint8_t x=Start_x;x<End_x;x++)
		I2C_OLED_GRAM[Line][x]=0;
}


//清屏函数
//选择位置清屏
//Inside,Outside,All
void I2C_OLED_Clear(uint8_t OLEDShowUP)
{
	uint16_t i, n;
	for (i = 0; i < I2C_PIC_H; i++)
	{
		for (n = 0; n < I2C_PIC_W; n++)
		{
			I2C_OLED_GRAM[i][n] = 0; //清除所有数据
		}
	}
	I2C_P_x = 0;
	I2C_P_y = 0; //清除OLED_Print函数中写入字符坐标
	I2C_OFFSET_W = I2C_PIC_W / 2 - I2C_OLED_W / 2;
	I2C_OFFSET_H = I2C_PIC_H / 2 - I2C_OLED_H / 2; //重置显示器与画布偏移量为画布中心
	if (OLEDShowUP == 1)
		I2C_OLED_UPdata(); //更新显示
}

//画点
//x:0~127
//y:0~63
void I2C_OLED_DrawPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8; //获得坐标所在页数
	m = y % 8; //获得偏移位数
	n = 1 << m;
	I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W] |= n; //将数据传入图像帧
}


//清除一个点
//x:0~127
//y:0~63
void I2C_OLED_ClearPoint(uint8_t x, uint8_t y)
{
	uint8_t i, m, n;
	i = y / 8;
	m = y % 8;
	n = 1 << m;
	I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W] = ~I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W];
	I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W] |= n;
	I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W] = ~I2C_OLED_GRAM[i + I2C_OFFSET_H][x + I2C_OFFSET_W]; //清除点
}

//画线
//x:0~128
//y:0~64
void I2C_OLED_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	int i = 0;
	float k, b;
	if (x1 == x2) //画竖线
	{
		for (i = 0; i < (y2 - y1); i++)
		{
			I2C_OLED_DrawPoint(x1, y1 + i);
		}
	}
	/*else if(y1==y2)   //画横线
	{
			for(i=0;i<(x2-x1);i++)
			{
				OLED_DrawPoint(x1+i,y1);
			}
  }*/
	else //画斜线
	{
		k = (y2 - y1) * 10 / (x2 - x1); //斜率*10防止斜率小于1时失真
		b = y1 - k * x1 / 10;
		while ((x1 + i) != x2)
		{
			I2C_OLED_DrawPoint(x1 + i, (int)((x1 + i) * k / 10 + b));
			if (x1 < x2)
			{
				i++;
			}
			else
			{
				i--;
			}
		}
	}
}


//x,y:圆心坐标
//r:圆的半径
void I2C_OLED_DrawCircle(uint8_t x, uint8_t y, uint8_t r)
{
	int a, b, num;
	a = 0;
	b = r;
	while (2 * b * b >= r * r)
	{
		I2C_OLED_DrawPoint(x + a, y - b);
		I2C_OLED_DrawPoint(x - a, y - b);
		I2C_OLED_DrawPoint(x - a, y + b);
		I2C_OLED_DrawPoint(x + a, y + b);

		I2C_OLED_DrawPoint(x + b, y + a);
		I2C_OLED_DrawPoint(x + b, y - a);
		I2C_OLED_DrawPoint(x - b, y - a);
		I2C_OLED_DrawPoint(x - b, y + a);

		a++;
		num = (a * a + b * b) - r * r; //计算画的点离圆心的距离
		if (num > 0)
		{
			b--;
			a--;
		}
	}
}


void I2C_OLED_ShowChar(uint8_t x, uint8_t y, char chr)
{
	uint8_t i, m, temp, size = Fonttemp->Height * Fonttemp->Width / 8;
	uint8_t x0 = x;
	uint8_t Pfont[size];

	//获取字符编码
	GetConslonsCode(Pfont, chr, Fonttemp);

	//	size=Fonttemp->Height*Fonttemp->Width/8;  //得到字体一个字符对应点阵集所占的字节数
	for (i = 0; i < size; i++)
	{
		temp = Pfont[i];
		for (m = 0; m < 8; m++) //写入数据
		{
			if (temp & 0x80)
				I2C_OLED_DrawPoint(x, y);
			else
				I2C_OLED_ClearPoint(x, y);
			temp <<= 1;
			x++;
		}
		if ((x - x0) == (Fonttemp->Width))
		{
			x = x0;
			y++;
		}
	}
}


void I2C_OLED_ShowChar_CH(uint8_t x, uint8_t y, uint16_t usChar)
{
	uint8_t rowCount, bitCount;
	uint8_t ucBuffer[WIDTH_CH_CHAR * HEIGHT_CH_CHAR / 8];
	uint16_t usTemp;
	uint8_t x0 = x;
	//取字模数据
	GetGBKCode(ucBuffer, usChar);

	for (rowCount = 0; rowCount < HEIGHT_CH_CHAR; rowCount++)
	{
		/* 取出两个字节的数据，在lcd上即是一个汉字的一行 */
		usTemp = ucBuffer[rowCount * 2]; //取高位
		usTemp = (usTemp << 8);
		usTemp |= ucBuffer[rowCount * 2 + 1]; //取低位

		for (bitCount = 0; bitCount < WIDTH_CH_CHAR; bitCount++)
		{
			if (usTemp & (0x8000 >> bitCount)) //高位在前
				I2C_OLED_DrawPoint(x, y);
			else
				I2C_OLED_ClearPoint(x, y);
			x++;
		}
		if (x - x0 == WIDTH_CH_CHAR)
		{
			x = x0;
			y++;
		}
	}
}

//dir 移动方向
//pix 移动像素
//cir 画布如果到达边界时，回到对面边界，到达循环效果,0表示不循环，1表示循环
void I2C_OLED_Move(uint8_t dir, uint8_t pix, uint8_t cir) //屏幕显示区域移动
{
	switch (dir)
	{
	case UP:
	{
		I2C_OFFSET_H += pix;
		if (I2C_OFFSET_H >= I2C_PIC_H - I2C_OLED_H)
		{
			if (cir == 0)
				I2C_OFFSET_H = I2C_PIC_H - I2C_OLED_H;
			else
				I2C_OFFSET_H = 0;
		}
		break;
	}
	case DOWN:
	{
		I2C_OFFSET_H -= pix;
		if (I2C_OFFSET_H <= 0)
		{
			if (cir == 0)
				I2C_OFFSET_H = 0;
			else
				I2C_OFFSET_H = I2C_PIC_H - I2C_OLED_H;
		}
		break;
	}
	case LEFT:
	{
		I2C_OFFSET_W += pix;
		if (I2C_OFFSET_W >= I2C_PIC_W - I2C_OLED_W)
		{
			if (cir == 0)
				I2C_OFFSET_W = I2C_PIC_W - I2C_OLED_W;
			else
				I2C_OFFSET_W = 0;
		}
		break;
	}
	case RIGHT:
	{
		I2C_OFFSET_W -= pix;
		if (I2C_OFFSET_W <= 0)
		{
			if (cir == 0)
				I2C_OFFSET_W = 0;
			else
				I2C_OFFSET_W = I2C_PIC_W - I2C_OLED_W;
		}
		break;
	}
	}
	I2C_OLED_UPdata();
}

/**
 * @brief  处理转义字符
 * @param  usX ：当前坐标X
 * @param  usY ：当前坐标Y
 * @param  pStr ：要显示的英文字符
 * @note 
 * @retval 处理字符串中的转义字符，并返回处理后的字符串地址
 */
char* OLED_StrEscCharacter(uint8_t *x,uint8_t *y,char *chr)
{
	uint8_t TabSize;
	while(*chr < ' '&&*chr != '\0')
	{
		switch (*chr)
		{
			case '\b': //退格（BS）
			{
				*x = *x - Fonttemp->Width;
				break;
			}
			case '\n': //换行（LF）
			{
				switch(Fonttemp->Height)
				{
					case 32:I2C_OLED_Clear_Line((*y/8)+3,*x,128);//清除第3行
					case 24:I2C_OLED_Clear_Line((*y/8)+2,*x,128);//清除第2行
					case 16:I2C_OLED_Clear_Line((*y/8)+1,*x,128);//清除第1行
					case 8:I2C_OLED_Clear_Line((*y/8),*x,128);break;//清除第0行
				}
				*x = 0;
				*y = *y + Fonttemp->Height;
				if (*y > (I2C_OLED_H * 8) - Fonttemp->Height) //屏幕写满
				{
					I2C_OLED_Move(UP, 2, 0);
					*y = *y - Fonttemp->Height;
				}
				break;
			}
			case '\r': //回车（CR）
			{
				switch(Fonttemp->Height)
				{
					case 32:I2C_OLED_Clear_Line((*y/8)+3,*x,128);//清除第3行
					case 24:I2C_OLED_Clear_Line((*y/8)+2,*x,128);//清除第2行
					case 16:I2C_OLED_Clear_Line((*y/8)+1,*x,128);//清除第1行
					case 8:I2C_OLED_Clear_Line((*y/8),*x,128);break;//清除第0行
				}
				*x = 0;
				*y = *y + Fonttemp->Height;
				if (*y > (I2C_OLED_H * 8) - Fonttemp->Height) //屏幕写满
				{
					I2C_OLED_Move(UP, 2, 0);
					*y = *y - Fonttemp->Height;
				}
				break;
			}
			case '\t': //水平制表（HT）
			{
				TabSize = 4 * Fonttemp->Width - (*x % (4 * Fonttemp->Width));
				switch(Fonttemp->Height)
				{
					case 32:I2C_OLED_Clear_Line((*y/8)+3,*x,*x+TabSize);//清除第3行
					case 24:I2C_OLED_Clear_Line((*y/8)+2,*x,*x+TabSize);//清除第2行
					case 16:I2C_OLED_Clear_Line((*y/8)+1,*x,*x+TabSize);//清除第1行
					case 8:I2C_OLED_Clear_Line((*y/8),*x,*x+TabSize);break;//清除第0行
				}
				*x = *x + TabSize;
				break;
			}
		}
		chr++;
	}
	return chr;
}

//显示字符串
//x,y:起点坐标
//size1:字体大小
//*chr:字符串起始地址
void I2C_OLED_ShowString(uint8_t x, uint8_t y, char *chr)
{
	uint16_t chinese_chr;
	while (*chr != '\0') //判断是不是非法字符!
	{
		chr=OLED_StrEscCharacter(&x,&y,chr);
		if( * chr == '\0' )
			break;
		if (*chr < 127) //英文字符
		{
			I2C_OLED_ShowChar(x, y, *chr);
			x += Fonttemp->Width;
			if (x > 128 - Fonttemp->Width) //换行
			{
				x = 0;
				y += Fonttemp->Height;
			}
			chr++;
		}
		else if (*chr > 127) //中文字符
		{
			chinese_chr = *(uint16_t *)chr;
			chinese_chr = (chinese_chr << 8) + (chinese_chr >> 8); //得到中文字符
			I2C_OLED_ShowChar_CH(x, y, chinese_chr);
			x += WIDTH_CH_CHAR;
			if (x > 128 - WIDTH_CH_CHAR) //换行
			{
				x = 0;
				y += HEIGHT_CH_CHAR;
			}
			chr += 2;
		}
	}
}

//m^n
uint32_t I2C_OLED_Pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;
	while (n--)
	{
		result *= m;
	}
	return result;
}

//str指向要显示字符的指针
//size显示字符字号
void I2C_OLED_Print(char *str) //在OLED屏上打印字符串
{
	uint16_t chinese_str;

	while (*str != '\0')
	{
		str=OLED_StrEscCharacter(&I2C_P_x,&I2C_P_y,str);
		if( * str == '\0' )
			break;
		if (*str < 127) //英文字符
		{
			I2C_OLED_ShowChar(I2C_P_x, I2C_P_y, *str);
			I2C_P_x = I2C_P_x + Fonttemp->Width;
			if (I2C_P_x > 128 - Fonttemp->Width) //换行
			{
				I2C_P_x = 0;
				I2C_P_y = I2C_P_y + Fonttemp->Height;
				if (I2C_P_y > 32 - Fonttemp->Height) //屏幕写满
				{
					I2C_OLED_Move(UP, 2, 0);
					I2C_P_y = I2C_P_y - Fonttemp->Height;
				}
			}
			str++;
		}
		else //中文字符
		{
			chinese_str = *(uint16_t *)str;
			chinese_str = (chinese_str << 8) + (chinese_str >> 8); //得到中文字符
			I2C_OLED_ShowChar_CH(I2C_P_x, I2C_P_y, chinese_str);
			I2C_P_x += WIDTH_CH_CHAR;

			if (I2C_P_x > 128 - WIDTH_CH_CHAR) //换行
			{
				I2C_P_x = 0;
				I2C_P_y = I2C_P_y + HEIGHT_CH_CHAR;
				if (I2C_P_y > 32 - HEIGHT_CH_CHAR) //屏幕写满
				{
					I2C_OLED_Move(UP, 2, 0);
					I2C_P_y = I2C_P_y - HEIGHT_CH_CHAR;
				}
			}
			str += 2; //一个汉字两个字节
		}
	}
	I2C_OLED_UPdata();
}


void I2C_OLED_ShowPicture(uint8_t x, uint8_t y, uint8_t Bmp_W, uint8_t Bmp_H, uint8_t *BMP)
{
	uint8_t x0, y0; //临时储存x，y的值
	x0 = x;
	y0 = y;
	for (y = y0; y < y0 + Bmp_H; y++)
	{
		for (x = x0; x < x0 + Bmp_W; x++)
		{
			I2C_OLED_GRAM[y + I2C_OFFSET_H][x + I2C_OFFSET_W] = *BMP;
			BMP++;
		}
	}
	I2C_OLED_UPdata();
}


//OLED的初始化
void I2C_OLED_Init(void)
{
	delay1ms(200);

	WriteCmd(0xAE);//关闭显示
	
	WriteCmd(0x40);//---set low column address
	WriteCmd(0xB0);//---set high column address

	WriteCmd(0xC8);//-not offset

	WriteCmd(0x81);//设置对比度
	WriteCmd(0xff);

	WriteCmd(0xa1);//段重定向设置

	WriteCmd(0xa6);//
	
	WriteCmd(0xa8);//设置驱动路数
	WriteCmd(0x1f);
	
	WriteCmd(0xd3);
	WriteCmd(0x00);
	
	WriteCmd(0xd5);
	WriteCmd(0xf0);
	
	WriteCmd(0xd9);
	WriteCmd(0x22);
	
	WriteCmd(0xda);
	WriteCmd(0x02);
	
	WriteCmd(0x20); //-Set Page Addressing Mode (0x00/0x01/0x02)
	//WriteCmd(0x02);//页地址模式
	WriteCmd(0x00); //水平寻址模式

	WriteCmd(0xdb);
	WriteCmd(0x49);
	
	WriteCmd(0x8d);
	WriteCmd(0x14);
	
	WriteCmd(0xaf); 
	
}
