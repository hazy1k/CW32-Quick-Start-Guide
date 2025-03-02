

/******************************************************************************
* Include files                                                              *
******************************************************************************/
#include "base_types.h"
#include "cw32f030.h"
#include "system_cw32f030.h"
#include "cw32f030_rcc.h"


/******************************************************************************
 ** System Clock Frequency (Core Clock) Variable according CMSIS
 ******************************************************************************/

uint32_t SystemCoreClock = 8000000;

void SystemCoreClockUpdate(void) // Update SystemCoreClock variable
{
    SystemCoreClock = 8000000;
}

/**
 ******************************************************************************
 ** \brief  Setup the microcontroller system. Initialize the System and update
 ** the SystemCoreClock variable.
 **
 ** \param  none
 ** \return none
 ******************************************************************************/
void SystemInit(void)
{
    //Load TrimCode in this.
    //...

    SystemCoreClockUpdate();
    CW_SYSCTRL->HSI_f.TRIM = *((volatile uint16_t *)RCC_HSI_TRIMCODEADDR);
    CW_SYSCTRL->LSI_f.TRIM = *((volatile uint16_t *)RCC_LSI_TRIMCODEADDR);
    //Init Hide thing.
    //...
}

#if 0
#if defined (__CC_ARM)
extern int32_t $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    SystemInit();
    $Super$$main();
    return 0;
}
#elif defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
extern int32_t $Super$$main(void);
/* re-define main function */
int $Sub$$main(void)
{
    SystemInit();
    $Super$$main();
    return 0;
}
#elif defined(__ICCARM__)
extern int32_t main(void);
/* __low_level_init will auto called by IAR cstartup */
extern void __iar_data_init3(void);
int __low_level_init(void)
{
    // call IAR table copy function.
    __iar_data_init3();
    SystemInit();
    main();
    return 0;
}
#endif

#endif


/******************************************************************************
* Internal Funcation                                                          *
******************************************************************************/

///**
// * \brief   delay1ms
// *          delay approximately 1ms.
// * \param   [in]  u32Cnt
// * \retval  void
// */
void delay1ms(uint32_t u32Cnt)
{
    uint32_t u32end;
    
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
    
    while(u32Cnt-- > 0)
    {
        SysTick->VAL  = 0;
        u32end = 0x1000000 - SystemCoreClock/1000;
        while(SysTick->VAL > u32end)
        {
            ;
        }
    }
    
    SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));
}


///**
// * \brief   delay100us
// *          delay approximately 100us.
// * \param   [in]  u32Cnt
// * \retval  void
// */
//void delay100us(uint32_t u32Cnt)
//{
//    uint32_t u32end;
//    
//    SysTick->LOAD = 0xFFFFFF;
//    SysTick->VAL  = 0;
//    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
//    
//    while(u32Cnt-- > 0)
//    {
//        SysTick->VAL = 0;

//        u32end = 0x1000000 - SystemCoreClock/10000;
//        while(SysTick->VAL > u32end)
//        {
//            ;
//        }
//    }
//    
//    SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));
//}


///**
// * \brief   delay10us
// *          delay approximately 10us.
// * \param   [in]  u32Cnt
// * \retval  void
// */
void delay10us(uint32_t u32Cnt)
{
    uint32_t u32end;
    
    SysTick->LOAD = 0xFFFFFF;
    SysTick->VAL  = 0;
    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_CLKSOURCE_Msk;
    
    while(u32Cnt-- > 0)
    {
        SysTick->VAL = 0;

        u32end = 0x1000000 - SystemCoreClock/100000;
        while(SysTick->VAL > u32end)
        {
            ;
        }
    }
    
    SysTick->CTRL = (SysTick->CTRL & (~SysTick_CTRL_ENABLE_Msk));
}


void FirmwareDelay(uint32_t DlyCnt )
{
	volatile uint32_t thisCnt = DlyCnt;
	while( thisCnt-- )
	{
		;
	}
}

/**
 * \brief   clear memory
 *          
 * \param   [in]  start addr
 * \param   [in]  memory size(byte)
 * \retval  void
 */
void MemClr(void *pu8Address, uint32_t u32Count)
{
    uint8_t *pu8Addr = (uint8_t *)pu8Address;
    
    if(NULL == pu8Addr)
    {
        return;
    }
    
    while (u32Count--)
    {
        *pu8Addr++ = 0;
    }
}

