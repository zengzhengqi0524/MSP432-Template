/**
 * @file bsp_pwm.c
 * @brief PWM板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "bsp_pwm.h"
#include "driverlib.h"
#include "gpio.h"
#include "time.h"
#include "rom.h"
#include "rom_map.h"

#define PWM1_GPIO_PORT GPIO_PORT_P2
#define PWM1_GPIO_PIN GPIO_PIN4
#define PWM2_GPIO_PORT GPIO_PORT_P2
#define PWM2_GPIO_PIN GPIO_PIN6

/**
 * @brief PWM波1输出的配置参数
 * 通道1
 */
Timer_A_PWMConfig pwm1_config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,      //时钟源采用SMCLK = 48M
        TIMER_A_CLOCKSOURCE_DIVIDER_48, //48分频 = 1MHz
        2000,
        TIMER_A_CAPTURECOMPARE_REGISTER_1,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0    //初始值置为0
};

/**
 * @brief PWM波2输出的配置参数
 * 通道3
 */
Timer_A_PWMConfig pwm2_config =
{
        TIMER_A_CLOCKSOURCE_SMCLK,      //时钟源采用SMCLK = 48M
        TIMER_A_CLOCKSOURCE_DIVIDER_48, //48分频 = 1MHz
        2000,
        TIMER_A_CAPTURECOMPARE_REGISTER_3,
        TIMER_A_OUTPUTMODE_RESET_SET,
        0    //初始值置为0
};

/**
 * @brief PWM初始化
 */
void bsp_pwm_init(void)
{
    //PWM引脚配置
    ROM_GPIO_setAsPeripheralModuleFunctionOutputPin(PWM1_GPIO_PORT, PWM1_GPIO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    ROM_GPIO_setAsPeripheralModuleFunctionOutputPin(PWM2_GPIO_PORT, PWM2_GPIO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    //开始产生PWM波，初始占空比为0
    ROM_Timer_A_generatePWM(TIMER_A0_BASE, &pwm1_config);
    ROM_Timer_A_generatePWM(TIMER_A0_BASE, &pwm2_config);
}

/**
 * @brief PWM波占空比设置
 * @param pwmnum pwm波序号
 * @param setnum 比较器数字大小，不能大于上限20000
 */
void bsp_pwm_set(uint8_t pwmnum, uint32_t setnum)
{
    switch (pwmnum)
    {
        //PWM1,通道1
        case 1:
        //改变占空比并输出
            if(pwm1_config.dutyCycle!=setnum)
            {
                pwm1_config.dutyCycle = setnum;
                ROM_Timer_A_generatePWM(TIMER_A0_BASE, &pwm1_config);
            }
        break;
        //PWM2,通道3
        case 2:
        //改变占空比并输出
            if(pwm2_config.dutyCycle!=setnum)
            {
                pwm2_config.dutyCycle = setnum;
                ROM_Timer_A_generatePWM(TIMER_A0_BASE, &pwm2_config);
            }
        break;

    }
}
