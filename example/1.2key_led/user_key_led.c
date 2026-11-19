#include "task.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"

/* Project 1.2: non-blocking key scan + LED feedback (20ms task)
 * KEY1: 已是红独亮则全灭，否则红独亮
 * KEY2: 已是绿独亮则全灭，否则绿独亮
 * KEY1+KEY2: 红→绿→蓝→红…独亮循环 */

static u8 s_solo = 0xFF;   /* 当前独亮色; 0xFF=全灭 */
static u8 s_cycle = 0;

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    switch (key)
    {
        case KEY_VALUE_1:
            if (s_solo == LED_RED) { LED_AllOff(); s_solo = 0xFF; }
            else                   { LED_Solo(LED_RED); s_solo = LED_RED; }
            break;
        case KEY_VALUE_2:
            if (s_solo == LED_GREEN) { LED_AllOff(); s_solo = 0xFF; }
            else                     { LED_Solo(LED_GREEN); s_solo = LED_GREEN; }
            break;
        case KEY_VALUE_BOTH:
        {
            u8 c;
            if (s_cycle == 0)      c = LED_RED;
            else if (s_cycle == 1) c = LED_GREEN;
            else                   c = LED_BLUE;
            LED_Solo(c);
            s_solo = c;
            s_cycle++;
            if (s_cycle >= 3) s_cycle = 0;
            break;
        }
        default:
            break;
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20, key_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    LED_Init();
    KEY_Init();
}
