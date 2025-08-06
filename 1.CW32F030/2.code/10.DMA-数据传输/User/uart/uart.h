#ifndef __UART_H
#define __UART_H

#include "main.h"
#include "stdio.h"

void uart1_init(void);
void usart_send_data(uint8_t ucch);
void usart_send_String(uint8_t *ucstr);
int fputc(int ch, FILE *f);

#endif /* __UART_H */
