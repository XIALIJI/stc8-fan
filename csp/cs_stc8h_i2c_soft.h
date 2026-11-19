#ifndef __CS_STC8H_I2C_SOFT_H__
#define __CS_STC8H_I2C_SOFT_H__

#include "cs_stc8h.h"

/* software I2C master on P1.5(SCL) / P1.4(SDA), used by TM1650 */

void cs_i2c_soft_init(void);
void cs_i2c_start(void);
void cs_i2c_stop(void);
void cs_i2c_send_byte(u8 dat);
u8   cs_i2c_read_byte(u8 ack);      /* ack:1=send ACK, 0=send NACK */
u8   cs_i2c_wait_ack(void);         /* returns 0 = ACK received    */
void cs_i2c_ack(void);
void cs_i2c_nack(void);

#endif
