#ifndef __CS_STC8H_PWM_H__
#define __CS_STC8H_PWM_H__

#include "cs_stc8h.h"

#define PWM_CH_MOTOR   6    /* P2.1, hardware PWM, ~1kHz  */
#define PWM_CH_SERVO   8    /* reserved: servo uses software PWM (see m_servo) */
#define PWM_DUTY_MAX   100

void pwm_init(u8 ch, u32 freq_hz, u8 duty);
void pwm_duty(u8 ch, u8 duty);
void pwm_output_control(u8 ch, u8 enable);

#endif
