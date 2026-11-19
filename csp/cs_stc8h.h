#ifndef __CS_STC8H_H__
#define __CS_STC8H_H__

#include "STC8H.H"

/* ---- basic types ---- */
typedef unsigned char  u8;
typedef unsigned int   u16;
typedef unsigned long  u32;
typedef signed   char  s8;
typedef signed   int   s16;
typedef signed   long  s32;

#ifndef NULL
#define NULL ((void *)0)
#endif

/* ---- system clock (STC8H, run at 24MHz) ---- */
#define MAIN_Fosc   24000000UL

/* ---- IE2 interrupt-enable bits (STC8H.H does not define them) ---- */
#define ET2   0x04
#define ET3   0x20
#define ET4   0x40

/* ---- GPIO pin modes (STC8H: PxM1:PxM0) ---- */
#define GPIO_QUASI   0u   /* quasi-bidirectional (weak pull-up)  */
#define GPIO_PP      1u   /* push-pull output                     */
#define GPIO_HIZ     2u   /* high-impedance input                 */
#define GPIO_OD      3u   /* open-drain output                    */

/* set one GPIO pin mode: mask selects the pin bit(s) */
#define GPIO_SET_MODE(REG_M1, REG_M0, MASK, MODE) do { \
    if ((MODE) & 1u) (REG_M0) |= (u8)(MASK); else (REG_M0) &= (u8)(~(MASK)); \
    if ((MODE) & 2u) (REG_M1) |= (u8)(MASK); else (REG_M1) &= (u8)(~(MASK)); \
} while (0)

#endif /* __CS_STC8H_H__ */
