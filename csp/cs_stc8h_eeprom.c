#include "cs_stc8h_eeprom.h"
#include <intrins.h>

#define IAP_EN   0x80

static void iap_enable(void)
{
    IAP_CONTR = IAP_EN;
    IAP_TPS = (u8)(MAIN_Fosc / 1000000UL);   /* 24 */
}

static void iap_disable(void)
{
    IAP_CONTR = 0;
    IAP_CMD = 0;
    IAP_TRIG = 0;
    IAP_ADDRH = 0xFF;
    IAP_ADDRL = 0xFF;
}

static void iap_trig(void)
{
    bit tmp;
    tmp = EA;
    EA = 0;
    IAP_TRIG = 0x5A;
    IAP_TRIG = 0xA5;
    _nop_();
    _nop_();
    EA = tmp;
}

u8 cs_eeprom_read_byte(u16 addr)
{
    u8 dat;
    iap_enable();
    IAP_CMD = 1;
    IAP_ADDRH = (u8)(addr >> 8);
    IAP_ADDRL = (u8)(addr & 0xFF);
    iap_trig();
    dat = IAP_DATA;
    iap_disable();
    return dat;
}

void cs_eeprom_write_byte(u16 addr, u8 dat)
{
    iap_enable();
    IAP_CMD = 2;
    IAP_ADDRH = (u8)(addr >> 8);
    IAP_ADDRL = (u8)(addr & 0xFF);
    IAP_DATA = dat;
    iap_trig();
    iap_disable();
}

void cs_eeprom_erase_sector(u16 addr)
{
    iap_enable();
    IAP_CMD = 3;
    IAP_ADDRH = (u8)(addr >> 8);
    IAP_ADDRL = (u8)(addr & 0xFF);
    iap_trig();
    iap_disable();
}
