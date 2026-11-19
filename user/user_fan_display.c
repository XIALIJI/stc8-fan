#include "user_fan.h"
#include "bs_sc8h25a_seg_tm1650.h"
#include "task.h"

static u8 temp_int(void)
{
    u8 t;
    if (!g_temp_valid) return 0;
    t = (u8)g_temp;
    if (t > 99) t = 99;
    return t;
}

static void show_2_2(u8 hi, u8 lo)
{
    SegTM1650_Clear();
    SegTM1650_DisplayChar(4, (u8)('0' + (hi / 10)), 0);
    SegTM1650_DisplayChar(3, (u8)('0' + (hi % 10)), 0);
    SegTM1650_DisplayChar(2, (u8)('0' + (lo / 10)), 0);
    SegTM1650_DisplayChar(1, (u8)('0' + (lo % 10)), 0);
}

void Fan_DispTask(void)
{
    switch (g_mode)
    {
        case MODE_MANUAL:
            if (!g_temp_valid) SegTM1650_DisplayDash();
            else show_2_2(temp_int(), g_gear);
            break;
        case MODE_AUTO:
            if (!g_temp_valid) SegTM1650_DisplayDash();
            else show_2_2(temp_int(), g_th_temp);
            break;
        case MODE_SOFT:
        {
            u8 lo;
            if (g_soft_item == 0)
            {
                if (!g_dist_valid) { SegTM1650_DisplayDash(); break; }
                lo = (g_dist > 99) ? 99 : (u8)g_dist;
            }
            else if (g_soft_item == 1)
            {
                lo = temp_int();
            }
            else
            {
                lo = g_duty;
            }
            show_2_2(temp_int(), lo);
            break;
        }
        case MODE_POT:
            if (g_pot_show_duty) SegTM1650_DisplayNum(g_duty);
            else                 SegTM1650_DisplayNum(g_pot_adc8);
            break;
        case MODE_LIGHT:
        {
            u8 sub = g_light_sub;
            SegTM1650_Clear();
            SegTM1650_DisplayChar(4, (u8)('0' + (sub + 1)), 0);
            if (sub == LIGHT_SUB_OFF)
            {
                SegTM1650_DisplayChar(3, '-', 0);
                SegTM1650_DisplayChar(2, '-', 0);
                SegTM1650_DisplayChar(1, '-', 0);
            }
            else if (g_light_flash_until && Task_GetMs() < g_light_flash_until)
            {
                u8 th = (sub == LIGHT_SUB_FILL) ? g_th_fill : g_th_stop;
                SegTM1650_DisplayChar(3, (u8)('0' + (th / 100)), 0);
                SegTM1650_DisplayChar(2, (u8)('0' + ((th / 10) % 10)), 0);
                SegTM1650_DisplayChar(1, (u8)('0' + (th % 10)), 0);
            }
            else
            {
                SegTM1650_DisplayChar(3, (u8)('0' + (g_light_adc8 / 100)), 0);
                SegTM1650_DisplayChar(2, (u8)('0' + ((g_light_adc8 / 10) % 10)), 0);
                SegTM1650_DisplayChar(1, (u8)('0' + (g_light_adc8 % 10)), 0);
            }
            break;
        }
        case MODE_SWING:
            SegTM1650_Clear();
            SegTM1650_DisplayChar(4, g_swing_en ? '1' : '0', 0);
            SegTM1650_DisplayChar(3, '-', 0);
            SegTM1650_DisplayChar(2, (u8)('0' + (g_theta / 10)), 0);
            SegTM1650_DisplayChar(1, (u8)('0' + (g_theta % 10)), 0);
            break;
        default:
            if (g_store_save_until && Task_GetMs() < g_store_save_until)
            {
                SegTM1650_Clear();
                SegTM1650_DisplayChar(4, 'S', 0);
                SegTM1650_DisplayChar(3, 'A', 0);
                SegTM1650_DisplayChar(2, 'U', 0);
                SegTM1650_DisplayChar(1, 'E', 0);
            }
            else if (!g_temp_valid)
            {
                SegTM1650_DisplayDash();
            }
            else
            {
                show_2_2(temp_int(), g_th_temp);
            }
            break;
    }
}
