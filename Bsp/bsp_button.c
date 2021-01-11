/**
 * @file bsp_button.c
 * @brief 板载按键支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "gpio.h"
#include "bsp_button.h"
#include "rom_map.h"

/**
 * @brief 板载按键初始化
 * @note P1.1 P1.4
 */
void bsp_button_init(void)
{
    //设置端口,上升沿触发
    ROM_GPIO_setAsInputPinWithPullUpResistor(BUTTON1_GPIO_PORT,BUTTON1_GPIO_PIN);
    ROM_GPIO_setAsInputPinWithPullUpResistor(BUTTON2_GPIO_PORT,BUTTON2_GPIO_PIN);
    //清除中断标志
    ROM_GPIO_clearInterruptFlag(BUTTON1_GPIO_PORT,BUTTON1_GPIO_PIN);
    ROM_GPIO_clearInterruptFlag(BUTTON2_GPIO_PORT,BUTTON2_GPIO_PIN);
    //使能中断
    ROM_GPIO_enableInterrupt(BUTTON1_GPIO_PORT,BUTTON1_GPIO_PIN);
    ROM_GPIO_enableInterrupt(BUTTON2_GPIO_PORT,BUTTON2_GPIO_PIN);
    //优先级0xe0，高三位有效，优先级为7
    ROM_Interrupt_enableInterrupt(INT_PORT1);
    ROM_Interrupt_setPriority(INT_PORT1,0XE0);
}

