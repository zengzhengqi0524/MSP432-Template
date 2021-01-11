/**
 * @file bsp_spi.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 */
#ifndef __BSP_SPI_H
#define __BSP_SPI_H

void bsp_spi_init(void);
void bsp_spi0_send_byte(char data);
uint8_t bsp_spi0_rw(uint8_t data);

#endif
