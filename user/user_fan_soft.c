#include "user_fan.h"
#include "m_sonar.h"

void Fan_SoftUpdate(void)
{
    u16 d = Sonar_GetDistanceCm();
    u8 duty;
    g_dist = d;
    if (!Sonar_IsValid(d))
    {
        g_dist_valid = 0;
        Fan_SetReqDuty(0);
    }
    else
    {
        g_dist_valid = 1;
        if (d <= 20)      duty = 30;
        else if (d >= 80) duty = 100;
        else              duty = (u8)(30u + (((u32)(d - 20) * 70u) / 60u));
        Fan_SetReqDuty(duty);
    }
}

void Fan_SoftTask(void)
{
    if (g_run_mode == RUN_SOFT) Fan_SoftUpdate();
}

void Fan_SoftKey(u8 key)
{
    if (key == KEY_VALUE_1 || key == KEY_VALUE_2)
    {
        g_soft_item++;
        if (g_soft_item >= 3) g_soft_item = 0;
    }
}
