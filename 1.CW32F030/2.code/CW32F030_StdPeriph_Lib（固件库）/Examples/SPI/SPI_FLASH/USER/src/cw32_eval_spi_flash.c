/**
 * @file cw32_eval_spi_flash.c
 * @author P&S (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-26
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

/* Includes ------------------------------------------------------------------*/
#include "cw32_eval_spi_flash.h"

/* Private_TypesDefinitions --------------------------------------------------*/

/* Private_Defines -----------------------------------------------------------*/

/* Private_Variables ---------------------------------------------------------*/

/* Private_FunctionPrototypes ------------------------------------------------*/

/* Private_Functions ---------------------------------------------------------*/

/* Public_Functions ----------------------------------------------------------*/  

/**
 * @brief ����SPI FLASH
 * 
 */
void SPI_FLASH_Init(void)
{
  SPI_InitTypeDef  SPI_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
	
 /************************GPIO Configuration***********************/
  RCC_AHBPeriphClk_Enable(FLASH_SPI_SCK_GPIO_CLK | FLASH_SPI_MISO_GPIO_CLK | FLASH_SPI_MOSI_GPIO_CLK | FLASH_SPI_CS_GPIO_CLK, ENABLE);
  FLASH_SPI_APBClkENx(FLASH_SPI_CLK, ENABLE);

  //SPI SCK MOSI MISO ����
  FLASH_SPI_AF_SCK;
  FLASH_SPI_AF_MISO;  
  FLASH_SPI_AF_MOSI;     

  //CS
  GPIO_InitStructure.Pins = FLASH_SPI_CS_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
  GPIO_Init(FLASH_SPI_CS_GPIO_PORT, &GPIO_InitStructure);

  //SCK   
  GPIO_InitStructure.Pins = FLASH_SPI_SCK_GPIO_PIN;
  GPIO_Init(FLASH_SPI_SCK_GPIO_PORT, &GPIO_InitStructure);

  //MOSI
  GPIO_InitStructure.Pins = FLASH_SPI_MOSI_GPIO_PIN;
  GPIO_Init(FLASH_SPI_MOSI_GPIO_PORT, &GPIO_InitStructure);

  //MISO
  GPIO_InitStructure.Pins = FLASH_SPI_MISO_GPIO_PIN;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
  GPIO_Init(FLASH_SPI_MISO_GPIO_PORT, &GPIO_InitStructure);

  //����CS
  FLASH_SPI_CS_HIGH();

  /************************SPI Configuration***********************/
  SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;    // ˫��ȫ˫��
  SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                         // ����ģʽ
  SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                     // ֡���ݳ���Ϊ8bit
  SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                           // ʱ�ӿ��е�ƽΪ��
  SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                          // �ڶ������ز���
  SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                             // Ƭѡ�ź���SSI�Ĵ�������
  SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;    // ������ΪPCLK��8��Ƶ
  SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                    // �����Чλ MSB �շ���ǰ
  SPI_InitStructure.SPI_Speed = SPI_Speed_Low;                          // ����SPI
  
  SPI_Init(FLASH_SPIx, &SPI_InitStructure);
  SPI_Cmd(FLASH_SPIx, ENABLE); 
}

/**
 * @brief ͨ��SPI��ȡһ���ֽڵ�����
 * 
 * @return uint8_t :���յ�������
 */
uint8_t SPI_FLASH_ReadByte(void)
{
  return (SPI_FLASH_SendByte(FLASH_DUMMY_BYTE));
}

/**
 * @brief ͨ��SPI����һ���ֽڵ����ݣ������ؽ��յ���һ���ֽ�����
 * 
 * @param byte :�����͵�����
 * @return uint8_t :���յ�������
 */
uint8_t SPI_FLASH_SendByte(uint8_t byte)
{
  while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_TXE) == RESET);

  SPI_SendData(FLASH_SPIx, byte);

  while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_RXNE) == RESET);

  return SPI_ReceiveData(FLASH_SPIx);
}

/**
 * @brief ͨ��SPI���������ֽڵ����ݣ������ؽ��յ��������ֽ�����
 * 
 * @param HalfWord :�����͵�����
 * @return uint16_t :���յ�������
 */
uint16_t SPI_FLASH_SendHalfWord(uint16_t HalfWord)
{
  while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_TXE) == RESET);

  SPI_SendData(FLASH_SPIx, HalfWord);

  while(SPI_GetFlagStatus(FLASH_SPIx, SPI_FLAG_RXNE) == RESET);
	
  return SPI_ReceiveData(FLASH_SPIx);
}

/**
 * @brief ��FLASH���� дʹ�� ָ��
 * 
 */
void SPI_FLASH_WriteEnable(void)
{
  FLASH_SPI_CS_LOW();

  //���� дʹ�� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_WriteEnable);

  FLASH_SPI_CS_HIGH();
}

 /**
  * @brief �ȴ������������
  * 
  */
