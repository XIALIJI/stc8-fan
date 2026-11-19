#include "m_motor.h"
#include "cs_stc8h_pwm.h"

/* IN1 = P2.0 (fixed low), IN2 = P2.1 = PWM6 (hardware PWM, ~1kHz) */

static u8 s_speed = 0;
static u8 s_enabled = 1;

void Motor_Init(void)
{
    GPIO_SET_MODE(P2M1, P2M0, 0x01, GPIO_PP);   /* P2.0 IN1 push-pull */
    GPIO_SET_MODE(P2M1, P2M0, 0x02, GPIO_PP);   /* P2.1 IN2 push-pull */
    P20 = 0;
    s_speed = 0;
    s_enabled = 1;
    pwm_init(PWM_CH_MOTOR, 1000UL, 0);
}

void Motor_SetSpeed(u8 duty)
{
    if (duty > MOTOR_SPEED_MAX) duty = MOTOR_SPEED_MAX;
    s_speed = duty;
    pwm_duty(PWM_CH_MOTOR, duty);
}

void Motor_Stop(void)
{
    Motor_SetSpeed(0);
}

u8 Motor_GetSpeed(void)
{
    return s_speed;
}

void Motor_OutputEnable(u8 enable)
{
    s_enabled = enable ? 1 : 0;
    pwm_output_control(PWM_CH_MOTOR, s_enabled);
}
