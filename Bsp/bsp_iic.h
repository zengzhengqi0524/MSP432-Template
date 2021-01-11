/**
 * @file bsp_iic.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-08
 * @copyright Evan-GH
 */
#ifndef __BSP_IIC_H
#define __BSP_IIC_H

#include "driverlib.h"

//决定启用硬件iic或是软件iic的开关
#define SOFTWARE_IIC 0
#define HARDWARE_IIC 1

typedef enum {
    eUSCI_IDLE = 0,
    eUSCI_SUCCESS = 0,
    eUSCI_BUSY = 1,
    eUSCI_NACK = 2,
    eUSCI_STOP,
    eUSCI_START
} eUSCI_status;

extern volatile eUSCI_status iic_status;
extern uint8_t* pData;

#if HARDWARE_IIC
void bsp_iic_init(void);
bool bsp_iic_write(uint8_t Addr, uint8_t Reg, uint8_t* Data,uint8_t ByteCount);
#endif

#if SOFTWARE_IIC
void bsp_iic_init(void);
#endif

#endif
