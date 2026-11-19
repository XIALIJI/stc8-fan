#ifndef __BS_SC8H25A_LED_H__
#define __BS_SC8H25A_LED_H__

#include "cs_stc8h.h"

#define LED_RED     0
#define LED_GREEN   1
#define LED_BLUE    2
#define LED_WHITE   3

#define LED_ON      0    /* active low */
#define LED_OFF     1

void LED_Init(void);
void LED_Control(u8 color, u8 state);
void LED_Solo(u8 color);
void LED_Toggle(u8 color);
void LED_AllOff(void);
void LED_AllOn(void);

#endif
