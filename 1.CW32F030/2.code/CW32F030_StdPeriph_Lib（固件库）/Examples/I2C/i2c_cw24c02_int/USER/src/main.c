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
#define  I2C1_SCL_GPIO_PIN        GPIO_PIN_10    //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�
#define  I2C1_SDA_GPIO_PORT       CW_GPIOB   
#define  I2C1_SDA_GPIO_PIN        GPIO_PIN_11    //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�

#define  I2C2_SCL_GPIO_PORT       CW_GPIOB
#define  I2C2_SCL_GPIO_PIN        GPIO_PIN_0     //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�
#define  I2C2_SDA_GPIO_PORT       CW_GPIOB   
#define  I2C2_SDA_GPIO_PIN        GPIO_PIN_1     //����Ķ�������GPIO��ʼ��������Ҫ��ͬ���޸�

//EEPROM�ڲ���ַ
uint8_t u8Addr = 0x00;        //��ַ�ֽ�
#define WRITELEN   8          //д���ݳ���
#define READLEN   8           //�����ݳ���
#define WriteReadCycle  35    //д������,ÿ��д������Ϊn+i(nΪ������i=0~7)

uint8_t u8Senddata[8] = {0x66,0x02,0x03,0x04,0x05,0x60,0x70,0x20};
uint8_t u8Senddata2[8] = {0x55,0xAA,0xAA,0x55,0x55,0xAA,0x55,0xAA};
uint8_t u8Recdata[16]={0x00};
uint8_t u8SendLen=0;
uint8_t u8RecvLen=0;
uint8_t SendFlg = 0,Comm_flg = 0;
uint8_t u8recvflg=0;
uint8_t u8State = 0;

uint8_t receivedflag = 0;    //��ȡ��ɱ�־


/**
 * @brief  I2C1 EEPROM��д�жϺ���
 * 
 */
void I2c1EepromReadWriteInterruptFunction(void)
{
    u8State = I2C_GetState(CW_I2C1);
    switch(u8State)
    {
        case 0x08:     //������START�ź�
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X00);
            break;
        case 0x10:     //�������ظ���ʼ�ź�
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            if(0 == SendFlg)
            {
                I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X00);    //д����
            }
            else
            {
                I2C_Send7bitAddress(CW_I2C1, I2C_SLAVEADDRESS,0X01);    //������,eeprom �����
            }
            break;
        case 0x18:    //������SLA+W/R�ֽ�
            I2C_GenerateSTART(CW_I2C1, DISABLE);
            I2C_SendData(CW_I2C1, u8Addr);   //���ͷ���EEPROM��Ŀ���ַ�ֽ�
            break;
        case 0x20:    //������SLA+W��ӻ�����NACK
        case 0x38:    //�����ڷ��� SLA+W �׶λ��߷������ݽ׶ζ�ʧ�ٲ�  ����  �����ڷ��� SLA+R �׶λ��߻�Ӧ NACK �׶ζ�ʧ�ٲ�
        case 0x30:    //������һ�������ֽں�ӻ�����NACK
        case 0x48:    //������SLA+R��ӻ�����NACK
            I2C_GenerateSTOP(CW_I2C1, ENABLE);
            I2C_GenerateSTART(CW_I2C1, ENABLE);          
        break;			
        case 0x58:    //���յ�һ�������ֽڣ���NACK�ѻظ�
						u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C1);//�������ݶ�ȡ��ɣ�NACK�ѷ���
						receivedflag =1;
            I2C_GenerateSTOP(CW_I2C1, ENABLE);//����ֹͣ����
            break;				
        case 0x28:     //������1�ֽ����ݣ�����EEPROM��memory��ַҲ����������ͺ��������Ҳ�����											
						if(0 == SendFlg)
            {
								if(u8SendLen <WRITELEN)
								{
									I2C_SendData(CW_I2C1,u8Senddata[u8SendLen++]);
								}
								else
								{
									u8SendLen = 0;
									Comm_flg = 1;
									SendFlg = 1;
									I2C_GenerateSTOP(CW_I2C1, ENABLE);//���������ݣ�����ֹͣ�ź�
								}
            }
            else       //SendFlg=1Ϊ����SendFlg=0Ϊд�������ݷ������ַ�ֽں��ظ���ʼ����
						{
							  CW_I2C1->CR_f.STA = 1;  //set start       //�����ظ�START�ź�,START���ɺ�����д�󣬻ᵼ��0X10״̬���Թ����ʴ˴������ú���
                I2C_GenerateSTOP(CW_I2C1, DISABLE); 
						}
            break;
						
        case 0x40:     //������SLA+R�źţ���ʼ��������
            u8RecvLen = 0;
						if(READLEN>1)
						{
							I2C_AcknowledgeConfig(CW_I2C1,ENABLE);//��ȡ���ݳ���1���ֽڲŷ���ACK
						}
            break;	
        case 0x50:     //������һ�ֽ����ݣ��ڽ������1�ֽ�����֮ǰ����AA=0;
						u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C1);
            if(u8RecvLen==READLEN-1)
            {
                I2C_AcknowledgeConfig(CW_I2C1,DISABLE);;
            }
            break;
    } 
    I2C_ClearIrq(CW_I2C1);
}

