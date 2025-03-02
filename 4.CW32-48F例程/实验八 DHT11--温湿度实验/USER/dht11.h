#ifndef __DHT11_H
#define __DHT11_H 
#include "main.h"
 

uint8_t DHT11_Init(void);//初始化DHT11
uint8_t DHT11_Read_Data(float *temp,uint8_t *humi);  
uint8_t DHT11_Read_Byte(void);//读出一个字节
uint8_t DHT11_Read_Bit(void);//读出一个位
uint8_t DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11   
uint8_t DHT11_GPIO_Config ( void );


#endif















