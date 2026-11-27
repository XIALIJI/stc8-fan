#include "cs_stc8h_uart.h"

void UART1_Init(void)
{
    SCON = 0x50;                 /* mode 1, 8N1, REN=1            */
    T2L  = 0x8F;                 /* 9600 @ 24MHz: 24M/4/9600=625  */
    T2H  = 0xFD;                 /* 65536-625 = 64911 = 0xFD8F    */
    AUXR = 0x15;                 /* T2R=1, T2x12=1, S1ST2=1       */
    GPIO_SET_MODE(P3M1, P3M0, 0x02, GPIO_PP);   /* P3.1 TX push-pull */
}

void UART1_SendByte(u8 dat)
{
    SBUF = dat;
    while (TI == 0) { }
    TI = 0;
}

void UART1_SendString(char *s)
{
    while (*s != 0)
    {
        UART1_SendByte((u8)(*s));
        s++;
    }
}

static u8 uart_hexchar(u8 v)
{
    return (u8)((v < 10) ? ('0' + v) : ('A' + v - 10));
}

void UART1_SendHex(u8 v)
{
    UART1_SendByte(uart_hexchar((u8)(v >> 4)));
    UART1_SendByte(uart_hexchar((u8)(v & 0x0F)));
}
