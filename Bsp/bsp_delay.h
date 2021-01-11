/**
 * @file bsp_delay.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 */
#ifndef __BSP_DELAY_H
#define __BSP_DELAY_H

extern __IO uint32_t uwTick;

void bsp_delay_init(void);
void bsp_delay_IncTick(void);
uint32_t bsp_delay_GetTick(void);
void bsp_delay_ms(uint32_t Delay);
void bsp_delay_us(uint32_t Delay);
#endif
