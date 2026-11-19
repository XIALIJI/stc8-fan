#include "task.h"
#include "bs_sc8h25a_temp_ds18b20.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.1: DS18B20 周期读温(1s), 低两位显示温度整数, 无效显示 ---- */

static float s_temp = 0.0F;
static bit   s_valid = 0;

static void temp_read_task(void)
{
    float t = DS18B20_ReadTemperature();
    if (t != TEMP_INVALID)
    {
        s_temp = t;
        s_valid = 1;
    }
    else
    {
        s_valid = 0;
    }
}

static void seg_show_task(void)
{
    if (s_valid)
    {
        SegTM1650_DisplayNum((u16)s_temp);
    }
    else
    {
        SegTM1650_DisplayDash();
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(1000, temp_read_task),
    TASK_ENTRY(200,  seg_show_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    SegTM1650_Init();
    DS18B20_Init();
}
