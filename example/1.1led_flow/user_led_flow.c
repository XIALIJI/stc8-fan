#include "task.h"
#include "bs_sc8h25a_led.h"

/* Project 1.1: tri-color LED flow, 500ms task, red->green->blue */

static u8 s_step = 0;

static void led_flow_task(void)
{
    switch (s_step)
    {
        case 0:  LED_Solo(LED_RED);   break;
        case 1:  LED_Solo(LED_GREEN); break;
        default: LED_Solo(LED_BLUE);  break;
    }
    s_step++;
    if (s_step >= 3) s_step = 0;
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(500, led_flow_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    LED_Init();
}
