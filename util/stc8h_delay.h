#ifndef __STC8H_DELAY_H__
#define __STC8H_DELAY_H__

#include "cs_stc8h.h"

void Delay_1ms(void);
void Delay_10us(void);
void Delay_ms(u16 ms);
void Delay_us(u8 us);       /* us: counted in 10us steps */

#endif /* __STC8H_DELAY_H__ */
