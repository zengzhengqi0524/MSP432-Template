/**
 * @file main.c
 * @brief 工程主体
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "bsp_delay.h"
#include "bsp_pwm.h"
#include "bsp_uart.h"
#include "bsp_clock.h"
#include "bsp_led.h"
#include "bsp_rgb.h"
#include "bsp_button.h"
#include "bsp_iic.h"
#include "bsp_spi.h"
#include "bsp_adc.h"
#include "bsp_flash.h"
#include "bsp_st7789.h"
#include "bsp_ads1292.h"
#include "app_math.h"
#include "app_dt.h"
#include "FreeRTOS.h"
#include "task.h"

static TaskHandle_t task_log_Sysmonitor_Handle = NULL;  //!< 监控任务句柄
TaskHandle_t task_init_Creat_Handle = NULL;  //!< 任务创建句柄

static TaskHandle_t task_main_Handle = NULL;  //!< 主任务句柄

/**
 * @brief 系统监控任务
 * @param pvParameters 任务句柄
 */
void task_log_Sysmonitor(void* pvParameters)
{
    int a= 0;
	while(1)
	{
        bsp_led_toggle();
        bsp_adc_calVoltage();
        //oled刷新测试
        BACK_COLOR=WHITE;
        POINT_COLOR=RED;
        showhanzi(10,0,0);  //中
        showhanzi(45,0,1);  //景
        showhanzi(80,0,2);  //园
        showhanzi(115,0,3);  //电
        showhanzi(150,0,4);  //子
        LCD_ShowString(10,35,"1.3 IPS-SPI 240*320");
        LCD_ShowString(10,55,"LCD_W:");	LCD_ShowNum(70,55,LCD_W,3);
        LCD_ShowString(110,55,"LCD_H:");LCD_ShowNum(160,55,LCD_H,3);
        LCD_ShowNum(70,155,a++,3);
        vTaskDelay(1);
	}
}

void task_main(void* pvParameters)
{
    TickType_t xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
    while (1)
    {
        vTaskDelayUntil(&xLastWakeTime,1/portTICK_PERIOD_MS);
    }

}

int main(void)
{
    //关看门狗
    WDT_A_holdTimer();
    //初始化系统时钟
    bsp_clock_init();
    //中断分组初始化，优先级0~7
    Interrupt_setPriorityGrouping(0x03);
    //关闭中断开始系统硬件初始化(部分硬件，这些不会用到与中断有关的变量)
    Interrupt_disableMaster();
    //开启Fpu，并启动堆栈
    FPU_enableLazyStacking();
    FPU_enableModule();
    //使用RTOS不需要对systick进行初始化，初始化timer32
    bsp_delay_init();
    //LED初始化
    bsp_led_init();
    //IIC初始化
    bsp_iic_init();
    //RGB初始化
    bsp_rgb_init();
    //按键初始化
    bsp_button_init();
    //SPI初始化
    bsp_spi_init();
    //串口初始化
    bsp_uart_init();
    //ADC初始化,adc用了timer_a
    bsp_adc_init();
    //spi初始化
    bsp_spi_init();
    //初始化全部完成后开启处理器中断响应(后面初始化的都是需要用到相关中断的)
    Interrupt_enableMaster();
    //初始化ADS1292
    ADS1292_Init();
    //初始化OLED
    Lcd_Init();
	LCD_Clear(RED); //清屏
    LCD_Clear(WHITE);
    LCD_Clear(YELLOW);

    //任务创建阶段
    taskENTER_CRITICAL();  //进入临界区

	//以下任务的堆栈大小可以通过Debug查看堆栈余量来进行确定

	xTaskCreate((TaskFunction_t )task_log_Sysmonitor,
                (const char*    )"Log_Task",
                (uint16_t       )128,
                (void*          )NULL,
                (UBaseType_t    )1,
                (TaskHandle_t*  )&task_log_Sysmonitor_Handle);  //创建系统监控任务

    xTaskCreate((TaskFunction_t )task_main,
                (const char*    )"Log_main",
                (uint16_t       )128,
                (void*          )NULL,
                (UBaseType_t    )2,
                (TaskHandle_t*  )&task_main_Handle);  //创建系统监控任务

	taskEXIT_CRITICAL();  //退出临界区

    vTaskStartScheduler(); //启动调度器

    while(1)
    {
        //如果调度器运行正常，不会运行到这里
        bsp_led_toggle();
        bsp_rgb_set(0x01);
        bsp_delay_ms(1000);  //如果运行到这里，板载LED1S周期闪烁，板载RGB红灯常亮
    }
}
