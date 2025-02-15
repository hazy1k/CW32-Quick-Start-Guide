#include "uart.h"

void uart1_init(void)
{
	/* 时钟配置 */
	RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_GPIOA, ENABLE);  	
	RCC_APBPeriphClk_Enable2(RCC_APB2_PERIPH_UART1, ENABLE);	
    /* GPIO配置 */
	GPIO_InitTypeDef GPIO_InitStructure;
    PA08_AFx_UART1TXD();
    PA09_AFx_UART1RXD();
    GPIO_InitStructure.Pins = GPIO_PIN_8;						
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;				
    GPIO_InitStructure.Speed = GPIO_SPEED_HIGH;					
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);					
    GPIO_InitStructure.Pins = GPIO_PIN_9;						
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;			
    GPIO_Init(CW_GPIOA, &GPIO_InitStructure);				
    /* USART配置 */
	USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 115200U;			
    USART_InitStructure.USART_Over = USART_Over_16;				
    USART_InitStructure.USART_Source = USART_Source_PCLK;	
    USART_InitStructure.USART_UclkFreq = 64000000;  			
    USART_InitStructure.USART_StartBit = USART_StartBit_FE;	 
    USART_InitStructure.USART_StopBits = USART_StopBits_1;	
    USART_InitStructure.USART_Parity = USART_Parity_No ;	
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	
    USART_Init(CW_UART1, &USART_InitStructure);						
	/* NVIC配置 */
    NVIC_SetPriority(UART1_IRQn, 0);
    NVIC_EnableIRQ(UART1_IRQn);
    USART_ITConfig(CW_UART1, USART_IT_RC, ENABLE);
}

#if !defined(__MICROLIB)

#if (__ARMCLIB_VERSION <= 6000000)

struct __FILE
{
	int handle;
};
#endif

FILE __stdout;

void _sys_exit(int x)
{
	x = x;
}
#endif

void usart_send_data(uint8_t ucch)
{
    USART_SendData_8bit(CW_UART1, (uint8_t)ucch);
	while(RESET == USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE));
}

void usart_send_String(uint8_t *ucstr)
{   
      while(ucstr && *ucstr)
      {     
        usart_send_data(*ucstr++);    
      }
}
int fputc(int ch, FILE *f)
{
  USART_SendData_8bit(CW_UART1, (uint8_t)ch);
	while(RESET == USART_GetFlagStatus(CW_UART1, USART_FLAG_TXE))
	{
	}
  return ch;
}

void UART1_IRQHandler(void)
{
    uint8_t Buffer;
    if(USART_GetITStatus(CW_UART1, USART_IT_RC) != RESET)
    {
        Buffer = USART_ReceiveData_8bit(CW_UART1);
        USART_SendData_8bit(CW_UART1, Buffer); 
        USART_ClearITPendingBit(CW_UART1, USART_IT_RC);
    }
}
