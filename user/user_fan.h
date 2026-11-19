#ifndef __USER_FAN_H__
#define __USER_FAN_H__

#include "cs_stc8h.h"
#include "cs_stc8h_eeprom.h"
#include "bs_sc8h25a_key.h"

/* ---- 主模式 (KEY1+KEY2 循环) ---- */
#define MODE_MANUAL   0
#define MODE_AUTO     1
#define MODE_SOFT     2
#define MODE_POT      3
#define MODE_LIGHT    4
#define MODE_SWING    5
#define MODE_STORE    6
#define MODE_NUM      7

/* ---- 运行模式 (电机按此控速) ---- */
#define RUN_MANUAL    0
#define RUN_AUTO      1
#define RUN_SOFT      2
#define RUN_POT       3

/* ---- 光控子模式 ---- */
#define LIGHT_SUB_FILL  0
#define LIGHT_SUB_STOP  1
#define LIGHT_SUB_OFF   2

/* ---- 默认参数 ---- */
#define TH_TEMP_DEFAULT  26
#define TH_TEMP_MIN      16
#define TH_TEMP_MAX      40
#define TH_FILL_DEFAULT  20
#define TH_STOP_DEFAULT  25
#define THETA_DEFAULT    45
#define THETA_MIN        5
#define THETA_MAX        90
#define SWING_STEP       2

/* ---- EEPROM 布局 ---- */
#define EEP_MAGIC        0xAA
#define EEP_MAGIC_ADDR   (EEPROM_BASE_ADDR + 0)
#define EEP_TH_TEMP      (EEPROM_BASE_ADDR + 1)
#define EEP_LIGHT_SUB    (EEPROM_BASE_ADDR + 2)
#define EEP_TH_FILL      (EEPROM_BASE_ADDR + 3)
#define EEP_TH_STOP      (EEPROM_BASE_ADDR + 4)
#define EEP_SWING_EN     (EEPROM_BASE_ADDR + 5)
#define EEP_THETA        (EEPROM_BASE_ADDR + 6)

/* ---- 全局状态 ---- */
extern u8    g_mode;
extern u8    g_run_mode;
extern u8    g_gear;
extern u8    g_req_duty;
extern u8    g_duty;
extern float g_temp;
extern bit   g_temp_valid;
extern u8    g_th_temp;
extern u8    g_light_sub;
extern u8    g_th_fill;
extern u8    g_th_stop;
extern bit   g_swing_en;
extern u8    g_theta;
extern u8    g_servo_angle;
extern u8    g_pot_adc8;
extern bit   g_pot_show_duty;
extern u16   g_dist;
extern bit   g_dist_valid;
extern u8    g_soft_item;
extern u8    g_light_adc8;
extern u32   g_light_flash_until;
extern u32   g_store_save_until;

/* ---- 任务回调 ---- */
void Fan_KeyTask(void);
void Fan_PotTask(void);
void Fan_SoftTask(void);
void Fan_LightTask(void);
void Fan_SwingTask(void);
void Fan_WsTask(void);
void Fan_DispTask(void);
void Fan_TempTask(void);
void Fan_AutoTask(void);

/* ---- 接口 ---- */
void Fan_ModeLed(u8 mode);
void Fan_SetReqDuty(u8 duty);
void Fan_ResolveDuty(void);
void Fan_AutoUpdate(void);
void Fan_SoftUpdate(void);
void Fan_PotUpdate(void);
void Fan_AutoKey(u8 key);
void Fan_SoftKey(u8 key);
void Fan_PotKey(u8 key);
void Fan_LightKey(u8 key);
void Fan_SwingKey(u8 key);
void Fan_EepromKey(u8 key);
void Fan_EepromLoad(void);
void Fan_EepromSave(void);

#endif
