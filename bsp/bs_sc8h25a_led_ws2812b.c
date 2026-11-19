#include "bs_sc8h25a_led_ws2812b.h"
#include <intrins.h>

/* WS2812B on P0.3, 4 LEDs, GRB byte order. Bit-bang timing is
 * cycle-sensitive (T0H/T0L/T1H/T1L ~ 0.35us/0.8us/0.7us/0.6us).
 * Verify the colors on board; tune the nop counts if shifted. */

#define WS_PIN  P03

static u8 ws_buf[12];   /* GRB: [i*3+0]=G, +1=R, +2=B */

static void ws_nop(u8 n)
{
    while (n--) _nop_();
}

static void ws_send_byte(u8 dat)
{
    u8 i;
    for (i = 0; i < 8; i++)
    {
        if (dat & 0x80)
        {
            WS_PIN = 1; ws_nop(14); WS_PIN = 0; ws_nop(11);
        }
        else
        {
            WS_PIN = 1; ws_nop(5);  WS_PIN = 0; ws_nop(16);
        }
        dat <<= 1;
    }
}

void WS2812B_Init(void)
{
    GPIO_SET_MODE(P0M1, P0M0, 0x08, GPIO_PP);   /* P0.3 push-pull */
    WS2812B_Clear();
}

void WS2812B_SetPixel(u8 index, u8 r, u8 g, u8 b)
{
    if (index >= WS2812B_LED_NUM) index = WS2812B_LED_NUM - 1;
    ws_buf[index * 3 + 0] = g;
    ws_buf[index * 3 + 1] = r;
    ws_buf[index * 3 + 2] = b;
}

void WS2812B_SetColorAll(u8 r, u8 g, u8 b)
{
    u8 i;
    for (i = 0; i < WS2812B_LED_NUM; i++)
    {
        WS2812B_SetPixel(i, r, g, b);
    }
}

void WS2812B_Show(void)
{
    u8 i;
    bit ea_save;
    ea_save = EA;
    EA = 0;
    for (i = 0; i < 12; i++) ws_send_byte(ws_buf[i]);
    WS_PIN = 0;
    ws_nop(255); ws_nop(255); ws_nop(255);   /* >50us reset */
    EA = ea_save;
}

void WS2812B_Clear(void)
{
    WS2812B_SetColorAll(0, 0, 0);
    WS2812B_Show();
}
