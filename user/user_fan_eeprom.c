#include "user_fan.h"
#include "cs_stc8h_eeprom.h"
#include "task.h"

void Fan_EepromLoad(void)
{
    u8 magic, t;
    magic = cs_eeprom_read_byte(EEP_MAGIC_ADDR);
    if (magic != EEP_MAGIC) goto use_defaults;

    t = cs_eeprom_read_byte(EEP_TH_TEMP);
    g_th_temp = (t >= TH_TEMP_MIN && t <= TH_TEMP_MAX) ? t : TH_TEMP_DEFAULT;

    t = cs_eeprom_read_byte(EEP_LIGHT_SUB);
    g_light_sub = (t <= LIGHT_SUB_OFF) ? t : LIGHT_SUB_FILL;

    g_th_fill = cs_eeprom_read_byte(EEP_TH_FILL);
    g_th_stop = cs_eeprom_read_byte(EEP_TH_STOP);
    g_swing_en = cs_eeprom_read_byte(EEP_SWING_EN) ? 1 : 0;

    t = cs_eeprom_read_byte(EEP_THETA);
    g_theta = (t >= THETA_MIN && t <= THETA_MAX) ? t : THETA_DEFAULT;
    return;

use_defaults:
    g_th_temp = TH_TEMP_DEFAULT;
    g_light_sub = LIGHT_SUB_FILL;
    g_th_fill = TH_FILL_DEFAULT;
    g_th_stop = TH_STOP_DEFAULT;
    g_swing_en = 0;
    g_theta = THETA_DEFAULT;
}

void Fan_EepromSave(void)
{
    cs_eeprom_erase_sector(EEP_MAGIC_ADDR);
    cs_eeprom_write_byte(EEP_TH_TEMP,   g_th_temp);
    cs_eeprom_write_byte(EEP_LIGHT_SUB, g_light_sub);
    cs_eeprom_write_byte(EEP_TH_FILL,   g_th_fill);
    cs_eeprom_write_byte(EEP_TH_STOP,   g_th_stop);
    cs_eeprom_write_byte(EEP_SWING_EN,  g_swing_en ? 1 : 0);
    cs_eeprom_write_byte(EEP_THETA,     g_theta);
    cs_eeprom_write_byte(EEP_MAGIC_ADDR, EEP_MAGIC);
}

void Fan_EepromKey(u8 key)
{
    if (key == KEY_VALUE_1)
    {
        Fan_EepromLoad();
        Fan_ResolveDuty();
    }
    else if (key == KEY_VALUE_2)
    {
        Fan_EepromSave();
        g_store_save_until = Task_GetMs() + 1000;
    }
}
