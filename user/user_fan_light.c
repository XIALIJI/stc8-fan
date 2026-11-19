#include "user_fan.h"
#include "cs_stc8h_adc.h"
#include "bs_sc8h25a_led.h"
#include "task.h"

void Fan_LightTask(void)
{
    u8 h8;
    u8 off;
    h8 = adc_read_h8(ADC_P10);
    g_light_adc8 = h8;

    if (g_light_sub == LIGHT_SUB_FILL)
    {
        if (h8 > g_th_fill)
        {
            LED_Control(LED_WHITE, LED_ON);
        }
        else
        {
            off = (g_th_fill >= 4) ? (u8)(g_th_fill - 4) : 0;
            if (h8 < off) LED_Control(LED_WHITE, LED_OFF);
        }
    }
    else
    {
        LED_Control(LED_WHITE, LED_OFF);
    }

    Fan_ResolveDuty();
}

void Fan_LightKey(u8 key)
{
    if (key == KEY_VALUE_1)
    {
        g_light_sub++;
        if (g_light_sub > LIGHT_SUB_OFF) g_light_sub = LIGHT_SUB_FILL;
    }
    else if (key == KEY_VALUE_2)
    {
        if (g_light_sub == LIGHT_SUB_FILL)
        {
            if (g_th_fill <= 250) g_th_fill += 5; else g_th_fill = 0;
            g_light_flash_until = Task_GetMs() + 2000;
        }
        else if (g_light_sub == LIGHT_SUB_STOP)
        {
            if (g_th_stop <= 250) g_th_stop += 5; else g_th_stop = 0;
            g_light_flash_until = Task_GetMs() + 2000;
        }
    }
}