void SPI_FLASH_WaitForWriteEnd(void)
{
  uint8_t flashstatus = 0;

  FLASH_SPI_CS_LOW();

  // ���� ��״̬�Ĵ���1 ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg1);

  do
  {
    //��FLASH״̬�Ĵ���
    flashstatus = SPI_FLASH_ReadByte();	 
  }while((flashstatus & FLASH_WIP_FLAG) == SET);  //����д�룬����ѭ��

  FLASH_SPI_CS_HIGH();
}

/**
 * @brief ��״̬�Ĵ���1
 * 
 * @return uint8_t 
 */
uint8_t SPI_FLASH_ReadStatusReg1(void)
{
  uint8_t flashstatus = 0;

  FLASH_SPI_CS_LOW();

  // ���� ��״̬�Ĵ���1 ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg1);

  flashstatus = SPI_FLASH_ReadByte();	

  FLASH_SPI_CS_HIGH(); 

  return flashstatus;
}

/**
 * @brief ��״̬�Ĵ���2
 * 
 * @return uint8_t 
 */
uint8_t SPI_FLASH_ReadStatusReg2(void)
{
  uint8_t flashstatus = 0;

  FLASH_SPI_CS_LOW();

  // ���� ��״̬�Ĵ���2 ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReadStatusReg2);

  flashstatus = SPI_FLASH_ReadByte();	

  FLASH_SPI_CS_HIGH(); 

  return flashstatus;
}

/**
 * @brief д״̬�Ĵ���1
 * 
 * @param status 
 */
void SPI_FLASH_WriteStatusReg1(uint8_t status)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();

  FLASH_SPI_CS_LOW();

  // ���� д״̬�Ĵ���1 ָ��
  SPI_FLASH_SendByte(FLASH_CMD_WriteStatusReg1);

  //���� ״̬�Ĵ���1 �趨ֵ
  SPI_FLASH_SendByte(status);

  FLASH_SPI_CS_HIGH(); 

  //�ȴ�д�����
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief д״̬�Ĵ���2
 * 
 * @param status 
 */
void SPI_FLASH_WriteStatusReg2(uint8_t status)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();

  FLASH_SPI_CS_LOW();

  // ���� д״̬�Ĵ���2 ָ��
  SPI_FLASH_SendByte(FLASH_CMD_WriteStatusReg2);

  //���� ״̬�Ĵ���2 �趨ֵ
  SPI_FLASH_SendByte(status);

  FLASH_SPI_CS_HIGH(); 

  //�ȴ�д�����
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief �������� 4KB
 * 
 * @param SectorAddr :��������������ַ
 */
