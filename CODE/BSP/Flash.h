#ifndef _Flash_H
#define _Flash_H

#include "main.h"

void Flash_Cal_Write(float zero_angle);
uint8_t Flash_Cal_Read(float *zero_angle);

#endif
