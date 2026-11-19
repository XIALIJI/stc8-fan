#include "user_fan.h"
#include "bs_sc8h25a_temp_ds18b20.h"

void Fan_TempTask(void)
{
    float t = DS18B20_ReadTemperature();
    if (t != TEMP_INVALID)
    {
        g_temp = t;
        g_temp_valid = 1;
    }
    else
    {
        g_temp_valid = 0;
    }
}

void Fan_AutoUpdate(void)
{
    u8 d;
    s16 dt;
    if (!g_temp_valid) d = 0;
    else
    {
        dt = (s16)((s16)(u8)g_temp - (s16)g_th_temp);
        if (dt <= 0)      d = 0;
        else if (dt < 2)  d = 30;
        else if (dt < 5)  d = 60;
        else              d = 90;
    }
    Fan_SetReqDuty(d);
}

void Fan_AutoTask(void)
{
    if (g_run_mode == RUN_AUTO) Fan_AutoUpdate();
}

void Fan_AutoKey(u8 key)
{
    if (key == KEY_VALUE_1)
    {
        if (g_th_temp <= TH_TEMP_MIN) g_th_temp = TH_TEMP_MAX; else g_th_temp--;
    }
    else if (key == KEY_VALUE_2)
    {
        g_th_temp++;
        if (g_th_temp > TH_TEMP_MAX) g_th_temp = TH_TEMP_MIN;
    }
}
