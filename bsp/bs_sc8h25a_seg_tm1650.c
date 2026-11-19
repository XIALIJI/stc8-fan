#include "bs_sc8h25a_seg_tm1650.h"
#include "cs_stc8h_i2c_soft.h"

/* TM1650 4-digit display via software I2C (P1.4 SDA / P1.5 SCL).
 * digit1 = rightmost (units). Verify command bytes against the module on board. */

#define TM1650_CMD_DATA    0x48
#define TM1650_ADDR_DIG1   0x68
#define TM1650_CMD_CTRL    0x44

#define SEG_DP             0x80

static u8 s_buf[4];
static u8 s_dp = 0;

static u8 code SEG_NUM[10] =
{
    0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F
};

static void tm_refresh(void)
{
    u8 i;
    cs_i2c_start(); cs_i2c_send_byte(TM1650_CMD_DATA); cs_i2c_stop();
    cs_i2c_start(); cs_i2c_send_byte(TM1650_ADDR_DIG1);
    for (i = 0; i < 4; i++)
    {
        u8 s = s_buf[i];
        if (s_dp & (u8)(1 << i)) s |= SEG_DP;
        cs_i2c_send_byte(s);
    }
    cs_i2c_stop();
}

void SegTM1650_Init(void)
{
    cs_i2c_soft_init();
    s_dp = 0;
    SegTM1650_Clear();
    cs_i2c_start(); cs_i2c_send_byte(TM1650_CMD_DATA); cs_i2c_stop();
    cs_i2c_start(); cs_i2c_send_byte(TM1650_CMD_CTRL); cs_i2c_stop();
}

void SegTM1650_Clear(void)
{
    u8 i;
    for (i = 0; i < 4; i++) s_buf[i] = 0x00;
    s_dp = 0;
    tm_refresh();
}

void SegTM1650_DisplayDash(void)
{
    u8 i;
    s_dp = 0;
    for (i = 0; i < 4; i++) s_buf[i] = 0x40;
    tm_refresh();
}

void SegTM1650_DisplayNum(u16 num)
{
    if (num > 9999) num = 9999;
    s_buf[0] = SEG_NUM[num % 10];
    s_buf[1] = (num >= 10)   ? SEG_NUM[(num / 10) % 10]   : 0x00;
    s_buf[2] = (num >= 100)  ? SEG_NUM[(num / 100) % 10]  : 0x00;
    s_buf[3] = (num >= 1000) ? SEG_NUM[(num / 1000) % 10] : 0x00;
    s_dp = 0;
    tm_refresh();
}

void SegTM1650_DisplayFloat(float f)
{
    u16 v;
    if (f < 0.0F) f = 0.0F;
    if (f > 999.9F) f = 999.9F;
    v = (u16)(f * 10.0F + 0.5F);
    s_buf[0] = SEG_NUM[v % 10];
    s_buf[1] = SEG_NUM[(v / 10) % 10];
    s_buf[2] = SEG_NUM[(v / 100) % 10];
    s_buf[3] = SEG_NUM[(v / 1000) % 10];
    s_dp = 0x01;
    tm_refresh();
}

u8 SegTM1650_SegCode(u8 ch)
{
    if (ch >= '0' && ch <= '9') return SEG_NUM[ch - '0'];
    switch (ch)
    {
        case '-': return 0x40;
        case ' ': return 0x00;
        case 'A': return 0x77;
        case 'b': return 0x7C;
        case 'C': return 0x39;
        case 'd': return 0x5E;
        case 'E': return 0x79;
        case 'F': return 0x71;
        case 'H': return 0x76;
        case 'L': return 0x38;
        case 'o': return 0x5C;
        case 'O': return 0x3F;
        case 'P': return 0x73;
        case 'S': return 0x6D;
        case 'U': return 0x3E;
        case 'V': return 0x3E;
        case 't': return 0x78;
        case 'n': return 0x54;
        case 'r': return 0x50;
        default: return 0x00;
    }
}

void SegTM1650_DisplayBit(u8 seg_code, u8 digit, bit point)
{
    if (digit < 1 || digit > 4) return;
    s_buf[digit - 1] = seg_code & 0x7F;
    if (point) s_dp |= (u8)(1 << (digit - 1));
    else       s_dp &= (u8)~(1 << (digit - 1));
    tm_refresh();
}

void SegTM1650_DisplayChar(u8 digit, u8 ch, bit point)
{
    SegTM1650_DisplayBit(SegTM1650_SegCode(ch), digit, point);
}
