/**
 * @file bsp_delay.c
 * @brief 延时板级支持包
 * 移植自STM32HAL库
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "bsp_delay.h"
#include "rom.h"
#include "rom_map.h"
#include "timer32.h"

//1S共进入中断1000次
#define uwTickFreq 1
#define HAL_MAX_DELAY 0xFFFFFFFFU
__IO uint32_t uwTick = 0;
//使用FreeRTOS时，systick作为系统时基，这里更换timer32作为用户延时函数的实现
#define USE_FREE_RTOS 1

#if !USE_FREE_RTOS
/**
 * @brief Systick设置初始化函数
 * @param freq 频率数
 */
void bsp_delay_init(void)
{
    //使能Systick
    SysTick_enableModule();
    //设置装载数
    SysTick_setPeriod(SystemCoreClock/1000);
    //使能Systick相关中断
    SysTick_enableInterrupt();
}
#else
/**
 * @brief timer32定时器初始化
 */
void bsp_delay_init(void)
{
    //48M/16 = 3M
    ROM_Timer32_initModule(TIMER32_0_BASE,TIMER32_PRESCALER_16,TIMER32_32BIT,TIMER32_PERIODIC_MODE);
    //使能中断
    ROM_Interrupt_enableInterrupt(INT_T32_INT1);
    ROM_Timer32_enableInterrupt(TIMER32_0_BASE);
    //优先级设置0x00，高三位有效，优先级为0
    ROM_Interrupt_setPriority(INT_T32_INT1,0X00);
    //3M / 1000 = 3000
    ROM_Timer32_setCount(TIMER32_0_BASE,3000);
    ROM_Timer32_startTimer(TIMER32_0_BASE,false);
}

#endif

/**
 * @brief Systick中断服务函数中调用，自增uwTick
 */
void bsp_delay_IncTick(void)
{
  uwTick += uwTickFreq;
}

/**
 * @brief 获取Systick计时数
 * @return uwTick 计数
 */
uint32_t bsp_delay_GetTick(void)
{
  return uwTick;
}

/**
 * @brief MS级延时函数
 * @param Delay 延时的Ms数
 */
void bsp_delay_ms(uint32_t Delay)
{
  uint32_t tickstart = bsp_delay_GetTick();
  uint32_t wait = Delay;

  if (wait < HAL_MAX_DELAY)
  {
    wait += (uint32_t)(uwTickFreq);
  }

  while((bsp_delay_GetTick() - tickstart) < wait)
  {
  }
}
/**
 * @brief us级延时函数
 * @param Delay 延时的us数
 */
void bsp_delay_us(uint32_t Delay)
{
    //TODO 测试延时
    uint32_t i,j;
    for(i=4;i>0;i--)
    {
        for(j=Delay;j>0;j--)
        {
            ;
        }
    }
}