/**
 * @brief  I2C2 EEPROM��д�жϺ���
 * 
 */
void I2c2EepromReadWriteInterruptFunction(void)
{
    u8State = I2C_GetState(CW_I2C2);
    switch(u8State)
    {
        case 0x08:     //������START�ź�
            I2C_GenerateSTART(CW_I2C2, DISABLE);
            I2C_Send7bitAddress(CW_I2C2, I2C_SLAVEADDRESS,0X00);
            break;
        case 0x10:     //�������ظ���ʼ�ź�
            I2C_GenerateSTART(CW_I2C2, DISABLE);
            if(0 == SendFlg)
            {
                I2C_Send7bitAddress(CW_I2C2, I2C_SLAVEADDRESS,0X00);    //д����
            }
            else
            {
                I2C_Send7bitAddress(CW_I2C2, I2C_SLAVEADDRESS,0X01);    //������,eeprom �����
            }
            break;
        case 0x18:    //������SLA+W/R�ֽ�
            I2C_GenerateSTART(CW_I2C2, DISABLE);
            I2C_SendData(CW_I2C2, u8Addr);   //���ͷ���EEPROM��Ŀ���ַ�ֽ�
            break;
        case 0x20:    //������SLA+W��ӻ�����NACK
        case 0x38:    //�����ڷ��� SLA+W �׶λ��߷������ݽ׶ζ�ʧ�ٲ�  ����  �����ڷ��� SLA+R �׶λ��߻�Ӧ NACK �׶ζ�ʧ�ٲ�
        case 0x30:    //������һ�������ֽں�ӻ�����NACK
        case 0x48:    //������SLA+R��ӻ�����NACK
            I2C_GenerateSTOP(CW_I2C2, ENABLE);
            I2C_GenerateSTART(CW_I2C2, ENABLE);          
        break;			
        case 0x58:    //���յ�һ�������ֽڣ���NACK�ѻظ�
						u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C2);//�������ݶ�ȡ��ɣ�NACK�ѷ���
						receivedflag =1;
            I2C_GenerateSTOP(CW_I2C2, ENABLE);//����ֹͣ����
            break;				
        case 0x28:     //������1�ֽ����ݣ�����EEPROM��memory��ַҲ����������ͺ��������Ҳ�����											
						if(0 == SendFlg)    //SendFlg=0Ϊд
            {
								if(u8SendLen <WRITELEN)
								{
									I2C_SendData(CW_I2C2,u8Senddata[u8SendLen++]);
								}
								else
								{
									u8SendLen = 0;
									Comm_flg = 1;
									SendFlg = 1;
									I2C_GenerateSTOP(CW_I2C2, ENABLE);//���������ݣ�����ֹͣ�ź�
								}
            }
            else       //SendFlg=1Ϊ���������ݷ������ַ�ֽں��ظ���ʼ����
						{
							  CW_I2C2->CR_f.STA = 1;  //set start       //�����ظ�START�ź�,START���ɺ�����д�󣬻ᵼ��0X10״̬���Թ����ʴ˴������ú���
                I2C_GenerateSTOP(CW_I2C2, DISABLE); 
						}
            break;
						
        case 0x40:     //������SLA+R�źţ���ʼ��������
            u8RecvLen = 0;
						if(READLEN>1)
						{
							I2C_AcknowledgeConfig(CW_I2C2,ENABLE);//��ȡ���ݳ���1���ֽڲŷ���ACK
						}
            break;	
        case 0x50:     //������һ�ֽ����ݣ��ڽ������1�ֽ�����֮ǰ����AA=0;
						u8Recdata[u8RecvLen++] = I2C_ReceiveData(CW_I2C2);
            if(u8RecvLen==READLEN-1)
            {
                I2C_AcknowledgeConfig(CW_I2C2,DISABLE);;
            }
            break;
    } 
    I2C_ClearIrq(CW_I2C2);
}


