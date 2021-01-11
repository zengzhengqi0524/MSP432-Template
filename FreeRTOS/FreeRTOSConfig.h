/*
 * FreeRTOS Kernel V10.4.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
 */


/******************************************************************************
	See http://www.freertos.org/a00110.html for an explanation of the
	definitions contained in this file.
******************************************************************************/

#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

//加载Ti固件库
#include "driverlib.h"
#include "rom_map.h"

/* Set mainCREATE_SIMPLE_BLINKY_DEMO_ONLY to one to run the simple blinky demo,
or 0 to run the more comprehensive test and demo application.

The comprehensive demo uses FreeRTOS+CLI to create a simple command line
interface through a UART.

The blinky demo uses FreeRTOS's tickless idle mode to reduce power consumption.
See the notes on the web page below regarding the difference in power saving
that can be achieved between using the generic tickless implementation (as used
by the blinky demo) and a tickless implementation that is tailored specifically
to the MSP432.

See http://www.FreeRTOS.org/TI_MSP432_Free_RTOS_Demo.html for instructions. */
#define configCREATE_SIMPLE_TICKLESS_DEMO	1


/*-----------------------------------------------------------
 * Application specific definitions.
 *
 * These definitions should be adjusted for your particular hardware and
 * application requirements.
 *
 * THESE PARAMETERS ARE DESCRIBED WITHIN THE 'CONFIGURATION' SECTION OF THE
 * FreeRTOS API DOCUMENTATION AVAILABLE ON THE FreeRTOS.org WEB SITE.
 * http://www.freertos.org/a00110.html
 *----------------------------------------------------------*/

/* Constants related to the behaviour or the scheduler. */
/* 某些运行 FreeRTOS 的硬件有两种方法选择下一个要执行的任务：
 * 通用方法和特定于硬件的方法（以下简称“特殊方法”）。
 * 通用方法：
 *       1.configUSE_PORT_OPTIMISED_TASK_SELECTION 为 0 或者硬件不支持这种特殊方法。
 *       2.可以用于所有 FreeRTOS 支持的硬件
 *       3.完全用 C 实现，效率略低于特殊方法。
 *       4.不强制要求限制最大可用优先级数目
 * 特殊方法：
 *       1.必须将 configUSE_PORT_OPTIMISED_TASK_SELECTION 设置为 1。
 *       2.依赖一个或多个特定架构的汇编指令（一般是类似计算前导零[CLZ]指令）。
 *       3.比通用方法更高效
 *       4.一般强制限定最大可用优先级数目为 32
 * 一般是硬件计算前导零指令，如果所使用的，MCU 没有这些硬件指令的话此宏应该设置为 0！
 */
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1
//使用抢占式调度器
#define configUSE_PREEMPTION					1
//使能时间片调度
#define configUSE_TIME_SLICING					1
//最大可用优先级
#define configMAX_PRIORITIES					( 32 )
//空闲任务放弃 CPU 使用权给其他同优先级的用户任务
#define configIDLE_SHOULD_YIELD					1
//系统节拍计数器变量数据类型，1 表示为 16 位无符号整形，0 表示为 32 位无符号整形
#define configUSE_16_BIT_TICKS					0 /* Only for 8 and 16-bit hardware. */

/* Constants that describe the hardware and memory usage. */
//CPU主频周期
#define configCPU_CLOCK_HZ						MAP_CS_getMCLK()
//1s心跳数
#define configTICK_RATE_HZ						( ( TickType_t ) 1000 )
//空闲任务堆栈大小
#define configMINIMAL_STACK_SIZE				( ( uint16_t ) 128 )
//任务名字长度
#define configMAX_TASK_NAME_LEN					( 16 )

/* Note heap_5.c is used so this only defines the part of the heap that is in
the first block of RAM on the LPC device.  See the initialisation of the heap
in main.c. */
//系统堆大小
#define configTOTAL_HEAP_SIZE					( ( size_t ) ( 50 * 1024 ) )
//支持动态内存申请
#define configSUPPORT_DYNAMIC_ALLOCATION        1
//关闭静态内存申请
#define configSUPPORT_STATIC_ALLOCATION         0

/* Constants that build features in or out. */
//使能互斥信号量
#define configUSE_MUTEXES						1
//保持系统节拍一直运行
#define configUSE_TICKLESS_IDLE					0
#define configUSE_APPLICATION_TASK_TAG			0
#define configUSE_NEWLIB_REENTRANT 				0
#define configUSE_CO_ROUTINES 					0
//使能计数信号量
#define configUSE_COUNTING_SEMAPHORES 			1
//使能递归互斥信号量
#define configUSE_RECURSIVE_MUTEXES				1
//启用队列
#define configUSE_QUEUE_SETS					1
#define configUSE_TASK_NOTIFICATIONS			1

