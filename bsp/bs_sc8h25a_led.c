#include "bs_sc8h25a_led.h"

/* red P0.0, green P0.1, blue P0.2, white P3.4; all active low */

void LED_Init(void)
{
    GPIO_SET_MODE(P0M1, P0M0, 0x07, GPIO_PP);   /* P0.0~P0.2 push-pull */
    GPIO_SET_MODE(P3M1, P3M0, 0x10, GPIO_PP);   /* P3.4 push-pull       */
    LED_AllOff();
}

void LED_Control(u8 color, u8 state)
{
    switch (color)
    {
        case LED_RED:   P00 = state; break;
        case LED_GREEN: P01 = state; break;
        case LED_BLUE:  P02 = state; break;
        case LED_WHITE: P34 = state; break;
        default: break;
    }
}

void LED_Solo(u8 color)
{
    LED_AllOff();
    LED_Control(color, LED_ON);
}

void LED_Toggle(u8 color)
{
    switch (color)
    {
        case LED_RED:   P00 = !P00; break;
        case LED_GREEN: P01 = !P01; break;
        case LED_BLUE:  P02 = !P02; break;
        case LED_WHITE: P34 = !P34; break;
        default: break;
    }
}

void LED_AllOff(void)
{
    P00 = LED_OFF; P01 = LED_OFF; P02 = LED_OFF; P34 = LED_OFF;
}

void LED_AllOn(void)
{
    P00 = LED_ON; P01 = LED_ON; P02 = LED_ON; P34 = LED_ON;
}
