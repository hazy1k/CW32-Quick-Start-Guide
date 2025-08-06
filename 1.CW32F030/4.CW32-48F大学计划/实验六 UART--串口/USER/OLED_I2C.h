#ifndef __OLED_I2C_H
#define	__OLED_I2C_H

#include "main.h"

#define OLED_ADDRESS	0x78 //通过调整0R电阻,屏可以0x78和0x7A两个地址 -- 默认0x78
#define COM				0x00  // OLED 指令（禁止修改）  写命令
#define DAT 			0x40  // OLED 数据（禁止修改）  写数据


#define UP		8		//上
#define DOWN	2		//下
#define LEFT	4		//左
#define RIGHT	6		//右

#define I2C_PIC_W		128
#define I2C_PIC_H		8		//画布大小
#define I2C_OLED_W		128
#define I2C_OLED_H		8		//显示屏大小



static void I2C_OLED_WR_Byte(unsigned dat,unsigned cmd);
static void I2C_OLED_Write_Command(unsigned char IIC_Command);
static void I2C_OLED_Write_Data(unsigned char IIC_Data);
static void I2C_OLED_Byte_Write(uint8_t addr,uint8_t data);

void I2C_OLED_Init(void);
void I2C_init(void);

void I2C_OLED_ColorTurn(uint8_t i);
void I2C_OLED_DisplayTurn(uint8_t i);
void I2C_OLED_DisPlay_On(void);
void I2C_OLED_DisPlay_Off(void);
void I2C_OLED_UPdata(void);
void I2C_OLED_Clear_Line(uint8_t Line,uint8_t Start_x,uint8_t End_x);
void I2C_OLED_Clear(uint8_t OLEDShowUP);
void I2C_OLED_DrawPoint(uint8_t x,uint8_t y);
void I2C_OLED_ClearPoint(uint8_t x,uint8_t y);
void I2C_OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2);
void I2C_OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void I2C_OLED_ShowChar(uint8_t x,uint8_t y,char chr);
void I2C_OLED_ShowChar_CH(uint8_t x,uint8_t y,uint16_t usChar);
void I2C_OLED_ShowString(uint8_t x,uint8_t y,char *chr);
void I2C_OLED_Move(uint8_t dir,uint8_t pix,uint8_t cir);
void I2C_OLED_Print(char *str);
void I2C_OLED_WR_BP(uint8_t x,uint8_t y);
void I2C_OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t Bmp_W,uint8_t Bmp_H,uint8_t *BMP);


#endif
