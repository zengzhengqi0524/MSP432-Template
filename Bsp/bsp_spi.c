/**
 * @file bsp_spi.c
 * @brief spi板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-07
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "gpio.h"
#include "spi.h"
#include "rom.h"
#include "rom_map.h"
#include "bsp_spi.h"

//ADS1292用spi0
#define ADS_SPI_GPIO_PORT GPIO_PORT_P1
#define ADS_SPI_CLK_PIN GPIO_PIN5
#define ADS_SPI_SIMO_PIN GPIO_PIN6
#define ADS_SPI_SOMI_PIN GPIO_PIN7
//OLED用spi2
#define OLED_SPI_GPIO_PORT GPIO_PORT_P3
#define OLED_SPI_CLK_PIN GPIO_PIN5
#define OLED_SPI_SIMO_PIN GPIO_PIN6
#define OLED_SPI_SOMI_PIN GPIO_PIN7

//OLED用配置
const eUSCI_SPI_MasterConfig spi2_config =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             // 选择时钟源为SMCLK
        48000000,                                   // SMCLK = 48MHZ
        4000000,                                    // SPICLK = 4Mhz
        EUSCI_B_SPI_MSB_FIRST,                     // MSB先行
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_HIGH, // High polarity
        EUSCI_SPI_4PIN_UCxSTE_ACTIVE_HIGH                          // 4线SPI模式
};

//ADS1292用配置
const eUSCI_SPI_MasterConfig spi0_config =
{
        EUSCI_B_SPI_CLOCKSOURCE_SMCLK,             //SMCLK
        48000000,                                   // SMCLK = 48MHZ
        4000000,                                    // SPICLK =4M
        EUSCI_B_SPI_MSB_FIRST,                     // MSB先行
        EUSCI_B_SPI_PHASE_DATA_CHANGED_ONFIRST_CAPTURED_ON_NEXT,    // Phase
        EUSCI_B_SPI_CLOCKPOLARITY_INACTIVITY_LOW, // low polarity
        EUSCI_B_SPI_3PIN                           // 3线模式
};

void bsp_spi_init(void)
{
    //引脚配置
    ROM_GPIO_setAsPeripheralModuleFunctionOutputPin(ADS_SPI_GPIO_PORT,ADS_SPI_CLK_PIN | ADS_SPI_SIMO_PIN | ADS_SPI_SOMI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    ROM_GPIO_setAsPeripheralModuleFunctionOutputPin(OLED_SPI_GPIO_PORT,OLED_SPI_CLK_PIN | OLED_SPI_SIMO_PIN | OLED_SPI_SOMI_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    //初始化模块并使能
    ROM_SPI_initMaster(EUSCI_B0_BASE, &spi0_config);
    ROM_SPI_initMaster(EUSCI_B2_BASE, &spi2_config);
    ROM_SPI_enableModule(EUSCI_B0_BASE);
    ROM_SPI_enableModule(EUSCI_B2_BASE);
    //中断设置
    ROM_SPI_clearInterruptFlag(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT);
    ROM_SPI_enableInterrupt(EUSCI_B0_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT);
    ROM_Interrupt_enableInterrupt(INT_EUSCIB0);
    //优先级6
    ROM_Interrupt_setPriority(INT_EUSCIB0,0XD0);

    SPI_clearInterruptFlag(EUSCI_B2_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT);
    SPI_enableInterrupt(EUSCI_B2_BASE, EUSCI_B_SPI_TRANSMIT_INTERRUPT);
    Interrupt_enableInterrupt(INT_EUSCIB2);
    //优先级6
    ROM_Interrupt_setPriority(INT_EUSCIB2,0XD0);
}

/**
 * @brief spi0总线数据发送函数
 * @param data 要发送的数据
 * @note ADS1292使用
 */
void bsp_spi0_send_byte(char data)
{
    //以下等待操作是为了防止spi传送时数据还没发出去就被后一个数据覆盖掉
    //等待总线不忙
    while (ROM_SPI_isBusy(EUSCI_B0_BASE) != EUSCI_SPI_NOT_BUSY);
    //等待发送缓冲区为空
	while ((ROM_SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
    //开始发送
    ROM_SPI_transmitData(EUSCI_B0_BASE,data);
}

/**
 * @brief spi0总线读取数据
 * @return uint8_t 读取到的数据
 * @note ADS1292使用
 */
uint8_t bsp_spi0_rw(uint8_t data)
{
    //以下等待操作是为了防止spi传送时数据还没发出去就被后一个数据覆盖掉
    //等待总线不忙
    while (ROM_SPI_isBusy(EUSCI_B0_BASE) != EUSCI_SPI_NOT_BUSY);
    //等待发送缓冲d区为空
	while ((ROM_SPI_getInterruptStatus(EUSCI_B0_BASE,EUSCI_B_SPI_TRANSMIT_INTERRUPT)));
    //开始发送
    ROM_SPI_transmitData(EUSCI_B0_BASE,data);
    //发完等一下总线
    while (ROM_SPI_isBusy(EUSCI_B0_BASE) != EUSCI_SPI_NOT_BUSY);
    return SPI_receiveData(EUSCI_B0_BASE);;
}
