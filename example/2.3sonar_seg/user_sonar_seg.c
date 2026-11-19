#include "task.h"
#include "m_sonar.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.3: 超声波测距(100ms, 占T0) + 分区指示 + KEY1 冻结显示 */

static u16 s_dist = 0;
static bit s_freeze = 0;

static void key_task(void)
{
    KEY_Scan();
    if (KEY_GetNum() == KEY_VALUE_1) s_freeze = !s_freeze;
}

static void sonar_task(void)
{
    u16 d;
    if (s_freeze) return;

    d = Sonar_GetDistanceCm();
    s_dist = d;
    if (!Sonar_IsValid(d))
    {
        SegTM1650_DisplayDash();
        LED_Control(LED_RED,   LED_ON);
        LED_Control(LED_GREEN, LED_OFF);
        LED_Control(LED_BLUE,  LED_OFF);
    }
    else
    {
        SegTM1650_DisplayNum(d);
        LED_Control(LED_RED,   (d < 20)             ? LED_ON : LED_OFF);
        LED_Control(LED_GREEN, (d >= 20 && d < 50)  ? LED_ON : LED_OFF);
        LED_Control(LED_BLUE,  (d >= 50)            ? LED_ON : LED_OFF);
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  key_task),
    TASK_ENTRY(100, sonar_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    Sonar_Init();
    KEY_Init();
    LED_Init();
    SegTM1650_Init();
}
