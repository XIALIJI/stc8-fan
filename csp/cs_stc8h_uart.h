#ifndef __CS_STC8H_UART_H__
#define __CS_STC8H_UART_H__

#include "cs_stc8h.h"

/* UART1 debug port: P3.1 TXD / P3.0 RXD, 9600 8N1 @ 24MHz 1T */

void UART1_Init(void);
void UART1_SendByte(u8 dat);
void UART1_SendString(char *s);
void UART1_SendHex(u8 v);

#endif
