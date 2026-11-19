#include "user_fan.h"
#include "task.h"
#include "m_motor.h"
#include "m_servo.h"
#include "m_sonar.h"
#include "bs_sc8h25a_temp_ds18b20.h"
#include "cs_stc8h_adc.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_led_ws2812b.h"
#include "bs_sc8h25a_seg_tm1650.h"

u8    g_mode = MODE_MANUAL;
u8    g_run_mode = RUN_MANUAL;
u8    g_gear = 0;
u8    g_req_duty = 0;
u8    g_duty = 0;
float g_temp = 0.0F;
bit   g_temp_valid = 0;
u8    g_th_temp = TH_TEMP_DEFAULT;
u8    g_light_sub = LIGHT_SUB_FILL;
u8    g_th_fill = TH_FILL_DEFAULT;
u8    g_th_stop = TH_STOP_DEFAULT;
bit   g_swing_en = 0;
u8    g_theta = THETA_DEFAULT;
u8    g_servo_angle = 90;
u8    g_pot_adc8 = 0;
bit   g_pot_show_duty = 1;
u16   g_dist = 0;
bit   g_dist_valid = 0;
u8    g_soft_item = 0;
u8    g_light_adc8 = 0;
u32   g_light_flash_until = 0;
u32   g_store_save_until = 0;

void Fan_ResolveDuty(void)
{
    u8 d = g_req_duty;
    if (g_light_sub == LIGHT_SUB_STOP && g_light_adc8 >= g_th_stop) d = 0;
    g_duty = d;
    Motor_SetSpeed(d);
}

void Fan_SetReqDuty(u8 duty)
{
    g_req_duty = duty;
    Fan_ResolveDuty();
}

void User_Init(void)
{
    Motor_Init();
    Servo_Init();
    Sonar_Init();
    DS18B20_Init();
    adc_init(ADC_P11, 15, 12);
    KEY_Init();
    LED_Init();
    WS2812B_Init();
    SegTM1650_Init();

    Fan_EepromLoad();

    g_mode = MODE_MANUAL;
    g_run_mode = RUN_MANUAL;
    Fan_ModeLed(MODE_MANUAL);
    Motor_Stop();
    Servo_SetAngle(90);
    Fan_ResolveDuty();
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,   Fan_KeyTask),
    TASK_ENTRY(20,   Fan_PotTask),
    TASK_ENTRY(80,   Fan_SoftTask),
    TASK_ENTRY(150,  Fan_LightTask),
    TASK_ENTRY(80,   Fan_SwingTask),
    TASK_ENTRY(100,  Fan_WsTask),
    TASK_ENTRY(200,  Fan_DispTask),
    TASK_ENTRY(1000, Fan_TempTask),
    TASK_ENTRY(1000, Fan_AutoTask),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);
