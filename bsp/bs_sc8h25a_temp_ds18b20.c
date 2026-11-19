#include "bs_sc8h25a_temp_ds18b20.h"
#include "stc8h_delay.h"
#include <intrins.h>

/* DS18B20 on P2.4 (one-wire). Interrupts are masked during bit-timing
 * (not during the 750ms conversion) so the T4 task tick cannot corrupt
 * the 1-wire slots. Timing is cycle-sensitive; verify on board. */

#define DQ_LOW()        (P24 = 0)
#define DQ_RELEASE()    (P24 = 1)
#define DQ_READ()       (P24)

void DS18B20_Init(void)
{
    GPIO_SET_MODE(P2M1, P2M0, 0x10, GPIO_QUASI);
    P_SW2 |= 0x80;
    P2PU  |= 0x10;
    DQ_RELEASE();
}

static void ds_delay_us(u16 us)
{
    while (us--)
    {
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_(); _nop_(); _nop_();
        _nop_(); _nop_(); _nop_();
    }
}

static bit ds_reset(void)
{
    bit present;
    DQ_LOW();
    ds_delay_us(480);
    DQ_RELEASE();
    ds_delay_us(60);
    present = DQ_READ();
    ds_delay_us(420);
    return (present == 0) ? 1 : 0;
}

static void ds_write_bit(bit b)
{
    DQ_LOW();
    if (b)
    {
        ds_delay_us(8);
        DQ_RELEASE();
        ds_delay_us(55);
    }
    else
    {
        ds_delay_us(65);
        DQ_RELEASE();
        ds_delay_us(2);
    }
}

static void ds_write_byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        ds_write_bit(dat & 0x01);
        dat >>= 1;
    }
}

static bit ds_read_bit(void)
{
    bit b;
    DQ_LOW();
    ds_delay_us(2);
    DQ_RELEASE();
    ds_delay_us(8);
    b = DQ_READ();
    ds_delay_us(50);
    return b;
}

static u8 ds_read_byte(void)
{
    u8 i, dat;
    dat = 0;
    for (i = 0; i < 8; i++)
    {
        if (ds_read_bit()) dat |= (u8)(1 << i);
    }
    return dat;
}

float DS18B20_ReadTemperature(void)
{
    u8 lsb, msb;
    s16 raw;
    bit ea_save;

    ea_save = EA;
    EA = 0;
    if (!ds_reset()) { EA = ea_save; return TEMP_INVALID; }
    ds_write_byte(0xCC);
    ds_write_byte(0x44);
    EA = ea_save;

    Delay_ms(750);

    EA = 0;
    if (!ds_reset()) { EA = ea_save; return TEMP_INVALID; }
    ds_write_byte(0xCC);
    ds_write_byte(0xBE);
    lsb = ds_read_byte();
    msb = ds_read_byte();
    EA = ea_save;

    raw = (s16)(((u16)msb << 8) | lsb);
    return (float)raw * 0.0625F;
}
