#include "task.h"
#include "cs_stc8h_adc.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.4: 光敏电阻控白灯(P10), 高8位比较, 阈值100±(KEY1+5/KEY2-5),
 * 滞回门限=阈值-4(阈值<4取0); KEY1+KEY2 切 ADC/阈值 显示 */

#define LIGHT_TH_FILL_DEFAULT   100

static u8  s_th = LIGHT_TH_FILL_DEFAULT;
static u8  s_adc8 = 0;
static bit s_show_th = 0;    /* 0=显ADC(绿), 1=显阈值(红) */

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    switch (key)
    {
        case KEY_VALUE_1:     /* 阈值 +5 */
            if (s_th <= 250) s_th += 5; else s_th = 255;
            break;
        case KEY_VALUE_2:     /* 阈值 -5 */
            if (s_th >= 5) s_th -= 5; else s_th = 0;
            break;
        case KEY_VALUE_BOTH:  /* 切显示 */
            s_show_th = !s_show_th;
            break;
        default:
            break;
    }
}

static void light_task(void)
{
    u8 h8 = adc_read_h8(ADC_P10);
    u8 off;
    s_adc8 = h8;
    if (h8 > s_th)
    {
        LED_Control(LED_WHITE, LED_ON);
    }
    else
    {
        off = (s_th >= 4) ? (u8)(s_th - 4) : 0;
        if (h8 < off)
        {
            LED_Control(LED_WHITE, LED_OFF);
        }
    }
}

static void seg_show_task(void)
{
    if (s_show_th)
    {
        SegTM1650_DisplayNum(s_th);
        LED_Control(LED_RED,   LED_ON);
        LED_Control(LED_GREEN, LED_OFF);
    }
    else
    {
        SegTM1650_DisplayNum(s_adc8);
        LED_Control(LED_GREEN, LED_ON);
        LED_Control(LED_RED,   LED_OFF);
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  key_task),
    TASK_ENTRY(150, light_task),
    TASK_ENTRY(200, seg_show_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    adc_init(ADC_P10, 15, 12);
    KEY_Init();
    LED_Init();
    SegTM1650_Init();
    s_th = LIGHT_TH_FILL_DEFAULT;
    LED_Control(LED_WHITE, LED_OFF);
    LED_Control(LED_GREEN, LED_ON);
}
