#include "font.h"



uint8_t everRead=0;//记录Flash是否初始化	0没有初始化，1初始化

sFONT Font8x16 = {
  10*1024, /*数据在Flash开始地址*/
  8, /* Width */
  16, /* Height */
#if !FLASH_FONT	//当使用内存中的字库时，字体内存地址
  ASCII8x16_Table,	/* 字体内存地址 */
#endif
};

sFONT Font16x24 = {
  12*1024, /*开始地址*/
  16, /* Width */
  24, /* Height */
#if !FLASH_FONT	//当使用内存中的字库时，字体内存地址
  ASCII16x24_Table,	/* 字体内存地址 */
#endif
};

sFONT Font24x32 = {
  17*1024, /*开始地址*/
  24, /* Width */
  32, /* Height */
#if !FLASH_FONT	//当使用内存中的字库时，字体内存地址
  ASCII24x32_Table,	/* 字体内存地址 */
#endif
};
sFONT GB2312_H1616 = {
  256*1024, /*开始地址*/
  WIDTH_CH_CHAR, /* Width */
  HEIGHT_CH_CHAR, /* Height */
  /* 中文没有内存地址，内存占用太大了 */
};


//选择使用FLASH字模还是SD卡的字模
#if GBKCODE_FLASH


// 0：HZLIB字模(旧版，不建议使用), 1：GB2312字模
#if 1

/**
  * @brief  获取FLASH中文显示字库数据
  * @param  pBuffer:存储字库矩阵的缓冲区
  * @param  c ： 要获取的文字
  * @retval None.
  */
uint8_t GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
{
#if FLASH_FONT	//当使用内存中的字库时，不支持中文
	unsigned char High8bit,Low8bit;
	unsigned int pos,datnum;
	
	High8bit= c >> 8;     /* 取高8位数据 */
	Low8bit= c & 0x00FF;  /* 取低8位数据 */
	
	//要读取的字节数量
	datnum=GB2312_H1616.Width*GB2312_H1616.Height/8;
	/*GB2312 公式*/	
	pos = ( (High8bit-0xa1)*94 + Low8bit-0xa1 )*datnum;
	//读取字库数据  
	SPI_FLASH_BufferRead(pBuffer , GB2312_H1616.AddrStart + pos , datnum);
#else
	FONT_Bebug("当前字体不支持中文\n");
#endif
	return 0;  
}

/**
  * @brief  获取Flash中的英文字符
  * @param  pBuffer:存储字库矩阵的缓冲区
  * @param  c ： 要获取的文字
  * @param  Fonts： 文字字体
  * @retval None.
  */
uint8_t GetConslonsCode_from_EXFlash( uint8_t * pBuffer, const char c , sFONT *Fonts)
{
	unsigned char chr;
	unsigned int pos,datnum;
	
	chr = c - ' ';
	
	//要读取的字节数量(该字体下占datnum个字节)
	datnum = (Fonts->Width * Fonts->Height)/8;
	/*字模偏移地址*/
	pos = chr*datnum;
	#if FLASH_FONT
	//读取字库数据  
	SPI_FLASH_BufferRead(pBuffer , Fonts->AddrStart + pos , datnum);
	#else
	for(uint16_t i=0;i<datnum;i++)
	{
		*(pBuffer+i) = (Fonts->Font_Table[pos+i]);//获取该字体的某个字符编码首地址
	}
	#endif
	return 0;  
}


#else	//旧版字模HZLIB 配套的函数

	/*HZLIB中文字库存储在FLASH的起始地址*/
	#define HZLIB_START_ADDRESS   1*4096

	//旧版HZLIB字库使用的函数，HZLIB不支持标点
	//本函数使用的寻址公式跟GB2312的也稍有区别
	int GetGBKCode_from_EXFlash( uint8_t * pBuffer, uint16_t c)
	{ 
		unsigned char High8bit,Low8bit;
		unsigned int pos;
		static uint8_t everRead=0;
		
		High8bit= c >> 8;     /* 取高8位数据 */
		Low8bit= c & 0x00FF;  /* 取低8位数据 */	
		
		//本字模使用的寻址公式跟GB2312的稍有区别
		pos = ((High8bit-0xa0-16)*94+Low8bit-0xa1)*WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8;
		//地址指向HZLIB字模的地址HZLIB_START_ADDRESS
		SPI_FLASH_BufferRead(pBuffer,HZLIB_START_ADDRESS+pos,WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8);
		//printf ( "%02x %02x %02x %02x\n", pBuffer[0],pBuffer[1],pBuffer[2],pBuffer[3]);
		
		return 0;
	}
	#endif

#else //SD卡字模

	static FIL fnew;	/* file objects */
	static FATFS fs;	/* Work area (file system object) for logical drives */
	static FRESULT res_sd; 
	static UINT br;		/* File R/W count */

	/**
	  * @brief  获取SD卡中文显示字库数据
		* @param  pBuffer:存储字库矩阵的缓冲区
		* @param  c ： 要获取的文字
	  * @retval None.
	  */
	int GetGBKCode_from_sd ( uint8_t * pBuffer, uint16_t c)
	{ 
		unsigned char High8bit,Low8bit;
		unsigned int pos;
		static uint8_t everRead = 0;
		
		High8bit= c >> 8;     /* 取高8位数据 */
		Low8bit= c & 0x00FF;  /* 取低8位数据 */
			
		pos = ((High8bit-0xa1)*94+Low8bit-0xa1)*WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8;
		
		/*第一次使用，挂载文件系统，初始化sd*/
		if(everRead == 0)
		{
			res_sd = f_mount(&fs,"0:",1);
			everRead = 1;
		}
		res_sd = f_open(&fnew , GBKCODE_FILE_NAME, FA_OPEN_EXISTING | FA_READ);
		if ( res_sd == FR_OK ) 
		{
			f_lseek (&fnew, pos);		//指针偏移
			f_read( &fnew, pBuffer, WIDTH_CH_CHAR*HEIGHT_CH_CHAR/8, &br );//16*16大小的汉字 其字模 占用16*16/8个字节
			f_close(&fnew);
			return 0;  
		}
		else
			return -1;    
	}

#endif






