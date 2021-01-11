/**
 * @file app_dt.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-28
 * @copyright Evan-GH
 */
#ifndef __APP_DT_H
#define __APP_DT_H
#include "msp432.h"

void app_dt_ANO_Send_UserData(uint8_t _Frame_ID, float Data1, float Data2, float Data3);

#endif
