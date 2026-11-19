#include "user_fan.h"
#include "bs_sc8h25a_led_ws2812b.h"

static u8 s_phase = 0;

static void flow(u8 bright)
{
    u8 i;
    for (i = 0; i < WS2812B_LED_NUM; i++)
    {
        switch ((i + s_phase) & 3)
        {
            case 0: WS2812B_SetPixel(i, bright, 0, 0); break;
            case 1: WS2812B_SetPixel(i, 0, bright, 0); break;
            case 2: WS2812B_SetPixel(i, 0, 0, bright); break;
            default:WS2812B_SetPixel(i, bright, bright, bright); break;
        }
    }
    s_phase++;
    if (s_phase >= 4) s_phase = 0;
}

void Fan_WsTask(void)
{
    u8 d = g_duty;
    if (g_mode == MODE_LIGHT)
    {
        WS2812B_Clear();
        return;
    }
    if (g_mode == MODE_STORE)
    {
        flow(64);
        WS2812B_Show();
        return;
    }
    if (d == 0)
    {
        flow(255);
    }
    else if (d <= 30)
    {
        WS2812B_SetColorAll(255, 128, 0);
    }
    else if (d <= 60)
    {
        WS2812B_SetColorAll(0, 255, 80);
    }
    else
    {
        WS2812B_SetColorAll(0, 200, 255);
    }
    WS2812B_Show();
}
