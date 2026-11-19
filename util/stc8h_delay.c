#include "stc8h_delay.h"
#include <intrins.h>

/* Blocking delays based on 24MHz 1T system clock.
 * Values are approximate; do NOT use in the 1ms task ISR or
 * in tight real-time paths. For DS18B20 / WS2812B the dedicated
 * drivers implement their own calibrated timing. */

void Delay_10us(void)
{
    u8 i;
    i = 79;
    while (--i)
    {
        _nop_();
    }
}

void Delay_1ms(void)
{
    u16 i;
    i = 3600;
    while (--i)
    {
        _nop_();
    }
}

void Delay_ms(u16 ms)
{
    while (ms--)
    {
        Delay_1ms();
    }
}

void Delay_us(u8 us)
{
    while (us >= 10)
    {
        Delay_10us();
        us -= 10;
    }
}
