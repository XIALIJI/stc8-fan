#ifndef __M_MOTOR_H__
#define __M_MOTOR_H__

#include "cs_stc8h.h"

#define MOTOR_SPEED_MAX   100

void Motor_Init(void);
void Motor_SetSpeed(u8 duty);         /* 0~100 */
void Motor_Stop(void);
u8   Motor_GetSpeed(void);
void Motor_OutputEnable(u8 enable);

#endif
