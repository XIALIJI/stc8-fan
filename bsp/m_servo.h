#ifndef __M_SERVO_H__
#define __M_SERVO_H__

#include "cs_stc8h.h"

#define SERVO_ANGLE_MIN      0
#define SERVO_ANGLE_MAX      180
#define SERVO_ANGLE_MID      90
#define SERVO_PULSE_MIN_US   500
#define SERVO_PULSE_MAX_US   2500

void Servo_Init(void);
void Servo_SetAngle(u8 angle);         /* 0~180 deg */
u8   Servo_GetAngle(void);
void Servo_SetPulseUs(u16 pulse_us);   /* 500~2500us */
void Servo_OutputEnable(u8 enable);

#endif
