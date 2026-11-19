#ifndef __CS_STC8H_TIMER_H__
#define __CS_STC8H_TIMER_H__

#include "cs_stc8h.h"

/* ===== Task tick: Timer4, 1ms ===== */
void Timer4_Init(void);                         /* 16-bit auto-reload, 1ms */
void Timer4_SetTickCallback(void (*cb)(void));

/* ===== Pulse measurement: Timer0, 16-bit auto-reload 1T ===== */
void Timer0_Init(void);                         /* prepare, stopped */
void Timer0_Reset(void);                        /* clear overflow+count   */
void Timer0_Run(void);                          /* TR0 = 1                */
void Timer0_Stop(void);                         /* TR0 = 0                */
u32  Timer0_Read(void);                         /* overflow<<16 | count   */

#endif /* __CS_STC8H_TIMER_H__ */
