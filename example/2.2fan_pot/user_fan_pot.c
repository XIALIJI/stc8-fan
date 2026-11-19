#include "task.h"
#include "cs_stc8h_adc.h"
#include "m_motor.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.2: 电位器 ADC 无极调速
 * 20ms 采样均值≥4次, 高8位映射占空比0~100; KEY1 切换显示(占空比/ADC) */

static u8  s_duty = 0;
static u8  s_adc8 = 0;
static bit s_show_duty = 1;   /* 1=显占空比(绿), 0=显ADC(红) */

static void fan_pot_task(void)
{
    u8 key;
    u8 h8 = adc_read_avg_h8(ADC_P11, 4);
    s_adc8 = h8;
    s_duty = (u8)(((u16)h8 * 100u) / 255u);
    Motor_SetSpeed(s_duty);

    KEY_Scan();
    key = KEY_GetNum();
    if (key == KEY_VALUE_1) s_show_duty = !s_show_duty;
}

static void seg_show_task(void)
{
    if (s_show_duty)
    {
        SegTM1650_DisplayNum(s_duty);
        LED_Control(LED_GREEN, LED_ON);
        LED_Control(LED_RED,   LED_OFF);
    }
    else
    {
        SegTM1650_DisplayNum(s_adc8);
        LED_Control(LED_RED,   LED_ON);
        LED_Control(LED_GREEN, LED_OFF);
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  fan_pot_task),
    TASK_ENTRY(100, seg_show_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    adc_init(ADC_P11, 15, 12);
    Motor_Init();
    KEY_Init();
    LED_Init();
    SegTM1650_Init();
    LED_Control(LED_GREEN, LED_ON);
}
