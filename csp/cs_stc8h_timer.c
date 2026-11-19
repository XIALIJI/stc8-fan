#include "cs_stc8h_timer.h"

/* overflow counter shared with sonar measurement */
volatile u16 cs_timer0_overflow;

static void (*s_timer4_tick_cb)(void) = NULL;

#define TIMER4_1MS_RELOAD   (65536UL - (MAIN_Fosc / 1000UL))   /* 41536 */

void Timer4_SetTickCallback(void (*cb)(void))
{
    s_timer4_tick_cb = cb;
}

void Timer4_Init(void)
{
    T4T3M &= ~0xF0;                 /* stop, timer mode, no clock out */
    IE2  |= ET4;                    /* enable T4 interrupt             */
    T4T3M |= (1 << 5);              /* T4: 1T mode                     */
    T4H = (u8)(TIMER4_1MS_RELOAD >> 8);
    T4L = (u8)(TIMER4_1MS_RELOAD & 0xFF);
    T4T3M |= (1 << 7);              /* T4 run                          */
}

void Timer0_Init(void)
{
    TMOD &= ~0x03;                  /* T0 mode 0: 16-bit auto-reload    */
    AUXR |= 0x80;                   /* T0: 1T mode                      */
    ET0 = 1;                        /* enable T0 interrupt              */
    TR0 = 0;                        /* keep stopped until needed        */
    cs_timer0_overflow = 0;
    TH0 = 0;
    TL0 = 0;
    TF0 = 0;
}

void Timer0_Reset(void)
{
    cs_timer0_overflow = 0;
    TH0 = 0;
    TL0 = 0;
    TF0 = 0;
}

void Timer0_Run(void)
{
    TR0 = 1;
}

void Timer0_Stop(void)
{
    TR0 = 0;
}

u32 Timer0_Read(void)
{
    u16 cnt;
    cnt = (u16)((TH0 << 8) | TL0);
    return (((u32)cs_timer0_overflow << 16) | (u32)cnt);
}

/* ---- Timer0 interrupt (overflow counter for sonar) ---- */
void Timer0_ISR(void) interrupt 1
{
    cs_timer0_overflow++;
}

/* ---- Timer4 interrupt (1ms task tick) ---- */
void Timer4_ISR(void) interrupt 20
{
    if (s_timer4_tick_cb != NULL)
    {
        s_timer4_tick_cb();
    }
}
