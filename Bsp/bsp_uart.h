/**
 * @file bsp_uart.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 */
#ifndef __BSP_UART_H
#define __BSP_UART_h

#include "driverlib.h"

extern int16_t pos_x,pos_y,pos_z;

void bsp_uart_init(void);
void bsp_uart0_transmitString_DMA(uint8_t* String, uint16_t len);
void bsp_uart_analysis(uint8_t data);

#endif
