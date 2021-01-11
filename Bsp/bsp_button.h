/**
 * @file bsp_button.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 */
#ifndef __BSP_BUTTON_H
#define __BSP_BUTTON_H

#define BUTTON1_GPIO_PORT GPIO_PORT_P1
#define BUTTON2_GPIO_PORT GPIO_PORT_P1
#define BUTTON1_GPIO_PIN GPIO_PIN1
#define BUTTON2_GPIO_PIN GPIO_PIN4

void bsp_button_init(void);

#endif
