/**
 * @file bsp_iic.c
 * @brief IIC板级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-06
 * @copyright Evan-GH
 * @par 日志:
 */
#include "driverlib.h"
#include "i2c.h"
#include "bsp_iic.h"

#define SLAVE_ADDRESS 0X78

#define IIC_GPIO_PORT GPIO_PORT_P6
#define IIC_SDA_GPIO_PIN GPIO_PIN4
#define IIC_SCL_GPIO_PIN GPIO_PIN5
volatile eUSCI_status iic_status;
uint8_t* pData;

#if HARDWARE_IIC
//iic配置参数
eUSCI_I2C_MasterConfig iic1_config =
{
        EUSCI_B_I2C_CLOCKSOURCE_SMCLK,          //时钟源选择SMCLK
        0,                                      //时钟速率用api来获取
        EUSCI_B_I2C_SET_DATA_RATE_400KBPS,      //IIC速率：400K
        0,                                      //无计数阈值
        EUSCI_B_I2C_SEND_STOP_AUTOMATICALLY_ON_BYTECOUNT_THRESHOLD  //自动停止
};

void bsp_iic_init(void)
{
    //IIC引脚配置
    GPIO_setAsPeripheralModuleFunctionOutputPin(IIC_GPIO_PORT,IIC_SDA_GPIO_PIN | IIC_SCL_GPIO_PIN, GPIO_PRIMARY_MODULE_FUNCTION);
    //更新时钟速率参数
    iic1_config.i2cClk = CS_getMCLK();
    //初始化并使能模块
    I2C_initMaster(EUSCI_B1_BASE, &iic1_config);
}

bool bsp_iic_write(uint8_t Addr, uint8_t Reg, uint8_t* Data,uint8_t ByteCount)
{
    //等待总线就绪
    while(I2C_isBusBusy(EUSCI_B1_BASE));

    pData = Data;

    //关闭IIC模块以便更改配置
    I2C_disableModule(EUSCI_B1_BASE);

    //配置传送阈值
    iic1_config.byteCounterThreshold = ByteCount+1;
    I2C_initMaster(EUSCI_B1_BASE,(const eUSCI_I2C_MasterConfig*)&iic1_config);
    //设置从机地址并重新使能模块
    I2C_setSlaveAddress(EUSCI_B1_BASE,Addr);
    I2C_enableModule(EUSCI_B1_BASE);
    //使能中断
    I2C_enableInterrupt(EUSCI_B1_BASE,EUSCI_B_I2C_STOP_INTERRUPT+
    EUSCI_B_I2C_NAK_INTERRUPT + EUSCI_B_I2C_TRANSMIT_INTERRUPT0);

    //设置总线状态
    iic_status = eUSCI_BUSY;

    //传输开始帧
    I2C_masterSendMultiByteStart(EUSCI_B1_BASE,Reg);
    //使能中断以便传输接下来的数据
    Interrupt_enableInterrupt(INT_EUSCIB1);
    //等待传输完成
   while(iic_status == eUSCI_BUSY)
   {
       __no_operation(); //nop 不操作
   }
    //关闭中断
    I2C_disableInterrupt(EUSCI_B1_BASE,EUSCI_B_I2C_STOP_INTERRUPT+
    EUSCI_B_I2C_NAK_INTERRUPT + EUSCI_B_I2C_TRANSMIT_INTERRUPT0);
    Interrupt_disableInterrupt(INT_EUSCIB1);

    if (iic_status == eUSCI_NACK) return false;
    else return true;
}

#endif

#if SOFTWARE_IIC
void bsp_iic_init(void)
{

}
#endif

