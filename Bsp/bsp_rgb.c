/**
 * @file bsp_rgb.c
 * @brief 板载RGB支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "bsp_rgb.h"
#include "driverlib.h"
//RGB灯引脚定义
#define R_GPIO_PORT GPIO_PORT_P2
#define G_GPIO_PORT GPIO_PORT_P2
#define B_GPIO_PORT GPIO_PORT_P2
#define R_GPIO_PIN GPIO_PIN0
#define G_GPIO_PIN GPIO_PIN1
#define B_GPIO_PIN GPIO_PIN2
//RGB灯颜色代码
#define R_SET 0X01
#define G_SET 0X02
#define B_SET 0X04

/**
 * @brief RGB灯GPIO初始化
 */
void bsp_rgb_init(void)
{
    //配置引脚为输出
    ROM_GPIO_setAsOutputPin(R_GPIO_PORT,R_GPIO_PIN);
    ROM_GPIO_setAsOutputPin(G_GPIO_PORT,G_GPIO_PIN);
    ROM_GPIO_setAsOutputPin(B_GPIO_PORT,B_GPIO_PIN);
    //关闭所有灯光
    ROM_GPIO_setOutputLowOnPin(R_GPIO_PORT,R_GPIO_PIN);
    ROM_GPIO_setOutputLowOnPin(G_GPIO_PORT,G_GPIO_PIN);
    ROM_GPIO_setOutputLowOnPin(B_GPIO_PORT,B_GPIO_PIN);
}

/**
 * @brief 控制RGB亮起
 * @param color 颜色代码
 */
void bsp_rgb_set(uint8_t color)
{
    //先关闭输出
    ROM_GPIO_setOutputLowOnPin(R_GPIO_PORT,R_GPIO_PIN);
    ROM_GPIO_setOutputLowOnPin(G_GPIO_PORT,G_GPIO_PIN);
    ROM_GPIO_setOutputLowOnPin(B_GPIO_PORT,B_GPIO_PIN);
    //根据颜色码输出
    if(color & 0x01) ROM_GPIO_setOutputHighOnPin(R_GPIO_PORT,R_GPIO_PIN);
    if((color>>1) & 0x01) ROM_GPIO_setOutputHighOnPin(G_GPIO_PORT,G_GPIO_PIN);
    if((color>>2) & 0x01) ROM_GPIO_setOutputHighOnPin(B_GPIO_PORT,B_GPIO_PIN);
}
