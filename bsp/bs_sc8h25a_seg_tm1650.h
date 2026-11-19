#ifndef __BS_SC8H25A_SEG_TM1650_H__
#define __BS_SC8H25A_SEG_TM1650_H__

#include "cs_stc8h.h"

void SegTM1650_Init(void);
void SegTM1650_Clear(void);
void SegTM1650_DisplayDash(void);        /* "----" */
void SegTM1650_DisplayNum(u16 num);                   /* 0~9999, no leading zero */
void SegTM1650_DisplayFloat(float f);                 /* one decimal, auto point */
void SegTM1650_DisplayBit(u8 seg_code, u8 digit, bit point);  /* digit 1..4 */
u8   SegTM1650_SegCode(u8 ch);
void SegTM1650_DisplayChar(u8 digit, u8 ch, bit point);

#endif
