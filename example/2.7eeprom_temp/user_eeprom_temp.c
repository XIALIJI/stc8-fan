#include "task.h"
#include "cs_stc8h_eeprom.h"
#include "bs_sc8h25a_key.h"
#include "bs_sc8h25a_led.h"
#include "bs_sc8h25a_seg_tm1650.h"

/* Project 2.7: EEPROM 存温度阈值 + Magic 校验
 * 布局: +0 Magic 0xAA, +1 温度阈值(16~40, 默认26) */

#define MAGIC        0xAA
#define MAGIC_ADDR   (EEPROM_BASE_ADDR + 0)
#define TH_ADDR      (EEPROM_BASE_ADDR + 1)
#define TH_MIN       16
#define TH_MAX       40
#define TH_DEFAULT   26

static u8  s_th = TH_DEFAULT;
static bit s_adjust = 0;
static bit s_magic_ok = 0;
static u8  s_led_state = 0;     /* 0=灭, 1=绿(成功), 2=红(失败), 3=蓝(调节) */
static u32 s_save_until = 0;

static void led_update(void)
{
    LED_Control(LED_RED,   LED_OFF);
    LED_Control(LED_GREEN, LED_OFF);
    LED_Control(LED_BLUE,  LED_OFF);
    if (s_led_state == 1)      LED_Control(LED_GREEN, LED_ON);
    else if (s_led_state == 2) LED_Control(LED_RED,   LED_ON);
    else if (s_led_state == 3) LED_Control(LED_BLUE,  LED_ON);
}

static u8 th_load(void)
{
    u8 magic = cs_eeprom_read_byte(MAGIC_ADDR);
    u8 th = cs_eeprom_read_byte(TH_ADDR);
    if (magic == MAGIC && th >= TH_MIN && th <= TH_MAX)
    {
        s_magic_ok = 1;
        s_th = th;
        return 1;
    }
    s_magic_ok = 0;
    s_th = TH_DEFAULT;
    return 0;
}

static void th_save(void)
{
    cs_eeprom_erase_sector(MAGIC_ADDR);
    cs_eeprom_write_byte(TH_ADDR, s_th);
    cs_eeprom_write_byte(MAGIC_ADDR, MAGIC);
}

static void key_task(void)
{
    u8 key;
    KEY_Scan();
    key = KEY_GetNum();

    if (s_adjust)
    {
        switch (key)
        {
            case KEY_VALUE_1:      /* +1, 40->16 回绕 */
                s_th++;
                if (s_th > TH_MAX) s_th = TH_MIN;
                break;
            case KEY_VALUE_2:      /* -1, 16->40 回绕 */
                if (s_th <= TH_MIN) s_th = TH_MAX; else s_th--;
                break;
            case KEY_VALUE_BOTH:   /* 退出调节(不写EEPROM) */
                s_adjust = 0;
                s_led_state = s_magic_ok ? 1 : 2;
                led_update();
                break;
            default:
                break;
        }
    }
    else
    {
        switch (key)
        {
            case KEY_VALUE_1:      /* 读EEPROM更新显示 */
                s_led_state = th_load() ? 1 : 2;
                led_update();
                break;
            case KEY_VALUE_2:      /* 擦扇区再写阈值+Magic, 成功显SAVE */
                th_save();
                s_magic_ok = 1;
                s_led_state = 1;
                led_update();
                s_save_until = Task_GetMs() + 1000;
                break;
            case KEY_VALUE_BOTH:   /* 进入调节 */
                s_adjust = 1;
                s_led_state = 3;
                led_update();
                break;
            default:
                break;
        }
    }
}

static void seg_show_task(void)
{
    if (s_save_until && (Task_GetMs() < s_save_until))
    {
        SegTM1650_Clear();
        SegTM1650_DisplayChar(4, 'S', 0);
        SegTM1650_DisplayChar(3, 'A', 0);
        SegTM1650_DisplayChar(2, 'U', 0);
        SegTM1650_DisplayChar(1, 'E', 0);
    }
    else
    {
        s_save_until = 0;
        SegTM1650_DisplayNum(s_th);
    }
}

task_t idata s_task_table[] =
{
    TASK_ENTRY(20,  key_task),
    TASK_ENTRY(200, seg_show_task),
};
const u8 s_task_count = sizeof(s_task_table) / sizeof(s_task_table[0]);

void User_Init(void)
{
    SegTM1650_Init();
    KEY_Init();
    LED_Init();
    s_led_state = th_load() ? 1 : 2;
    led_update();
}
