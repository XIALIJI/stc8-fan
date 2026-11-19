#include "cs_stc8h_i2c_soft.h"
#include <intrins.h>

#define I2C_SCL_SET()   (P15 = 1)
#define I2C_SCL_CLR()   (P15 = 0)
#define I2C_SDA_SET()   (P14 = 1)
#define I2C_SDA_CLR()   (P14 = 0)
#define I2C_SDA_READ()  (P14)

static void i2c_delay(void)
{
    _nop_(); _nop_(); _nop_(); _nop_();
}

void cs_i2c_soft_init(void)
{
    GPIO_SET_MODE(P1M1, P1M0, 0x30, GPIO_QUASI);
    P_SW2 |= 0x80;
    P1PU |= 0x30;           /* weak pull-up on P1.4/P1.5 */
    I2C_SCL_SET();
    I2C_SDA_SET();
}

void cs_i2c_start(void)
{
    I2C_SDA_SET(); i2c_delay();
    I2C_SCL_SET(); i2c_delay();
    I2C_SDA_CLR(); i2c_delay();
    I2C_SCL_CLR(); i2c_delay();
}

void cs_i2c_stop(void)
{
    I2C_SDA_CLR(); i2c_delay();
    I2C_SCL_SET(); i2c_delay();
    I2C_SDA_SET(); i2c_delay();
}

void cs_i2c_send_byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80) I2C_SDA_SET(); else I2C_SDA_CLR();
        i2c_delay();
        I2C_SCL_SET(); i2c_delay();
        I2C_SCL_CLR(); i2c_delay();
        dat <<= 1;
    }
}

u8 cs_i2c_wait_ack(void)
{
    u8 ack;
    I2C_SDA_SET();
    i2c_delay();
    I2C_SCL_SET(); i2c_delay();
    ack = I2C_SDA_READ();
    I2C_SCL_CLR(); i2c_delay();
    return ack;
}

void cs_i2c_ack(void)
{
    I2C_SDA_CLR(); i2c_delay();
    I2C_SCL_SET(); i2c_delay();
    I2C_SCL_CLR(); i2c_delay();
    I2C_SDA_SET();
}

void cs_i2c_nack(void)
{
    I2C_SDA_SET(); i2c_delay();
    I2C_SCL_SET(); i2c_delay();
    I2C_SCL_CLR(); i2c_delay();
}

u8 cs_i2c_read_byte(u8 ack)
{
    u8 i, dat;
    dat = 0;
    I2C_SDA_SET();
    for (i = 0; i < 8; i++)
    {
        I2C_SCL_SET(); i2c_delay();
        dat <<= 1;
        if (I2C_SDA_READ()) dat |= 1;
        I2C_SCL_CLR(); i2c_delay();
    }
    if (ack) cs_i2c_ack(); else cs_i2c_nack();
    return dat;
}
