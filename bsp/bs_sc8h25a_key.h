#ifndef __BS_SC8H25A_KEY_H__
#define __BS_SC8H25A_KEY_H__

#include "cs_stc8h.h"

#define KEY_VALUE_NONE   0
#define KEY_VALUE_1      1
#define KEY_VALUE_2      2
#define KEY_VALUE_BOTH   3

void KEY_Init(void);
void KEY_Scan(void);      /* call periodically, ~10~20ms */
u8   KEY_GetNum(void);    /* returns one-shot key value    */

#endif
