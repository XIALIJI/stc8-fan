#include "cs_stc8h_adc.h"

/* STC8H 12-bit ADC, right-aligned (ADCCFG bit5=1) */

void adc_init(u8 ch, u8 clk, u8 res)
{
    P_SW2 |= 0x80;
    ADCCFG = (u8)(((res >= 12) ? 0x20 : 0x00) | (clk & 0x0F));
    ADC_CONTR = 0x80 | (ch & 0x0F);     /* power on + select channel */
}

u16 adc_read(u8 ch)
{
    ADC_CONTR = (ADC_CONTR & 0xF0) | 0x40 | (ch & 0x0F);  /* keep power, START */
    while ((ADC_CONTR & 0x20) == 0);                       /* wait FLAG        */
    ADC_CONTR &= ~0x20;
    return (u16)(((u16)ADC_RES << 8) | ADC_RESL);
}

u16 adc_read_avg(u8 ch, u8 n)
{
    u32 sum = 0;
    u8 i;
    if (n == 0) n = 1;
    for (i = 0; i < n; i++) sum += adc_read(ch);
    return (u16)(sum / n);
}

u8 adc_read_h8(u8 ch)
{
    return (u8)(adc_read(ch) >> 4);
}

u8 adc_read_avg_h8(u8 ch, u8 n)
{
    return (u8)(adc_read_avg(ch, n) >> 4);
}
