#include "task.h"
#include "cs_stc8h_uart.h"
#include "cs_stc8h_i2c_soft.h"
#include "stc8h_delay.h"

/* Diagnostic v2: GPIO config dump + line control test + full I2C scan,
 * everything reported on UART1 9600 so the PC can read it over COM7. */

static void diag_task(void)
{
    u8 addr, found, idle, driven;
    u8 buf[5];

    UART1_SendString("\r\n=== STC8 DIAG v2 ===\r\n");
    P_SW2 |= 0x80;
    UART1_SendString("P1M1=0x"); UART1_SendHex(P1M1);
    UART1_SendString(" P1M0=0x"); UART1_SendHex(P1M0);
    UART1_SendString(" P1PU=0x"); UART1_SendHex(P1PU);
    UART1_SendString("\r\n");

    /* can we actually control the two lines? */
    P14 = 1; P15 = 1; Delay_ms(2);
    idle = (u8)((P14 ? 1 : 0) | ((P15 ? 1 : 0) << 1));
    P14 = 0; Delay_ms(2);
    driven = (u8)((P14 ? 1 : 0) | ((P15 ? 1 : 0) << 1));
    P14 = 1; P15 = 1; Delay_ms(2);

    UART1_SendString("idle(SDA,SCL)=0x"); UART1_SendHex(idle);
    UART1_SendString(" driven SDA=0 ->0x"); UART1_SendHex(driven);
    UART1_SendString("  (expect idle=0x03, driven=0x02)\r\n");

    UART1_SendString("I2C scan 0x00-0x7F:\r\n");
    found = 0;
    for (addr = 0x00; addr < 0x80; addr++)
    {
        u8 ack;
        cs_i2c_start();
        ack = cs_i2c_send_byte((u8)(addr << 1));
        cs_i2c_stop();
        if (ack == 0)
        {
            UART1_SendString("  ACK 0x");
            UART1_SendHex(addr);
            UART1_SendString("\r\n");
            found++;
        }
    }
    UART1_SendString("  total=");
    UART1_SendByte((u8)('0' + found / 10));
    UART1_SendByte((u8)('0' + found % 10));
    UART1_SendString("\r\n");

    /* TM1650: control on + all segments */
    {
        u8 ctrl = 0x4F;
        UART1_SendString("ctrl 0x4F -> ");
        UART1_SendString((cs_i2c_write(0x48, &ctrl, 1) == 0) ? "ACK" : "NACK");
        UART1_SendString("\r\n");
    }
    buf[0] = 0x68;
    buf[1] = 0x7F; buf[2] = 0x7F; buf[3] = 0x7F; buf[4] = 0x7F;
    UART1_SendString("8888 -> ");
    UART1_SendString((cs_i2c_write(0x48, buf, 5) == 0) ? "ACK" : "NACK");
    UART1_SendString("\r\n");
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(3000, diag_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    UART1_Init();
    cs_i2c_soft_init();
    Delay_ms(100);
}
