#ifndef __BS_SC8H25A_LED_WS2812B_H__
#define __BS_SC8H25A_LED_WS2812B_H__

#include "cs_stc8h.h"

#define WS2812B_LED_NUM   4

void WS2812B_Init(void);
void WS2812B_SetPixel(u8 index, u8 r, u8 g, u8 b);
void WS2812B_SetColorAll(u8 r, u8 g, u8 b);
void WS2812B_Show(void);
void WS2812B_Clear(void);

#endif
