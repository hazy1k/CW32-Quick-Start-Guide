/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-04-29
 * 
 * @copyright Copyright (c) 2021
 * 
 */
/*******************************************************************************
*
* ������ɺ�������Ϣ
* �人��Դ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人��Դ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人��Դ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
*
*******************************************************************************/
#include "main.h"

void RCC_Configuration(void);
void GPIO_Configuration(void);
void NVIC_Configuration(void);

#define TESTI2C  2    //I2C1 = 1   I2C2 = 2

#define  I2C1_SCL_GPIO_PORT       CW_GPIOB
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_10      //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�
#define  I2C1_SDA_GPIO_PORT       CW_GPIOB   
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_11      //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�

#define  I2C2_SCL_GPIO_PORT       CW_GPIOB
#define  I2C2_SCL_GPIO_PIN        GPIO_PIN_0       //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�
#define  I2C2_SDA_GPIO_PORT       CW_GPIOB   
#define  I2C2_SDA_GPIO_PIN        GPIO_PIN_1       //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�


uint8_t u8Recdata[30]={0x00};     //��Ž��յ�������
uint32_t u32SendLen=0;            //��Ž������ݵĳ���
uint32_t u32RecvLen=0;            //��ŷ������ݵĳ���
uint8_t u8State = 0;              //I2Cģ��״̬


/**
 * @brief 
 * 
 * @return int32_t 
 */
int32_t main(void)
{	
    I2C_InitTypeDef I2C_InitStruct;

    //ʱ�ӳ�ʼ��
    RCC_Configuration();
 
    //IO�ڳ�ʼ��
    GPIO_Configuration();
    
    //I2C��ʼ��
    I2C_InitStruct.I2C_OwnSlaveAddr0 = OwnSlaveAddress0;  //TEST ADDRESS0
		I2C_InitStruct.I2C_OwnSlaveAddr1 = OwnSlaveAddress1;  //TEST ADDRESS1
		I2C_InitStruct.I2C_OwnSlaveAddr2 = OwnSlaveAddress2;  //TEST ADDRESS2
		
    I2C_InitStruct.I2C_OwnGc = DISABLE;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA = DISABLE;
    #if(0X01 == TESTI2C)
        I2C1_DeInit(); 
        I2C_Slave_Init(CW_I2C1,&I2C_InitStruct);//��ʼ��ģ��
        I2C_Cmd(CW_I2C1, ENABLE);//ģ��ʹ��
        I2C_AcknowledgeConfig(CW_I2C1,ENABLE);//ACK��
    #elif(0X02 == TESTI2C)
        I2C2_DeInit(); 
        I2C_Slave_Init(CW_I2C2,&I2C_InitStruct);//��ʼ��ģ��
        I2C_Cmd(CW_I2C2, ENABLE);//ģ��ʹ��
        I2C_AcknowledgeConfig(CW_I2C2,ENABLE);//ACK��
    #endif   
		
	while(1)
	{
    #if(0X01 == TESTI2C)
        I2C_SlaveRecDataFromMaster(CW_I2C1,u8Recdata,&u32RecvLen);//������д����
        I2C_SlaveSendDataToMaster(CW_I2C1,u8Recdata,&u32SendLen);//������������
    #elif(0X02 == TESTI2C)
        I2C_SlaveRecDataFromMaster(CW_I2C2,u8Recdata,&u32RecvLen);//������д����
        I2C_SlaveSendDataToMaster(CW_I2C2,u8Recdata,&u32SendLen);//������������
    #endif 		
	}  
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    CW_SYSCTRL->AHBEN_f.GPIOB  = 1;
    #if(0X01 == TESTI2C)
        CW_SYSCTRL->APBEN1_f.I2C1 = 1U;    //
    #elif(0X02 == TESTI2C)
        CW_SYSCTRL->APBEN1_f.I2C2 = 1U;    //
    #endif
}

/**
  * @brief  Configure the GPIO Pins.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
    #if(0X01 == TESTI2C)
        PB10_AFx_I2C1SCL();
        PB11_AFx_I2C1SDA();
		GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
    #elif(0X02 == TESTI2C)
        PB00_AFx_I2C2SCL();
        PB01_AFx_I2C2SDA();
		GPIO_InitStructure.Pins = I2C2_SCL_GPIO_PIN | I2C2_SDA_GPIO_PIN;
		GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
		GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
		GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStructure);
    #endif
}



/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


