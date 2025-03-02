/******************************************************************************
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PB6��SCL��
//              SDA   ��PB7��SDA��
//              RES   ��PA15
//              DC    ��PB4
//              CS    ��PB5 

*******************************************************************************/

#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111


#define LCD_CTRLA   	  	CW_GPIOA		//����TFT���ݶ˿�
#define LCD_CTRLB   	  	CW_GPIOB		//����TFT���ݶ˿�

#define LCD_SCL        	GPIO_PIN_6	//PB6--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_PIN_7	//PB7 MOSI--->>TFT --SDA/DIN
#define LCD_CS        	GPIO_PIN_5  //MCU_PB5--->>TFT --CS/CE

#define LCD_RS         	GPIO_PIN_4	//PB11--->>TFT --RS/DC
#define LCD_RST     	  GPIO_PIN_15	//PB10--->>TFT --RST


//Һ�����ƿ���1�������궨��
#define	LCD_SCL_SET  	LCD_CTRLB->BSRR=LCD_SCL    
#define	LCD_SDA_SET  	LCD_CTRLB->BSRR=LCD_SDA   
#define	LCD_CS_SET  	LCD_CTRLB->BSRR=LCD_CS  

    
#define	LCD_LED_SET  	LCD_CTRLA->BSRR=LCD_LED   
#define	LCD_RS_SET  	LCD_CTRLB->BSRR=LCD_RS 
#define	LCD_RST_SET  	LCD_CTRLA->BSRR=LCD_RST 
//Һ�����ƿ���0�������궨��
#define	LCD_SCL_CLR  	LCD_CTRLB->BRR=LCD_SCL  
#define	LCD_SDA_CLR  	LCD_CTRLB->BRR=LCD_SDA 
#define	LCD_CS_CLR  	LCD_CTRLB->BRR=LCD_CS 
    
#define	LCD_LED_CLR  	LCD_CTRLA->BRR=LCD_LED 
#define	LCD_RST_CLR  	LCD_CTRLA->BRR=LCD_RST
#define	LCD_RS_CLR  	LCD_CTRLB->BRR=LCD_RS 

#define LCD_DATAOUT(x) LCD_DATA->ODR=x; //�������
#define LCD_DATAIN     LCD_DATA->IDR;   //��������

#define LCD_WR_DATA(data){\
LCD_RS_SET;\
LCD_CS_CLR;\
LCD_DATAOUT(data);\
LCD_WR_CLR;\
LCD_WR_SET;\
LCD_CS_SET;\
} 



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(unsigned char Index);
void Lcd_WriteData(unsigned char Data);
void Lcd_WriteReg(unsigned char Index,unsigned char Data);
unsigned int Lcd_ReadReg(unsigned char LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(unsigned int Color);
void Lcd_SetXY(unsigned int x,unsigned int y);
void Gui_DrawPoint(unsigned int x,unsigned int y,unsigned int Data);
unsigned int Lcd_ReadPoint(unsigned int x,unsigned int y);
void Lcd_SetRegion(unsigned int x_start,unsigned int y_start,unsigned int x_end,unsigned int y_end);
void LCD_WriteData_16Bit(unsigned int Data);



void Redraw_Mainmenu(void);
void Num_Test(void);
void Font_Test(void);
void Color_Test(void);
void showimage(const unsigned char *p); 
void QDTFT_Test_Demo(void);
void Lcd_Clear_1(unsigned int Color) ;  

