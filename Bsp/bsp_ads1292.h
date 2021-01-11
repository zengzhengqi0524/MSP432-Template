#ifndef _ADS1292_H
#define _ADS1292_H
#include "driverlib.h"
//-----------------------------------------------------------------
// ADS1292引脚定义
//-----------------------------------------------------------------
#define ADS1292_DRDY_GPIO_PORT GPIO_PORT_P4
#define ADS1292_DRDY_GPIO_PIN GPIO_PIN1

#define ADS1292_START_GPIO_PORT GPIO_PORT_P4
#define ADS1292_START_GPIO_PIN GPIO_PIN2

#define ADS1292_PWDN_GPIO_PORT GPIO_PORT_P4
#define ADS1292_PWDN_GPIO_PIN GPIO_PIN3

#define ADS1292_CS_GPIO_PORT GPIO_PORT_P4
#define ADS1292_CS_GPIO_PIN GPIO_PIN4

#define ADS1292_GPIO1_GPIO_PORT GPIO_PORT_P4
#define ADS1292_GPIO1_GPIO_PIN GPIO_PIN5

#define ADS1292_GPIO2_GPIO_PORT GPIO_PORT_P4
#define ADS1292_GPIO2_GPIO_PIN GPIO_PIN6
//-----------------------------------------------------------------
// ADS1292命令定义
//-----------------------------------------------------------------
// 系统命令
#define WAKEUP 0X02    // 从待机模式唤醒
#define STANDBY 0X04   // 进入待机模式
#define RESET 0X06     // 复位ADS1292R
#define START 0X08     // 启动或转换
#define STOP 0X0A      // 停止转换
#define OFFSETCAL 0X1A // 通道偏移校准
// 数据读取命令
#define RDATAC 0X10 // 启用连续的数据读取模式,默认使用此模式
#define SDATAC 0X11 // 停止连续的数据读取模式
#define RDATA 0X12  // 通过命令读取数据;支持多种读回。
// 寄存器读取命令
#define RREG 0X20 // 读取001r rrrr 000n nnnn  其中r rrrr是起始寄存器地址
#define WREG 0X40 // 写入010r rrrr 000n nnnn	其中r rrrr是起始寄存器地址 n nnnn=是要写的数据*/

// ADS1292R内部寄存器地址定义
#define ID 0X00        // ID控制寄存器
#define CONFIG1 0X01   // 配置寄存器1
#define CONFIG2 0X02   // 配置寄存器2
#define LOFF 0X03      // 导联脱落控制寄存器
#define CH1SET 0X04    // 通道1设置寄存器
#define CH2SET 0X05    // 通道2设置寄存器
#define RLD_SENS 0X06  // 右腿驱动选择寄存器
#define LOFF_SENS 0X07 // 导联脱落检测选择寄存器
#define LOFF_STAT 0X08 // 导联脱落检测状态寄存器
#define RESP1 0X09     // 呼吸检测控制寄存器1
#define RESP2 0X0A     // 呼吸检测控制寄存器2
#define GPIO 0X0B      // GPIO控制寄存器

#define ADS1292_START_CLR() GPIO_setOutputLowOnPin(ADS1292_START_GPIO_PORT, ADS1292_START_GPIO_PIN)
#define ADS1292_START_SET() GPIO_setOutputHighOnPin(ADS1292_START_GPIO_PORT, ADS1292_START_GPIO_PIN)

#define ADS1292_PWDN_CLR() GPIO_setOutputLowOnPin(ADS1292_PWDN_GPIO_PORT, ADS1292_PWDN_GPIO_PIN)
#define ADS1292_PWDN_SET() GPIO_setOutputHighOnPin(ADS1292_PWDN_GPIO_PORT, ADS1292_PWDN_GPIO_PIN)

#define ADS1292_CS_CLR() GPIO_setOutputLowOnPin(ADS1292_CS_GPIO_PORT, ADS1292_CS_GPIO_PIN)
#define ADS1292_CS_SET() GPIO_setOutputHighOnPin(ADS1292_CS_GPIO_PORT, ADS1292_CS_GPIO_PIN)

#define ADS1292_DRDY GPIO_getInputPinValue(ADS1292_DRDY_GPIO_PORT, ADS1292_DRDY_GPIO_PIN)
//-----------------------------------------------------------------
// 外部函数声明
//-----------------------------------------------------------------
void ADS1292_Init(void);
void ADS1292_PowerOnInit(void);
void ADS1292_Write_Reg(uint8_t addr, uint8_t data);
uint8_t ADS1292_Read_Reg(uint8_t addr);
void ADS1292_Read_Data(uint8_t *data);

#endif
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