void SPI_FLASH_SectorErase(uint32_t SectorAddr)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();
  //�ȴ�д�����
  // SPI_FLASH_WaitForWriteEnd();

  FLASH_SPI_CS_LOW();

  //���� �������� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_SectorErase);

  //���� ������������ַ
  SPI_FLASH_SendByte((SectorAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((SectorAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(SectorAddr & 0xFF);

  FLASH_SPI_CS_HIGH();

  //�ȴ��������
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief ����� 64KB
 * 
 * @param BlockAddr :�������Ŀ��ַ
 */
void SPI_FLASH_BlockErase(uint32_t BlockAddr)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();
  //�ȴ�д�����
  // SPI_FLASH_WaitForWriteEnd();

  FLASH_SPI_CS_LOW();

  //���� ����� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_BlockErase);

  //���� ���������ַ
  SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(BlockAddr & 0xFF);

  FLASH_SPI_CS_HIGH();

  //�ȴ��������
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief ��Ƭ����
 * 
 */
void SPI_FLASH_ChipErase(void)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();

  FLASH_SPI_CS_LOW();

  //���� ��Ƭ���� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ChipErase);

  FLASH_SPI_CS_HIGH();

  //�ȴ��������
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief ҳд������
 * 
 * @param pBuffer :��д�����ݵ�ָ��
 * @param WriteAddr :д���ַ
 * @param NumByteToWrite :д�����ݳ���
 * @note
 *    -��Ҫ�Ȳ���
 *    -NumByteToWrite <= SPI_FLASH_PerWritePageSize
 */
void SPI_FLASH_PageWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  //���� дʹ�� ָ��
  SPI_FLASH_WriteEnable();

  FLASH_SPI_CS_LOW();

  //���� ҳд�� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_PageProgram);

  //���� д��ַ
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  SPI_FLASH_SendByte(WriteAddr & 0xFF);

  //д������
  while(NumByteToWrite--)
  {
    SPI_FLASH_SendByte(*pBuffer);
    pBuffer++;
  }

  FLASH_SPI_CS_HIGH();

  //�ȴ�д�����
  SPI_FLASH_WaitForWriteEnd();
}

/**
 * @brief д�벻��������
 * 
 * @param pBuffer :��д�����ݵ�ָ��
 * @param WriteAddr :д���ַ
 * @param NumByteToWrite :д�����ݳ���
 * @note
 *    -��Ҫ�Ȳ���
 */
void SPI_FLASH_BufferWrite(uint8_t* pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
  uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;
	
  Addr = WriteAddr % SPI_FLASH_PageSize;
  count = SPI_FLASH_PageSize - Addr;
  NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
  NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
	
  if(Addr == 0) /* WriteAddr �պð�ҳ���� */
  {
    if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */ 
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite >= SPI_FLASH_PageSize */
    { 
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if(NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
  else /* WriteAddr �� SPI_FLASH_PageSize ������  */
  {
    if(NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
    {
      if(NumOfSingle > count) /*!< (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
      {
        temp = NumOfSingle - count;
        
        //д�굱ǰҳ
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        //дʣ������
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite >= SPI_FLASH_PageSize */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / SPI_FLASH_PageSize;
      NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;
			
      //��д�굱ǰҳ���Ժ��ַ������
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      //WriteAddr �պð�ҳ����
      while(NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, SPI_FLASH_PageSize);
        WriteAddr +=  SPI_FLASH_PageSize;
        pBuffer += SPI_FLASH_PageSize;
      }

      if(NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}

/**
 * @brief ������
 * 
 * @param pBuffer :�洢�������ݵ�ָ��
 * @param ReadAddr :��ȡ��ַ
 * @param NumByteToRead :��ȡ���ݳ���
 */
void SPI_FLASH_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
  FLASH_SPI_CS_LOW();

  //���� ������ ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReadData);

  //���� ����ַ
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
	
  //������
  while(NumByteToRead--)
  {
    *pBuffer = SPI_FLASH_ReadByte();
    pBuffer++;
  }

  FLASH_SPI_CS_HIGH();
}

/**
 * @brief ������ȡ�����ֽ�����
 * 
 * @param ReadAddr :��ȡ��ַ
 */
void SPI_FLASH_StartReadSequence(uint32_t ReadAddr)
{
  FLASH_SPI_CS_LOW();

  //���� ������ ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReadData);

  //���� ����ַ
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  SPI_FLASH_SendByte(ReadAddr & 0xFF);
}

/**
 * @brief ��ȡDevice ID
 * 
 * @return uint8_t :Device ID
 */
uint8_t SPI_FLASH_DeviceID(void)
{
  uint8_t Temp = 0;

  FLASH_SPI_CS_LOW();

  //���� ��Device ID ָ��
  SPI_FLASH_SendByte(FLASH_CMD_DeviceID);

  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  
  //������
  Temp = SPI_FLASH_ReadByte();

  FLASH_SPI_CS_HIGH();

  return Temp;
}

/**
 * @brief ��ȡManufacturer / Device ID
 * 
 * @return uint16_t :Manufacturer / Device ID
 */
uint16_t SPI_FLASH_ManufactDeviceID(void)
{
  uint16_t Temp = 0, Temp0 = 0, Temp1 = 0;

  FLASH_SPI_CS_LOW();

  //���� ��Manufacturer / Device ID ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ManufactDeviceID);

  SPI_FLASH_SendByte(0x00);
  SPI_FLASH_SendByte(0x00);
  SPI_FLASH_SendByte(0x00);

  //������
  Temp0 = SPI_FLASH_ReadByte();
  Temp1 = SPI_FLASH_ReadByte();

  FLASH_SPI_CS_HIGH();

  Temp = (Temp0 << 8) | Temp1;

  return Temp;
}

/**
 * @brief ��ȡJEDEC ID
 * 
 * @return uint32_t :JEDEC ID
 */
uint32_t SPI_FLASH_JedecID(void)
{
  uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;

  FLASH_SPI_CS_LOW();

  //���� ��JEDEC ID ָ��
  SPI_FLASH_SendByte(FLASH_CMD_JedecID);

  //������
  Temp0 = SPI_FLASH_ReadByte();
  Temp1 = SPI_FLASH_ReadByte();
  Temp2 = SPI_FLASH_ReadByte();

  FLASH_SPI_CS_HIGH();

  Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;

  return Temp;
}

/**
 * @brief ��ȡUnique ID (64bit)
 * 
 * @param pBuffer :���UID��Buffer
 */
void SPI_FLASH_UniqueID(uint8_t* pBuffer)
{
  uint8_t NumByteToRead = 8;

  FLASH_SPI_CS_LOW();

  //���� ��Unique ID ָ��
  SPI_FLASH_SendByte(FLASH_CMD_UniqueID);

  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);
  SPI_FLASH_SendByte(FLASH_DUMMY_BYTE);

  //������
  while(NumByteToRead--)
  {
    *pBuffer = SPI_FLASH_ReadByte();
    pBuffer++;
  }

  FLASH_SPI_CS_HIGH();
}

/**
 * @brief �������ģʽ
 * 
 */
void SPI_FLASH_PowerDown(void)   
{ 
  FLASH_SPI_CS_LOW();

  //���� ���� ָ��
  SPI_FLASH_SendByte(FLASH_CMD_PowerDown);

  FLASH_SPI_CS_HIGH();
}   

/**
 * @brief ����
 * 
 */
void SPI_FLASH_WAKEUP(void)   
{
  FLASH_SPI_CS_LOW();

  //���� Release Power-down or High Performance Mode ָ��
  SPI_FLASH_SendByte(FLASH_CMD_ReleasePowerDown);

  FLASH_SPI_CS_HIGH();
}   







