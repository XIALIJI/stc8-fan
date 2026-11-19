#include "cs_stc8h_pwm.h"

/* PWMB channels 5..8 share one period (PWMB_ARR). Only PWM6 (motor) is
 * driven here. The servo also lies on PWMB (PWM8), but servo needs 50Hz
 * while motor needs ~1kHz and a single ARR cannot satisfy both, so the
 * servo uses a software PWM (see bsp/m_servo.c). */

/* PWMB_CCER1: CC6E = bit4 */
#define PWMB_CC6E   0x10
#define PWMB_ENO6   0x04

void pwm_init(u8 ch, u32 freq_hz, u8 duty)
{
    u32 period;
    u16 arr;

    P_SW2 |= 0x80;              /* access XSFR (PWMB_*) */

    if (freq_hz == 0) freq_hz = 1000;
    period = MAIN_Fosc / freq_hz;   /* ARR + 1 */
    if (period == 0)    period = 1;
    if (period > 65536UL) period = 65536UL;
    arr = (u16)(period - 1);

    if (ch == PWM_CH_MOTOR)     /* PWM6 -> P2.1 */
    {
        PWMB_CCER1 &= ~PWMB_CC6E;   /* close channel before CCMR write */
        PWMB_CCMR2 = 0x60;          /* channel 6: PWM mode 1           */
        PWMB_CCER1 |= PWMB_CC6E;    /* CC6E=1, CC6P=0 (active high)     */
        PWMB_CCMR2 |= 0x08;         /* preload enable                   */
        PWMB_ARRH = (u8)(arr >> 8);
        PWMB_ARRL = (u8)(arr & 0xFF);
        PWMB_ENO  |= PWMB_ENO6;     /* PWM6 output enable               */
        PWMB_PS   &= ~0x0C;         /* PWM6_1: P2.1 (default 0)         */
    }

    PWMB_BKR = 0x80;                /* main output enable               */
    PWMB_CR1 |= 0x01;               /* counter enable                   */
    pwm_duty(ch, duty);
}

void pwm_duty(u8 ch, u8 duty)
{
    u16 ccr;
    if (duty > PWM_DUTY_MAX) duty = PWM_DUTY_MAX;
    /* CCR = (ARR+1) * duty / 100; store ARR+1 approximation implicitly
     * by re-reading ARR: period counts = ARR + 1. */
    ccr = (u16)(((u32)(((u16)PWMB_ARRH << 8) | PWMB_ARRL) + 1UL) * duty / PWM_DUTY_MAX);

    P_SW2 |= 0x80;
    if (ch == PWM_CH_MOTOR)
    {
        PWMB_CCR6H = (u8)(ccr >> 8);
        PWMB_CCR6L = (u8)(ccr & 0xFF);
    }
}

void pwm_output_control(u8 ch, u8 enable)
{
    P_SW2 |= 0x80;
    if (ch == PWM_CH_MOTOR)
    {
        if (enable) PWMB_ENO |= PWMB_ENO6;
        else        PWMB_ENO &= ~PWMB_ENO6;
    }
}
