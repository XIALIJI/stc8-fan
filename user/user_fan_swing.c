#include "user_fan.h"
#include "m_servo.h"

static bit s_swing_dir = 1;

void Fan_SwingTask(void)
{
    if (!g_swing_en)
    {
        if (g_servo_angle != 90) { g_servo_angle = 90; Servo_SetAngle(90); }
        return;
    }
    if (s_swing_dir)
    {
        if (g_servo_angle < 90 + g_theta - SWING_STEP) g_servo_angle += SWING_STEP;
        else { g_servo_angle = 90 + g_theta; s_swing_dir = 0; }
    }
    else
    {
        if (g_servo_angle > 90 - g_theta + SWING_STEP) g_servo_angle -= SWING_STEP;
        else { g_servo_angle = 90 - g_theta; s_swing_dir = 1; }
    }
    Servo_SetAngle(g_servo_angle);
}

void Fan_SwingKey(u8 key)
{
    if (key == KEY_VALUE_1)
    {
        g_swing_en = !g_swing_en;
        if (!g_swing_en) { g_servo_angle = 90; Servo_SetAngle(90); }
    }
    else if (key == KEY_VALUE_2)
    {
        g_theta += 5;
        if (g_theta > THETA_MAX) g_theta = THETA_MIN;
    }
}
