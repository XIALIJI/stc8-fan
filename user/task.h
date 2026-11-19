#ifndef __TASK_H__
#define __TASK_H__

#include "cs_stc8h.h"

typedef void (*task_cb_t)(void);

typedef struct {
    u8  run;          /* pending flag                       */
    u16 counter;      /* countdown (ms)                     */
    u16 period_ms;    /* reload period (ms)                 */
    task_cb_t cb;     /* task function                      */
} task_t;

#define TASK_ENTRY(period, fn)   { 0, (period), (period), (fn) }

extern task_t idata s_task_table[];
extern const u8 s_task_count;

void Task_Init(void);
void Task_Tick_Cb(void);      /* called from T4 ISR every 1ms */
void Task_Run_Pending(void);  /* called from main loop        */
u32  Task_GetMs(void);        /* global 1ms counter           */

#endif
