#include "task.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led_ws2812b.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.6: WS2812B 档位灯效; KEY1 正向/KEY2 反向调档0~3
 * 档0橙黄 / 档1翠绿 / 档2冰蓝 / 档3四灯异色旋转流水 */

static u8 s_level = 0;
static u8 s_phase = 0;

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    if (key == KEY_VALUE_1)
    {
        s_level++;
        if (s_level >= 4) s_level = 0;
    }
    else if (key == KEY_VALUE_2)
    {
        if (s_level == 0) s_level = 3; else s_level--;
    }
}

static void ws_task(void)
{
    u8 i;
    if (s_level < 3)
    {
        switch (s_level)
        {
            case 0: WS2812B_SetColorAll(255, 128, 0);   break;  /* 橙黄 */
            case 1: WS2812B_SetColorAll(0, 255, 80);    break;  /* 翠绿 */
            default: WS2812B_SetColorAll(0, 200, 255);  break;  /* 冰蓝 */
        }
    }
    else
    {
        for (i = 0; i < WS2812B_LED_NUM; i++)
        {
            switch ((i + s_phase) & 3)
            {
                case 0: WS2812B_SetPixel(i, 255, 0, 0);   break;
                case 1: WS2812B_SetPixel(i, 0, 255, 0);   break;
                case 2: WS2812B_SetPixel(i, 0, 0, 255);   break;
                default:WS2812B_SetPixel(i, 255, 255, 255);break;
            }
        }
        s_phase++;
        if (s_phase >= 4) s_phase = 0;
    }
    WS2812B_Show();
    SegTM1650_DisplayNum(s_level);
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  key_task),
    TASK_ENTRY(100, ws_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    WS2812B_Init();
    KEY_Init();
    SegTM1650_Init();
}
