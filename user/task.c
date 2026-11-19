#include "task.h"
#include "cs_stc8h_timer.h"

static volatile u32 s_ms_tick = 0;

void Task_Init(void)
{
    Timer4_SetTickCallback(Task_Tick_Cb);
    Timer4_Init();
}

void Task_Tick_Cb(void)
{
    u8 i;
    s_ms_tick++;
    for (i = 0; i < s_task_count; i++)
    {
        if (s_task_table[i].counter)
        {
            s_task_table[i].counter--;
            if (s_task_table[i].counter == 0)
            {
                s_task_table[i].counter = s_task_table[i].period_ms;
                s_task_table[i].run = 1;
            }
        }
    }
}

void Task_Run_Pending(void)
{
    u8 i;
    for (i = 0; i < s_task_count; i++)
    {
        if (s_task_table[i].run)
        {
            s_task_table[i].run = 0;
            s_task_table[i].cb();
        }
    }
}

u32 Task_GetMs(void)
{
    return s_ms_tick;
}