int32_t main(void)
{
		I2C_InitTypeDef I2C_InitStruct;
	
		uint16_t tempcnt = 0 ;
	
	  //ʱ�ӳ�ʼ��
    RCC_Configuration();
 
    //IO�ڳ�ʼ��
    GPIO_Configuration();
    
    //I2C��ʼ��
		I2C_InitStruct.I2C_Baud = 0x01;//500K=(8000000/(8*(1+1))
    I2C_InitStruct.I2C_BaudEn = ENABLE;
    I2C_InitStruct.I2C_FLT = DISABLE;
    I2C_InitStruct.I2C_AA =  DISABLE;
    #if(0x01 == TESTI2C)
        I2C1_DeInit(); 
        I2C_Master_Init(CW_I2C1,&I2C_InitStruct);//��ʼ��ģ��
    #elif(0x02 == TESTI2C)
        I2C2_DeInit(); 
        I2C_Master_Init(CW_I2C2,&I2C_InitStruct);//��ʼ��ģ��
    #endif   
    
    //�ж�����
    NVIC_Configuration();

		//I2Cģ�鿪ʼ����
		#if(0x01 == TESTI2C)
				I2C_Cmd(CW_I2C1,ENABLE);  //ģ��ʹ��
		#elif(0x02 == TESTI2C)
				I2C_Cmd(CW_I2C2,ENABLE);  //ģ��ʹ��
		#endif  
		
		tempcnt =0;
		for(uint8_t i=0;i<8;i++)
		{
			u8Senddata[i] = i;
		}
						
		while(1)
		{
			#if(0x01 == TESTI2C)
				I2C_GenerateSTART(CW_I2C1, ENABLE); //��ʼ�ź�
			#elif(0x02 == TESTI2C)
				I2C_GenerateSTART(CW_I2C2, ENABLE); //��ʼ�ź�
			#endif
		
			while(1)
			{
					//�ȴ����ݷ������
				  while(!Comm_flg) ;        

				  FirmwareDelay(3000);
				
					//���������ݹ���
					Comm_flg = 0;
					receivedflag=0;
					#if(0x01 == TESTI2C)
						I2C_GenerateSTART(CW_I2C1, ENABLE); //��ʼ�ź�
					#elif(0x02 == TESTI2C)
						I2C_GenerateSTART(CW_I2C2, ENABLE); //��ʼ�ź�
					#endif

					//�ȴ����ݶ�ȡ���
				  while(!receivedflag) ; 
				
				  //��ʼ����һ��д����
					receivedflag = 0;
					SendFlg = 0;
					u8RecvLen = 0;
					tempcnt++;
					for(uint8_t i=0;i<8;i++)
					{
						u8Senddata[i] =tempcnt+i;
					}
					break;
			}
		  
			//���Դ�����ɣ��˳�
			if(tempcnt >=WriteReadCycle)
			{
				break;
			}	
						
		}
		
		while(1);
			
}

/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration(void)
{
    CW_SYSCTRL->AHBEN_f.GPIOB  = 1;
    #if(0x01 == TESTI2C)
        CW_SYSCTRL->APBEN1_f.I2C1 = 1U;    //
    #elif(0x02 == TESTI2C)
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
    #if(0x01 == TESTI2C)
			PB10_AFx_I2C1SCL();
			PB11_AFx_I2C1SDA();
			GPIO_InitStructure.Pins = I2C1_SCL_GPIO_PIN | I2C1_SDA_GPIO_PIN;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_Init(I2C1_SCL_GPIO_PORT, &GPIO_InitStructure);
    #elif(0x02 == TESTI2C)
			PB00_AFx_I2C2SCL();
			PB01_AFx_I2C2SDA();
			GPIO_InitStructure.Pins = I2C2_SCL_GPIO_PIN | I2C2_SDA_GPIO_PIN;
			GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_OD;
			GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
			GPIO_Init(I2C2_SCL_GPIO_PORT, &GPIO_InitStructure);
    #endif
}

/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
     __disable_irq(); 
    #if(0x01 == TESTI2C)
        NVIC_EnableIRQ(I2C1_IRQn); 
    #elif(0x02 == TESTI2C)
        NVIC_EnableIRQ(I2C2_IRQn); 
    #endif
    __enable_irq();  
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/


