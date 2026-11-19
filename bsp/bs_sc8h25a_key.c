#include "bs_sc8h25a_key.h"

/* KEY1 = P3.2, KEY2 = P3.3, active low, non-blocking 4-state machine */

static u8 s_raw_last   = 0;   /* last raw scan        */
static u8 s_stable     = 0;   /* debounced value      */
static u8 s_stable_cnt = 0;
static u8 s_prev_state = 0;   /* previous debounced   */
static u8 s_value      = KEY_VALUE_NONE;  /* latch for GetNum  */

void KEY_Init(void)
{
    GPIO_SET_MODE(P3M1, P3M0, 0x0C, GPIO_QUASI);   /* P3.2/P3.3 quasi      */
    P_SW2 |= 0x80;
    P3PU |= 0x0C;                                  /* internal pull-up      */
    s_raw_last = 0; s_stable = 0; s_stable_cnt = 0;
    s_prev_state = 0; s_value = KEY_VALUE_NONE;
}

void KEY_Scan(void)
{
    u8 now = 0;
    if (P32 == 0) now |= 0x01;
    if (P33 == 0) now |= 0x02;

    if (now == s_raw_last) { if (s_stable_cnt < 2) s_stable_cnt++; }
    else s_stable_cnt = 0;
    s_raw_last = now;

    if (s_stable_cnt >= 1) s_stable = now;       /* debounced (~2 scans) */

    /* one-shot on no-key -> key transition */
    if (s_stable != 0 && s_prev_state == 0)
    {
        s_value = s_stable;    /* 1=K1, 2=K2, 3=both */
    }
    s_prev_state = s_stable;
}

u8 KEY_GetNum(void)
{
    u8 r = s_value;
    s_value = KEY_VALUE_NONE;
    return r;
}
