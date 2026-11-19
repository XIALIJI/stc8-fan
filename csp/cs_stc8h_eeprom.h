#ifndef __CS_STC8H_EEPROM_H__
#define __CS_STC8H_EEPROM_H__

#include "cs_stc8h.h"

/* STC8H8K64U data-flash (IAP-EEPROM). Configure EEPROM >= 1KB in STC-ISP
 * so that 0xF000 is inside the data-flash region. erase granularity: 512B. */
#define EEPROM_BASE_ADDR   0xF000UL

u8   cs_eeprom_read_byte(u16 addr);
void cs_eeprom_write_byte(u16 addr, u8 dat);
void cs_eeprom_erase_sector(u16 addr);

#endif
