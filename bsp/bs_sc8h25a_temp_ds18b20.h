#ifndef __BS_SC8H25A_TEMP_DS18B20_H__
#define __BS_SC8H25A_TEMP_DS18B20_H__

#include "cs_stc8h.h"

#define TEMP_INVALID   255.0F

void  DS18B20_Init(void);
float DS18B20_ReadTemperature(void);   /* degrees C; TEMP_INVALID on error */

#endif