/* Constants that define which hook (callback) functions should be used. */
//Hook函数均不开启
#define configUSE_IDLE_HOOK						0
#define configUSE_TICK_HOOK						0
#define configUSE_MALLOC_FAILED_HOOK			0

/* Constants provided for debugging and optimisation assistance. */
/* 大于 0 时启用堆栈溢出检测功能，如果使用此功能
 * 用户必须提供一个栈溢出钩子函数，如果使用的话
 * 此值可以为 1 或者 2，因为有两种栈溢出检测方法 */
#define configCHECK_FOR_STACK_OVERFLOW			0
#define configASSERT( x ) if( ( x ) == 0 ) { taskDISABLE_INTERRUPTS(); for( ;; ); }
#define configQUEUE_REGISTRY_SIZE				0

/* Software timer definitions. */
#define configUSE_TIMERS						1
#define configTIMER_TASK_PRIORITY				(configMAX_PRIORITIES-1)
#define configTIMER_QUEUE_LENGTH				10
#define configTIMER_TASK_STACK_DEPTH			( configMINIMAL_STACK_SIZE * 2  )

/* Set the following definitions to 1 to include the API function, or zero
to exclude the API function.  NOTE:  Setting an INCLUDE_ parameter to 0 is only
necessary if the linker does not automatically remove functions that are not
referenced anyway. */
#define INCLUDE_vTaskPrioritySet				1
#define INCLUDE_uxTaskPriorityGet				1
#define INCLUDE_vTaskDelete						1
#define INCLUDE_vTaskCleanUpResources			0
#define INCLUDE_vTaskSuspend					1
#define INCLUDE_vTaskDelayUntil					1
#define INCLUDE_vTaskDelay						1
#define INCLUDE_uxTaskGetStackHighWaterMark		0
#define INCLUDE_xTaskGetIdleTaskHandle			0
#define INCLUDE_eTaskGetState					1
#define INCLUDE_xTaskResumeFromISR				0
#define INCLUDE_xTaskGetCurrentTaskHandle		1
#define INCLUDE_xTaskGetSchedulerState			0
#define INCLUDE_xSemaphoreGetMutexHolder		0
#define INCLUDE_xTimerPendFunctionCall			1

/* This demo makes use of one or more example stats formatting functions.  These
format the raw data provided by the uxTaskGetSystemState() function in to human
readable ASCII form.  See the notes in the implementation of vTaskList() within
FreeRTOS/Source/tasks.c for limitations. */
#define configUSE_STATS_FORMATTING_FUNCTIONS	1

/* Dimensions a buffer that can be used by the FreeRTOS+CLI command
interpreter.  See the FreeRTOS+CLI documentation for more information:
http://www.FreeRTOS.org/FreeRTOS-Plus/FreeRTOS_Plus_CLI/ */
#define configCOMMAND_INT_MAX_OUTPUT_SIZE		2048


/* Cortex-M3/4 interrupt priority configuration follows...................... */

/* Use the system definition, if there is one. */
#ifdef __NVIC_PRIO_BITS
	#define configPRIO_BITS       __NVIC_PRIO_BITS
#else
	#define configPRIO_BITS       3     /* 8 priority levels */
#endif

/* The lowest interrupt priority that can be used in a call to a "set priority"
function. */
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY			0x07

/* The highest interrupt priority that can be used by any interrupt service
routine that makes calls to interrupt safe FreeRTOS API functions.  DO NOT CALL
INTERRUPT SAFE FREERTOS API FUNCTIONS FROM ANY INTERRUPT THAT HAS A HIGHER
PRIORITY THAN THIS! (higher priorities are lower numeric values. */
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	5

/* Interrupt priorities used by the kernel port layer itself.  These are generic
to all Cortex-M ports, and do not rely on any particular library functions. */
#define configKERNEL_INTERRUPT_PRIORITY 		( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )
/* !!!! configMAX_SYSCALL_INTERRUPT_PRIORITY must not be set to zero !!!!
See http://www.FreeRTOS.org/RTOS-Cortex-M3-M4.html. */
#define configMAX_SYSCALL_INTERRUPT_PRIORITY 	( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << (8 - configPRIO_BITS) )

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
standard names - can't be used with CCS due to limitations in the assemblers
pre-processing. */
#ifndef __TI_COMPILER_VERSION__
	#define xPortPendSVHandler 	PendSV_Handler
	#define vPortSVCHandler 	SVC_Handler
	#define xPortSysTickHandler	SysTick_Handler
#endif

/* The trace facility is turned on to make some functions available for use in
CLI commands. */
//关闭可视化追踪
#define configUSE_TRACE_FACILITY	0

/* The #ifdef guards against the file being included from IAR assembly files. */
/* Constants related to the generation of run time stats.  Run time stats
are gathered in the full demo, not the blinky demo. */
#define configGENERATE_RUN_TIME_STATS			0
#define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()
#define portGET_RUN_TIME_COUNTER_VALUE()		0

#endif /* FREERTOS_CONFIG_H */

