#include "m_sonar.h"
#include "cs_stc8h_timer.h"
#include "stc8h_delay.h"

/* HC-SR04 style sonar: TRIG=P2.6, ECHO=P2.5, uses T0 (blocking measure). */

#define TRIG  P26
#define ECHO  P25

/* 24MHz 1T: round-trip = 2*0.01m / 340m/s = 58.82us/cm -> 1412 ticks/cm */
#define CNT_PER_CM  1412UL

void Sonar_Init(void)
{
    GPIO_SET_MODE(P2M1, P2M0, 0x40, GPIO_PP);   /* P2.6 TRIG push-pull */
    GPIO_SET_MODE(P2M1, P2M0, 0x20, GPIO_HIZ);  /* P2.5 ECHO input     */
    TRIG = 0;
    Timer0_Init();
}

u16 Sonar_GetDistanceCm(void)
{
    u32 cnt;
    u16 wait;

    TRIG = 1;
    Delay_10us();
    TRIG = 0;

    wait = 0;
    while (ECHO == 0)
    {
        if (++wait == 0) return SONAR_ERR_NO_ECHO;
    }

    Timer0_Reset();
    Timer0_Run();

    wait = 0;
    while (ECHO == 1)
    {
        if (++wait == 0) { Timer0_Stop(); return SONAR_ERR_TIMEOUT; }
    }
    Timer0_Stop();

    cnt = Timer0_Read() / CNT_PER_CM;
    if (cnt > SONAR_VALID_MAX) return SONAR_ERR_OVERFLOW;
    return (u16)cnt;
}

u8 Sonar_IsValid(u16 dist)
{
    return (dist <= SONAR_VALID_MAX) ? 1 : 0;
}
