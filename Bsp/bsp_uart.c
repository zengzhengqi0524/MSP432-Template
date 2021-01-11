/**
 * @file bsp_uart.c
 * @brief 串口板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "bsp_uart.h"
#include "uart.h"
#include "gpio.h"
#include "dma.h"
#include "rom.h"
#include "rom_map.h"
#include <stdio.h>
#include <string.h>
#include "bsp_delay.h"
#include <stdlib.h>

//注意内存对齐，用户手册表示需要1024字节对齐
uint8_t controlTable[1024] __attribute__((aligned(1024)));
int16_t pos_x,pos_y,pos_z;

#define UART1_GPIO_PORT GPIO_PORT_P1
#define UART1_TX_GPIO_PIN GPIO_PIN3
#define UART1_RX_GPIO_PIN GPIO_PIN2
#define UART2_GPIO_PORT GPIO_PORT_P3
#define UART2_TX_GPIO_PIN GPIO_PIN3
#define UART2_RX_GPIO_PIN GPIO_PIN2

/**
 * @brief 串口0配置参数
 * @note 计算参数的网址：http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html
 * @note SMCLK前面已经配置成为48M，见bsp_clock.c
 */
const eUSCI_UART_Config bsp_uart0_config =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //时钟源采用SMCLK
    6,                                     //BRDIV = 26
    0,                                       //UCxBRF = 1
    0,                                       //UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,                  //无校验位
    EUSCI_A_UART_LSB_FIRST,                  //LSB先行
    EUSCI_A_UART_ONE_STOP_BIT,               //一停止位
    EUSCI_A_UART_MODE,                       //UART模式
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  //过采样
};

/**
 * @brief 波特率115200
 * 串口2
 */
const eUSCI_UART_Config bsp_uart2_config =
{
    EUSCI_A_UART_CLOCKSOURCE_SMCLK,          //时钟源采用SMCLK
    26,                                     //BRDIV = 26
    1,                                       //UCxBRF = 1
    0,                                       //UCxBRS = 0
    EUSCI_A_UART_NO_PARITY,                  //无校验位
    EUSCI_A_UART_LSB_FIRST,                  //LSB先行
    EUSCI_A_UART_ONE_STOP_BIT,               //一停止位
    EUSCI_A_UART_MODE,                       //UART模式
    EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION  //过采样
};

uint8_t send_data[16] = {0x1,0x2,0x4,0x8,0x16,0x32,0x64,0x3,0x6,0x12,0x24,0x48,0x00,0x41,0x25,0x33};
/**
 * @brief 串口配置初始化
 */
void bsp_uart_init(void)
{
    //Uart1引脚配置
    ROM_GPIO_setAsPeripheralModuleFunctionInputPin(UART1_GPIO_PORT,UART1_TX_GPIO_PIN | UART1_RX_GPIO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    ROM_UART_initModule(EUSCI_A0_BASE, &bsp_uart0_config);
    //使能模块
    ROM_UART_enableModule(EUSCI_A0_BASE);
    //使能中断
    ROM_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    ROM_Interrupt_enableInterrupt(INT_EUSCIA0);
    //优先级二号
    ROM_Interrupt_setPriority(INT_EUSCIA0,0x40);
    //DMA配置
    ROM_DMA_enableModule();
    ROM_DMA_setControlBase(controlTable);
    ROM_DMA_assignChannel(DMA_CH0_EUSCIA0TX);
    ROM_DMA_setChannelControl(UDMA_PRI_SELECT | DMA_CH0_EUSCIA0TX,
                        UDMA_SIZE_8|UDMA_SRC_INC_8|UDMA_DST_INC_NONE|UDMA_ARB_1);
    ROM_DMA_assignInterrupt(DMA_INT0, 0);
    ROM_Interrupt_enableInterrupt(INT_DMA_INT0);
    //优先级一号
    ROM_Interrupt_setPriority(INT_DMA_INT0,0X20);

    //串口2配置
    ROM_GPIO_setAsPeripheralModuleFunctionInputPin(UART2_GPIO_PORT,UART2_TX_GPIO_PIN | UART2_RX_GPIO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    ROM_UART_initModule(EUSCI_A2_BASE, &bsp_uart2_config);
    //使能模块
    ROM_UART_enableModule(EUSCI_A2_BASE);
    //使能中断
    ROM_UART_enableInterrupt(EUSCI_A2_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
    //优先级三号
    ROM_Interrupt_setPriority(INT_EUSCIA2,0x60);
    ROM_Interrupt_enableInterrupt(INT_EUSCIA2);

}

/**
 * @brief 打印一个字符串
 * @param String 字符串内容
 */
void bsp_uart0_transmitString_DMA(uint8_t* String, uint16_t len)
{
    uint32_t DMAMode = DMA_getChannelMode(DMA_CH0_EUSCIA0TX);
    //等待上一次传输完毕
    while(DMAMode != UDMA_MODE_STOP)
    {
        DMAMode = DMA_getChannelMode(DMA_CH0_EUSCIA0TX);
    }
    //DMA传输
    ROM_DMA_setChannelTransfer(UDMA_PRI_SELECT | DMA_CH0_EUSCIA0TX,
                        UDMA_MODE_BASIC,
                        String,
                        (void*)UART_getTransmitBufferAddressForDMA(EUSCI_A0_BASE),
                        len);
    ROM_DMA_enableChannel(0);
}

//接收用字符缓冲区
uint8_t uart_rec_buffer[255];
/**
 * @brief 对收到的字符进行解析
 * @param data 接收到的单字符
 */
 void bsp_uart_analysis(uint8_t data)
 {
     //状态机变量
     static uint8_t status = 0;
     //接收到的数据数，应该接收到的数据长度
     static uint8_t cnt = 0, length =0;
     //数据和校验
     static uint8_t sum_check = 0;
     //数据接收状态机
     switch (status)
     {
         case 0:
         //检测帧首
             if (data == 0xff) status = 1;
             else {status = 0; cnt = 0; length = 0;sum_check = 0;}
         break;
         case 1:
         //检测功能字
             if(data == 0x11) status = 2;
              else {status = 0; cnt = 0; length = 0;sum_check = 0;}
         break;
         case 2:
         //获取应该接收的数据长度
             length = data;
             status = 3;
             sum_check = (0xff+0x11+length) & 0xff;
         break;
         case 3:
         //接收数据
             uart_rec_buffer[cnt++] = data;
             sum_check = (sum_check+data) & 0xff;
             if(cnt == length)
             {
                 status = 4; cnt = 0; length = 0;
             }
         break;
         case 4:
         //和校验检测
             if(sum_check == data) status = 5;
             else {status = 0; cnt = 0; length = 0;sum_check = 0;}
         break;
         case 5:
         //帧尾检测
             if(data == 0x0d)
             {
                 pos_x = (uart_rec_buffer[0]<<0x08) + uart_rec_buffer[1];
                 pos_y = (uart_rec_buffer[2]<<0x08) + uart_rec_buffer[3];
                 pos_z = (uart_rec_buffer[4]<<0x08) + uart_rec_buffer[5];
             }
             status = 0; cnt = 0; length = 0;sum_check = 0;
         break;
         default:
             status = 0;
             sum_check = 0;  //和校验清0
             cnt = 0; length =0; //接收相关变量清0
         break;
     }
 }


/**
 * @brief printf重定向
 */
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
  #define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
  #define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
/**
  * @brief  Retargets the C library printf function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the EVAL_COM1 and Loop until the end of transmission */
  UART_transmitData(EUSCI_A0_BASE, (uint_fast8_t)ch);
  return ch;
}
