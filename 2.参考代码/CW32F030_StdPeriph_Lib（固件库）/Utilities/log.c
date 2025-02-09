
/******************************************************************************
 * Include files
 ******************************************************************************/
#include "log.h"
#include "cw32f030_gpio.h"
#include "cw32f030_rcc.h"
#include "cw32f030_uart.h"


/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/
/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/
static void SerialInit(uint32_t BaudRate);
static void SerialSend(uint8_t Data);

/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
static uint8_t const pow2_table[] = {0, 1, 2, 3, 4, 5, 6, 7};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/

void LogInit(void)
{
    SerialInit(LOG_SERIAL_BPS);
}

static void SerialInit(uint32_t BaudRate)
{
    uint32_t PCLK_Freq;
    GPIO_InitTypeDef GPIO_InitStructure = {0};
    USART_InitTypeDef USART_InitStructure = {0};

    PCLK_Freq = SystemCoreClock >> pow2_table[CW_SYSCTRL->CR0_f.HCLKPRS];
    PCLK_Freq >>= pow2_table[CW_SYSCTRL->CR0_f.PCLKPRS];

#ifdef DEBUG_USING_UART1
    //时钟使能
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_UART1_CLK_ENABLE();

    // 默认UART1.TX 映射到PA08 UART1.RX 映射到PA09，
    // 如需分配其他引脚请自行修改
    // 先设置UART TX RX 复用，后设置GPIO的属性，避免口线上出现毛刺
    PA08_AFx_UART1TXD();
    PA09_AFx_UART1RXD();
    GPIO_InitStructure.Pins = GPIO_PIN_8;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_9;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
#elif defined DEBUG_USING_UART2
    __RCC_UART2_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    // 默认UART2.TX 映射到PA06 UART2.RX 映射到PA07，
    // 如需分配其他引脚请自行修改
    PA06_AFx_UART2TXD();
    PA07_AFx_UART2RXD();

    GPIO_InitStructure.Pins = GPIO_PIN_6;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_7;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

#elif defined DEBUG_USING_UART3
    __RCC_UART3_CLK_ENABLE();
    __RCC_GPIOA_CLK_ENABLE();

    // 默认UART3.TX 映射到PA2 UART3.RX 映射到PA3
    // 如需分配其他引脚请自行修改
    PA02_AFx_UART3TXD();
    PA03_AFx_UART3RXD();

    GPIO_InitStructure.Pins = GPIO_PIN_2;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.Pins = GPIO_PIN_3;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);
#endif

    USART_InitStructure.USART_BaudRate = BaudRate;
    USART_InitStructure.USART_Over = USART_Over_16;
    USART_InitStructure.USART_Source = USART_Source_PCLK;
    USART_InitStructure.USART_UclkFreq = PCLK_Freq;
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

#ifdef DEBUG_USING_UART1
    USART_Init(CW_UART1, &USART_InitStructure);
#elif defined DEBUG_USING_UART2
    USART_Init(CW_UART2, &USART_InitStructure);
#elif defined DEBUG_USING_UART3
    USART_Init(CW_UART3, &USART_InitStructure);
#endif   
}

static void SerialSend(uint8_t Data)
{
#ifdef DEBUG_USING_UART1
    USART_SendData_8bit(CW_UART1, Data);
    while (USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE) == RESET);
#elif defined DEBUG_USING_UART2
    USART_SendData_8bit(CW_UART2, Data);
    while (USART_GetFlagStatus(CW_UART2, USART_FLAG_TXE) == RESET);
#elif defined DEBUG_USING_UART3
    USART_SendData_8bit(CW_UART3, Data);
    while (USART_GetFlagStatus(CW_UART3, USART_FLAG_TXE) == RESET);
#endif
}

int fputc(int ch, FILE *f)
{
    SerialSend(ch);
    return 0;
}

/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
