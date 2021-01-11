/**
 * @file bsp_led.c
 * @brief 板载LED支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "bsp_led.h"
#include "gpio.h"
#include "rom.h"
#include "rom_map.h"

#define LED_GPIO_PORT GPIO_PORT_P1
#define LED_GPIO_PIN GPIO_PIN0

/**
 * @brief LED端口初始化
 */
void bsp_led_init(void)
{
    //配置P1.1为输出端口
    ROM_GPIO_setAsOutputPin(LED_GPIO_PORT,LED_GPIO_PIN);
    ROM_GPIO_setOutputLowOnPin(LED_GPIO_PORT,LED_GPIO_PIN);
}

/**
 * @brief LED开启
 */
void bsp_led_on(void)
{
    ROM_GPIO_setOutputHighOnPin(LED_GPIO_PORT,LED_GPIO_PIN);
}

/**
 * @brief LED关闭
 */
void bsp_led_off(void)
{
    ROM_GPIO_setOutputLowOnPin(LED_GPIO_PORT,LED_GPIO_PIN);
}

/**
 * @brief LED翻转
 */
void bsp_led_toggle(void)
{
    ROM_GPIO_toggleOutputOnPin(LED_GPIO_PORT,LED_GPIO_PIN);
}
