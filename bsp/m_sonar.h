#ifndef __M_SONAR_H__
#define __M_SONAR_H__

#include "cs_stc8h.h"

#define SONAR_VALID_MAX        999
#define SONAR_ERR_NO_ECHO      1000
#define SONAR_ERR_TIMEOUT      2000
#define SONAR_ERR_OVERFLOW     3000

void Sonar_Init(void);
u16  Sonar_GetDistanceCm(void);
u8   Sonar_IsValid(u16 dist);

#endif
