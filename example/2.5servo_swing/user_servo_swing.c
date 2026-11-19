#include "task.h"
#include "m_servo.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.5: 舵机 90°±θ 往复; θ默认45(5~90); KEY1/2 ±5, KEY1+KEY2 开关 */

#define SWING_STEP   2

static u8  s_theta = 45;
static bit s_scan_on = 0;
static u8  s_angle = 90;
static bit s_dir_up = 1;

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    switch (key)
    {
        case KEY_VALUE_1:
            if (s_theta <= 85) s_theta += 5; else s_theta = 90;
            break;
        case KEY_VALUE_2:
            if (s_theta >= 10) s_theta -= 5; else s_theta = 5;
            break;
        case KEY_VALUE_BOTH:
            s_scan_on = !s_scan_on;
            if (!s_scan_on) { s_angle = 90; Servo_SetAngle(90); }
            break;
        default:
            break;
    }
}

static void swing_task(void)
{
    if (!s_scan_on) return;
    if (s_dir_up)
    {
        if (s_angle < 90 + s_theta - SWING_STEP) s_angle += SWING_STEP;
        else { s_angle = 90 + s_theta; s_dir_up = 0; }
    }
    else
    {
        if (s_angle > 90 - s_theta + SWING_STEP) s_angle -= SWING_STEP;
        else { s_angle = 90 - s_theta; s_dir_up = 1; }
    }
    Servo_SetAngle(s_angle);
}

static void seg_show_task(void)
{
    if (s_scan_on)
    {
        SegTM1650_DisplayNum(s_angle);
        LED_Control(LED_GREEN, LED_ON);
        LED_Control(LED_RED,   LED_OFF);
    }
    else
    {
        SegTM1650_DisplayNum(s_theta);
        LED_Control(LED_RED,   LED_ON);
        LED_Control(LED_GREEN, LED_OFF);
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  key_task),
    TASK_ENTRY(80,  swing_task),
    TASK_ENTRY(100, seg_show_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    Servo_Init();
    KEY_Init();
    LED_Init();
    SegTM1650_Init();
}
