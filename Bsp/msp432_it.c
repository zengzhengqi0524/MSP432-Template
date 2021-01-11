/**
 * @file msp432_it.c
 * @brief 中断服务函数入口
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
#include "bsp_button.h"
#include "bsp_iic.h"
#include "bsp_adc.h"
#include "rom.h"
#include "rom_map.h"

void NMI_Handler(void)
{
    //to do
}

void HardFault_Handler(void)
{
  while (1)
  {
      //to do
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
      //to do
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
      //to do
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
      //to do
  }
}

void DebugMon_Handler(void)
{
}

/**
 * @brief Systick中断服务函数
 */
//void SysTick_Handler(void)
//{
//    bsp_delay_IncTick();
//}

/**
 * @brief GPIOP1中断服务函数
 */
void PORT1_IRQHandler(void)
{
    uint32_t status;
    //获取中断状态
    status = ROM_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    //清除标志位
    ROM_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    //按键1中断触发，这里暂时不做延时检测
    if(status & BUTTON1_GPIO_PIN)
    {
        bsp_delay_ms(10);//延时检测，软件消抖
        if(ROM_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1) & BUTTON1_GPIO_PIN)
        {
            //处理部分
        }
    }
    //按键2中断触发，这里暂时不做延时检测
    else if(status & BUTTON2_GPIO_PIN)
    {
        bsp_delay_ms(10);//延时检测，软件消抖
        if(ROM_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1) & BUTTON2_GPIO_PIN)
        {
            //处理部分
        }
    }
}

/**
 * @brief UART1接收中断服务程序
 */
void EUSCIA0_IRQHandler(void)
{
    //获取中断状态
    uint32_t status = ROM_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

    ROM_UART_clearInterruptFlag(EUSCI_A0_BASE, status);
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //UART_transmitData(EUSCI_A0_BASE, MAP_UART_receiveData(EUSCI_A0_BASE));
        //数据解析
        //bsp_uart_analysis(UART_receiveData(EUSCI_A0_BASE));
    }
}

void EUSCIB1_IRQHandler(void)
{
   uint_fast16_t status;
   //获取中断状态
   status = I2C_getEnabledInterruptStatus(EUSCI_B1_BASE);
   I2C_clearInterruptFlag(EUSCI_B1_BASE, status);

   //从机没有应答
   if (status & EUSCI_B_I2C_NAK_INTERRUPT)
   {
       //停止iic传输
       MAP_I2C_masterSendMultiByteStop(EUSCI_B1_BASE);
       //清除中断
       MAP_I2C_clearInterruptFlag(EUSCI_B1_BASE, EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
       //设置状态为NACK
       iic_status = eUSCI_NACK;
   }
   //开始IIC传输
   if (status & EUSCI_B_I2C_START_INTERRUPT)
   {
       //设置状态为开始iic传输
       iic_status = eUSCI_START;
   }
   //结束IIC传输
   if (status & EUSCI_B_I2C_STOP_INTERRUPT)
   {
       //设置状态为iic传输结束
       iic_status = eUSCI_STOP;
   }
   //接收中断
   if (status & EUSCI_B_I2C_RECEIVE_INTERRUPT0)
   {

   }
   //发送一个字符完成的中断
   if (status & EUSCI_B_I2C_TRANSMIT_INTERRUPT0)
   {
       //发送下一个字符
       MAP_I2C_masterSendMultiByteNext(EUSCI_B1_BASE, *pData++);
   }
}

void TA0_0_IRQHandler(void)
{
//获取中断状态
    uint32_t status = ROM_Timer_A_getInterruptStatus(TIMER_A0_BASE);

    //UART_clearInterruptFlag(EUSCI_A0_BASE, status);
}

void EUSCIA2_IRQHandler(void)
{
    uint32_t status = ROM_UART_getEnabledInterruptStatus(EUSCI_A2_BASE);
    ROM_UART_clearInterruptFlag(EUSCI_A2_BASE, status);

    //接收中断,uart接收到字节,接收中断标志位会置1
    if(status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        bsp_uart_analysis(UART_receiveData(EUSCI_A2_BASE));
    }
}

/**
 * @brief TIMER32 中断服务函数
 */
void T32_INT1_IRQHandler(void)
{
    ROM_Timer32_clearInterruptFlag(TIMER32_0_BASE); //清除Timer32第一个计数器中断标志
    bsp_delay_IncTick();
}

/**
 * @brief DMA中断服务函数
 */
void DMA_INT0_IRQHandler(void)
{
    ROM_DMA_clearInterruptFlag(DMA_CH0_EUSCIA0TX);
}

/**
 * @brief ADC中断服务程序
 */
void ADC14_IRQHandler(void)
{
    //获取中断状态
    uint64_t status = ROM_ADC14_getEnabledInterruptStatus();
    //清除标志位
    ROM_ADC14_clearInterruptFlag(status);
    //用于储存采样结果的数组
    uint16_t resultsBuffer[3];

    if (ADC_INT2 & status)
    {
        //ADC2采集完了说明一次多通道转换已经结束
        MAP_ADC14_getMultiSequenceResult(resultsBuffer);
        for(int i=2; i>=0; i--)
        {
            bsp_adc_OriginalData[i][bsp_adc_index] = resultsBuffer[i];
        }
        if((bsp_adc_index++) == 50) bsp_adc_index = 0; //循环填充
    }
}

/**
 * @brief SPI0中断服务程序
 */
void EUSCIB0_IRQHandler(void)
{
    //获取中断状态
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B0_BASE);
    //清除中断
    SPI_clearInterruptFlag(EUSCI_B0_BASE, status);
}

/**
 * @brief SPI2中断服务程序
 */
void EUSCIB2_IRQHandler(void)
{
    //获取中断状态
    uint32_t status = SPI_getEnabledInterruptStatus(EUSCI_B2_BASE);
    //清除中断
    SPI_clearInterruptFlag(EUSCI_B2_BASE, status);
}
