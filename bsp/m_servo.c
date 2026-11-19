#include "m_servo.h"
#include "cs_stc8h_timer.h"

/* Servo on P2.3, 50Hz (20ms), 500~2500us pulse.
 * Implemented as software PWM using T2 (20ms frame) + T3 (pulse width).
 * Reason: PWM8(P2.3) and PWM6(motor,P2.1) share the PWMB period register;
 * 1kHz (motor) and 50Hz (servo) cannot be produced by one PWM group. */

#define SERVO_PIN         P23
#define SERVO_PERIOD_US   20000UL
#define TICK_PER_US       2UL     /* 12T mode: 0.5us per tick */

static u16 s_pulse_ticks = 3000;
static u8  s_angle = 90;
static bit s_enabled = 1;

static u16 angle_to_ticks(u8 angle)
{
    u32 us;
    if (angle > 180) angle = 180;
    us = SERVO_PULSE_MIN_US + ((u32)angle * (SERVO_PULSE_MAX_US - SERVO_PULSE_MIN_US)) / 180UL;
    return (u16)(us * TICK_PER_US);
}

void Servo_Init(void)
{
    GPIO_SET_MODE(P2M1, P2M0, 0x08, GPIO_PP);   /* P2.3 push-pull */
    s_angle = SERVO_ANGLE_MID;
    s_pulse_ticks = angle_to_ticks(s_angle);
    s_enabled = 1;
    SERVO_PIN = 0;

    /* T2: frame, 12T, auto-reload */
    AUXR &= ~0x1C;
    T2H = (u8)((65536UL - (SERVO_PERIOD_US * TICK_PER_US)) >> 8);
    T2L = (u8)((65536UL - (SERVO_PERIOD_US * TICK_PER_US)) & 0xFF);
    IE2 |= ET2;
    /* T3: pulse, 12T, stopped */
    T4T3M &= ~0x0F;
    IE2 |= ET3;
    T3H = (u8)((65536UL - s_pulse_ticks) >> 8);
    T3L = (u8)((65536UL - s_pulse_ticks) & 0xFF);
    AUXR |= (1 << 4);   /* T2 run */
}

void Servo_SetAngle(u8 angle)
{
    if (angle > 180) angle = 180;
    s_angle = angle;
    s_pulse_ticks = angle_to_ticks(angle);
}

u8 Servo_GetAngle(void)
{
    return s_angle;
}

void Servo_SetPulseUs(u16 pulse_us)
{
    if (pulse_us < SERVO_PULSE_MIN_US) pulse_us = SERVO_PULSE_MIN_US;
    if (pulse_us > SERVO_PULSE_MAX_US) pulse_us = SERVO_PULSE_MAX_US;
    s_pulse_ticks = (u16)(pulse_us * TICK_PER_US);
    s_angle = (u8)(((u32)(pulse_us - SERVO_PULSE_MIN_US) * 180UL) / (SERVO_PULSE_MAX_US - SERVO_PULSE_MIN_US));
}

void Servo_OutputEnable(u8 enable)
{
    s_enabled = enable ? 1 : 0;
    if (!s_enabled) SERVO_PIN = 0;
}

/* T2 ISR (frame start): drive high, arm T3 for the pulse width */
void Servo_FrameISR(void) interrupt 12
{
    if (s_enabled)
    {
        SERVO_PIN = 1;
        T3H = (u8)((65536UL - s_pulse_ticks) >> 8);
        T3L = (u8)((65536UL - s_pulse_ticks) & 0xFF);
        T4T3M |= (1 << 3);   /* T3 run */
    }
    else
    {
        SERVO_PIN = 0;
    }
}

/* T3 ISR (pulse end): drive low, stop T3 */
void Servo_PulseISR(void) interrupt 19
{
    SERVO_PIN = 0;
    T4T3M &= ~(1 << 3);   /* T3 stop */
}
