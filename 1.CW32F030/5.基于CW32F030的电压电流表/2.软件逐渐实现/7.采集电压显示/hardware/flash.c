#include "flash.h"

#define PAGE_SIZE 512
#define PAGE_NUM 1
#define START_ADDR (39 * PAGE_SIZE)
#define END_ADDR (START_ADDR + PAGE_SIZE -1)

uint8_t FLASH_WirteHalfWords(uint32_t WriteAddr, uint16_t* pWrBuf, uint16_t WrByteCnt)
{
    uint16_t  CR1BAK, CR2BAK;

    if (WriteAddr + WrByteCnt > 65536)
    {
        return(FLASH_ERROR_ADDR);
    }

    if ((WriteAddr & 0x00000001) != 0)    // 地址未对齐
    {
        return(FLASH_ERROR_ADDR);
    }    

    while (CW_FLASH->CR1 & bv5);                          //等待上1次操作完成，未作超时处理
    CW_FLASH->ICR = 0x00;                                    //clear all int flag
    CR2BAK = CW_FLASH->CR2;
    CR1BAK = CW_FLASH->CR1;
    CW_FLASH->CR2 = 0x5A5A0000 | (CR2BAK & 0xffe7u);       //关掉FETCH和BUFFER功能
    CW_FLASH->CR1 = 0x5A5A0000 | (CR1BAK | 0x01u);        // 设置为页写模式
    while (WrByteCnt)
    {
        *((volatile uint16_t*)(WriteAddr)) = *pWrBuf;        //write byte
        while (CW_FLASH->CR1 & bv5);                        //等待操作完成
        WriteAddr += 2;
        WrByteCnt--;
        pWrBuf++;
    }
    CW_FLASH->CR1 = 0x5A5A0000 | CR1BAK;
    CW_FLASH->CR2 = 0x5A5A0000 | CR2BAK;
    return(CW_FLASH->ISR);

}

void flash_erase(void)
{
    uint8_t Flag;
    // erase
    FLASH_UnlockPages(START_ADDR, END_ADDR);       // 解锁最后一个页面
    Flag = FLASH_ErasePages(START_ADDR, END_ADDR); // 擦除最后一个页面
    FLASH_LockAllPages();
    if (Flag)
    {
        while (1)
            ; // 擦除失败
    }
}

void flash_write(uint16_t offset,uint16_t *data,uint16_t lenght)
{
    uint8_t Flag;
    // write
    FLASH_UnlockPages(START_ADDR, END_ADDR); // 解锁最后一个页面
    Flag = FLASH_WirteHalfWords(START_ADDR+offset*2, data, lenght);
    FLASH_LockAllPages();
    if (Flag)
    {
        while (1)
            ; // 写入失败
    }
}

void flash_read(uint16_t offset,uint16_t *data,uint16_t lenght)
{
    int i;
    // read
    for (i = 0; i < lenght; i++)
    {
        *data++ = *((volatile uint16_t *)(START_ADDR + offset*2 + i*2));
    }
}
