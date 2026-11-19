#include "task.h"
#include "m_motor.h"
#include "bs_sc8h25a_temp_ds18b20.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_led_ws2812b.h"
#include "bs_sc8h25a_seg_tm1650.h"

static u8 code DUTY_TAB[4] = {0, 30, 60, 90};
static u8  s_gear = 0;
static float s_temp = 0.0F;
static bit   s_temp_valid = 0;
static u8  s_phase = 0;

static void update_motor(void) { Motor_SetSpeed(DUTY_TAB[s_gear]); }

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    if (key == KEY_VALUE_1)
    {
        if (s_gear > 0) { s_gear--; update_motor(); }
    }
    else if (key == KEY_VALUE_2)
    {
        if (s_gear < 3) { s_gear++; update_motor(); }
    }
}

static void temp_task(void)
{
    float t = DS18B20_ReadTemperature();
    if (t != TEMP_INVALID) { s_temp = t; s_temp_valid = 1; }
    else s_temp_valid = 0;
}

static void seg_task(void)
{
    u8 ti;
    if (s_temp_valid)
    {
        ti = (u8)s_temp;
        if (ti > 99) ti = 99;
        SegTM1650_Clear();
        SegTM1650_DisplayChar(4, (u8)('0' + (ti / 10)), 0);
        SegTM1650_DisplayChar(3, (u8)('0' + (ti % 10)), 0);
        SegTM1650_DisplayChar(2, (u8)('0' + (s_gear / 10)), 0);
        SegTM1650_DisplayChar(1, (u8)('0' + (s_gear % 10)), 0);
    }
    else
    {
        SegTM1650_DisplayDash();
    }
}

static void ws_task(void)
{
    u8 d = DUTY_TAB[s_gear];
    u8 i;
    if (d == 0)
    {
        for (i = 0; i < WS2812B_LED_NUM; i++)
        {
            switch ((i + s_phase) & 3)
            {
                case 0: WS2812B_SetPixel(i, 255, 0, 0); break;
                case 1: WS2812B_SetPixel(i, 0, 255, 0); break;
                case 2: WS2812B_SetPixel(i, 0, 0, 255); break;
                default:WS2812B_SetPixel(i, 255, 255, 255); break;
            }
        }
        s_phase++;
        if (s_phase >= 4) s_phase = 0;
    }
    else if (d <= 30) WS2812B_SetColorAll(255, 128, 0);
    else if (d <= 60) WS2812B_SetColorAll(0, 255, 80);
    else              WS2812B_SetColorAll(0, 200, 255);
    WS2812B_Show();
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,   key_task),
    TASK_ENTRY(1000, temp_task),
    TASK_ENTRY(200,  seg_task),
    TASK_ENTRY(100,  ws_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    Motor_Init();
    DS18B20_Init();
    KEY_Init();
    LED_Init();
    WS2812B_Init();
    SegTM1650_Init();
    s_gear = 0;
    Motor_Stop();
    LED_Control(LED_RED, LED_ON);
}
