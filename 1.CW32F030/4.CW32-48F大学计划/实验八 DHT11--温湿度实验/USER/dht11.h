#ifndef __DHT11_H
#define __DHT11_H 
#include "main.h"
 

uint8_t DHT11_Init(void);//��ʼ��DHT11
uint8_t DHT11_Read_Data(float *temp,uint8_t *humi);  
uint8_t DHT11_Read_Byte(void);//����һ���ֽ�
uint8_t DHT11_Read_Bit(void);//����һ��λ
uint8_t DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11   
uint8_t DHT11_GPIO_Config ( void );


#endif















