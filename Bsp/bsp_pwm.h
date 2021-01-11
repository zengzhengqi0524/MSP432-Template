/**
 * @file bsp_pwm.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 */
#ifndef __BSP_PWM_H
#define __BSP_PWM_H
#include "driverlib.h"

void bsp_pwm_init(void);
void bsp_pwm_set(uint8_t pwmnum, uint32_t setnum);

#endif
