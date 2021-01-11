/**
 * @file bsp_clock.c
 * @brief 时钟配置板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "bsp_clock.h"
#include "cs.h"
#include "rom_map.h"
#include "rom.h"

/**
 * @brief 系统时钟初始化
 * MCLK = SMCLK = 48M
 */
void bsp_clock_init(void)
{
    //配置外部时钟源输入
    ROM_GPIO_setAsPeripheralModuleFunctionOutputPin(GPIO_PORT_PJ,GPIO_PIN3 | GPIO_PIN2,GPIO_PRIMARY_MODULE_FUNCTION);
    MAP_CS_setExternalClockSourceFrequency(3200,48000000);
    ROM_PCM_setCoreVoltageLevel(PCM_VCORE1);
    ROM_FlashCtl_setWaitState(FLASH_BANK0,3);
    ROM_FlashCtl_setWaitState(FLASH_BANK1,3);
    MAP_CS_startHFXT(false);
    //配置MCLK,SMCLK,HSMCLK,ACLK=48MHz
    ROM_CS_initClockSignal(CS_MCLK,CS_HFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
    ROM_CS_initClockSignal(CS_SMCLK,CS_HFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
    ROM_CS_initClockSignal(CS_HSMCLK,CS_HFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
	ROM_CS_initClockSignal(CS_ACLK,CS_HFXTCLK_SELECT,CS_CLOCK_DIVIDER_1);
}
