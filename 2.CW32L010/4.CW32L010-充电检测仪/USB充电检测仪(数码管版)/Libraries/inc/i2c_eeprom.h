/**
 * @file i2c_eeprom.h
 * @author WHXY
 * @brief
 * @version 0.1
 * @date 2024-08-07
 *
 * @copyright Copyright (c) 2021
 * x
 */

/*******************************************************************************
*
* 代码许可和免责信息
* 武汉芯源半导体有限公司授予您使用所有编程代码示例的非专属的版权许可，您可以由此
* 生成根据您的特定需要而定制的相似功能。根据不能被排除的任何法定保证，武汉芯源半
* 导体有限公司及其程序开发商和供应商对程序或技术支持（如果有）不提供任何明示或暗
* 含的保证或条件，包括但不限于暗含的有关适销性、适用于某种特定用途和非侵权的保证
* 或条件。
* 无论何种情形，武汉芯源半导体有限公司及其程序开发商或供应商均不对下列各项负责，
* 即使被告知其发生的可能性时，也是如此：数据的丢失或损坏；直接的、特别的、附带的
* 或间接的损害，或任何后果性经济损害；或利润、业务、收入、商誉或预期可节省金额的
* 损失。
* 某些司法辖区不允许对直接的、附带的或后果性的损害有任何的排除或限制，因此某些或
* 全部上述排除或限制可能并不适用于您。
*
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "cw32l010_i2c.h"

/* Private_TypesDefinitions --------------------------------------------------*/
typedef enum {
    EE_STATE_IDLE = 0,
    EE_STATE_WRITE,
    EE_STATE_READ,
    EE_STATE_FAIL
}EEPROM_StateTypeDef;

typedef struct {
    uint8_t* EE_SendBuffPtr;
    uint8_t* EE_RecvBuffPtr;
    uint16_t EE_DataAddr;    
    uint16_t EE_SendLen;
    uint16_t EE_RecvLen;
    uint8_t  EE_DeviceAddr;    
    EEPROM_StateTypeDef  EE_State;       
}EEPROM_TypeDef;

/* Private_Defines -----------------------------------------------------------*/
//根据EEPROM的型号，确定页大小
#define EEPROM_PAGE_SIZE        8        // 24C02/04/08/16/32/64    每页8/16/16/16/32/32个字节

//根据EEPROM的型号，确定数据地址是8bit还是16bit
#define EEPROM_DATA_ADDR_WIDTH  1


extern EEPROM_TypeDef gEEPROMInstance;

void I2C_eepromWaitReady(I2C_TypeDef* I2Cx,  uint8_t SlaveAddr);
I2C_ErrorDef I2C_eepromWriteBytes(I2C_TypeDef* I2Cx,  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length);
I2C_ErrorDef I2C_eepromReadBytes(I2C_TypeDef* I2Cx,  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length);
I2C_ErrorDef I2C_eepromWriteBytesInt(I2C_TypeDef* I2Cx,  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length);
I2C_ErrorDef I2C_eepromReadBytesInt(I2C_TypeDef* I2Cx,  uint8_t SlaveAddr, uint16_t DataAddr, uint8_t *buffer, uint16_t Length);
void I2C_eepromHandleIRQ(I2C_TypeDef* I2Cx);

#ifdef __cplusplus
}
#endif

#endif /*__CW32L011_I2C_H */
