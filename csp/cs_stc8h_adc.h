#ifndef __CS_STC8H_ADC_H__
#define __CS_STC8H_ADC_H__

#include "cs_stc8h.h"

#define ADC_P10   0    /* P1.0 : light sensor   */
#define ADC_P11   1    /* P1.1 : potentiometer  */

void adc_init(u8 ch, u8 clk, u8 res);     /* clk:0~15 speed, res:12=right-align */
u16  adc_read(u8 ch);                     /* 12-bit raw (0~4095)               */
u16  adc_read_avg(u8 ch, u8 n);           /* average of n samples              */
u8   adc_read_h8(u8 ch);                  /* high 8 bits (0~255)               */
u8   adc_read_avg_h8(u8 ch, u8 n);        /* averaged high 8 bits              */

#endif
