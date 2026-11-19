#include "user_fan.h"
#include "bs_sc8h25a_led.h"

static u8 code DUTY_TAB[4] = {0, 30, 60, 90};

void Fan_ModeLed(u8 mode)
{
    LED_Control(LED_RED,   LED_OFF);
    LED_Control(LED_GREEN, LED_OFF);
    LED_Control(LED_BLUE,  LED_OFF);
    switch (mode)
    {
        case MODE_MANUAL: LED_Control(LED_RED, LED_ON); break;
        case MODE_AUTO:   LED_Control(LED_BLUE, LED_ON); break;
        case MODE_SOFT:   LED_Control(LED_GREEN, LED_ON); break;
        case MODE_POT:    LED_Control(LED_RED, LED_ON); LED_Control(LED_GREEN, LED_ON); break;
        case MODE_LIGHT:  LED_Control(LED_RED, LED_ON); LED_Control(LED_GREEN, LED_ON); LED_Control(LED_BLUE, LED_ON); break;
        case MODE_SWING:  LED_Control(LED_GREEN, LED_ON); LED_Control(LED_BLUE, LED_ON); break;
        case MODE_STORE:  LED_Control(LED_RED, LED_ON); LED_Control(LED_BLUE, LED_ON); break;
        default: break;
    }
}

static void manual_key(u8 key)
{
    if (key == KEY_VALUE_1)
    {
        if (g_gear > 0) g_gear--;
        Fan_SetReqDuty(DUTY_TAB[g_gear]);
    }
    else if (key == KEY_VALUE_2)
    {
        if (g_gear < 3) g_gear++;
        Fan_SetReqDuty(DUTY_TAB[g_gear]);
    }
}

static void switch_mode(void)
{
    g_mode++;
    if (g_mode >= MODE_NUM) g_mode = 0;
    Fan_ModeLed(g_mode);
    switch (g_mode)
    {
        case MODE_MANUAL: g_run_mode = RUN_MANUAL; Fan_SetReqDuty(DUTY_TAB[g_gear]); break;
        case MODE_AUTO:   g_run_mode = RUN_AUTO;   Fan_AutoUpdate(); break;
        case MODE_SOFT:   g_run_mode = RUN_SOFT;   Fan_SoftUpdate(); break;
        case MODE_POT:    g_run_mode = RUN_POT;    Fan_PotUpdate(); break;
        default: break;
    }
}

void Fan_KeyTask(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();
    if (key == KEY_VALUE_BOTH) { switch_mode(); return; }
    switch (g_mode)
    {
        case MODE_MANUAL: manual_key(key); break;
        case MODE_AUTO:   Fan_AutoKey(key); break;
        case MODE_SOFT:   Fan_SoftKey(key); break;
        case MODE_POT:    Fan_PotKey(key); break;
        case MODE_LIGHT:  Fan_LightKey(key); break;
        case MODE_SWING:  Fan_SwingKey(key); break;
        case MODE_STORE:  Fan_EepromKey(key); break;
        default: break;
    }
}
