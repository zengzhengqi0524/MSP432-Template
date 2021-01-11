/**
 * @file bsp_flash.c
 * @brief flash读写板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "gpio.h"
#include "flash.h"
#include "rom.h"
#include "rom_map.h"

volatile int CALIBRATION_START=0x0003F000;
//预留的用来保存数据到Flsh的变量
//设计用来保存pox_x,pos_y,pos_z,pitch_pwm_width,yaw_pwm_width
uint8_t flash_save_data[5] = {0x01,0x02,0x03,0x04,0x05};
//用于保存从Flash中读取出的数据
uint8_t flash_read_data[5];

uint8_t bsp_flash_write(void)
{
    //解锁Flash
    ROM_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
    //擦除Falsh
    if(!ROM_FlashCtl_eraseSector(CALIBRATION_START))
        while(1);
    //写入Falsh
    if(!ROM_FlashCtl_programMemory(flash_save_data,(void*) CALIBRATION_START, 5))
        while(1);
    //上锁Falsh
    ROM_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
    return 1;
}

uint8_t bsp_flash_read(void)
{
    int i = 0;
    //解锁Flash
    ROM_FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
    ROM_FlashCtl_setReadMode(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
    //读取falsh
    for(i=0; i<5; i++)
    {
        flash_read_data[i] = *(volatile uint8_t*)CALIBRATION_START;
        CALIBRATION_START ++;
    }
    //上锁Falsh
    ROM_FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1,FLASH_SECTOR31);
    //恢复地址
    CALIBRATION_START=0x0003F000;
    return 1;
}
