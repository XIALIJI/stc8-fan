#include "user_fan.h"
#include "cs_stc8h_adc.h"

void Fan_PotUpdate(void)
{
    u8 h8 = adc_read_avg_h8(ADC_P11, 4);
    g_pot_adc8 = h8;
    Fan_SetReqDuty((u8)(((u16)h8 * 100u) / 255u));
}

void Fan_PotTask(void)
{
    if (g_run_mode == RUN_POT) Fan_PotUpdate();
}

void Fan_PotKey(u8 key)
{
    if (key == KEY_VALUE_1 || key == KEY_VALUE_2) g_pot_show_duty = !g_pot_show_duty;
}
