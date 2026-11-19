#include "cs_stc8h.h"
#include "task.h"

extern void User_Init(void);

void main(void)
{
    P_SW2 |= 0x80;      /* allow extended-SFR access (PWM/ADC/PU/... ) */

    User_Init();
    Task_Init();        /* T4 1ms tick -> Task_Tick_Cb */
    EA = 1;             /* enable global interrupt    */

    while (1)
    {
        Task_Run_Pending();
    }
}
