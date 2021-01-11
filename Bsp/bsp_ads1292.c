
//-----------------------------------------------------------------
// 头文件包含
//-----------------------------------------------------------------
#include "driverlib.h"
#include "bsp_ads1292.h"
#include "bsp_spi.h"
#include "bsp_delay.h"
#include "bsp_led.h"
#include "gpio.h"
#include <stdio.h>
//-----------------------------------------------------------------

//-----------------------------------------------------------------
// void ADS1292_Init(void)
//-----------------------------------------------------------------
//
// 函数功能: ADS1292初始化
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Init(void)
{
    GPIO_setAsInputPinWithPullUpResistor(ADS1292_DRDY_GPIO_PORT, ADS1292_DRDY_GPIO_PIN);

    GPIO_setAsOutputPin(ADS1292_START_GPIO_PORT, ADS1292_START_GPIO_PIN);
    GPIO_setAsOutputPin(ADS1292_PWDN_GPIO_PORT, ADS1292_PWDN_GPIO_PIN);
    GPIO_setAsOutputPin(ADS1292_CS_GPIO_PORT, ADS1292_CS_GPIO_PIN);
    GPIO_setAsOutputPin(ADS1292_GPIO1_GPIO_PORT, ADS1292_GPIO1_GPIO_PIN);
    GPIO_setAsOutputPin(ADS1292_GPIO2_GPIO_PORT, ADS1292_GPIO2_GPIO_PIN);
}

//-----------------------------------------------------------------
// void ADS1292_PowerOnInit(void)
//-----------------------------------------------------------------
//
// 函数功能: ADS1292上电复位
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_PowerOnInit(void)
{
    uint8_t device_id;

    ADS1292_START_SET();
    ADS1292_CS_SET();
    ADS1292_PWDN_CLR(); // 进入掉电模式
    bsp_delay_ms(200);
    ADS1292_PWDN_SET(); // 退出掉电模式
    bsp_delay_ms(200);  // 等待稳定
    ADS1292_PWDN_CLR(); // 发出复位脉冲
    bsp_delay_us(10);
    ADS1292_PWDN_SET();
    bsp_delay_ms(200); // 等待稳定，可以开始使用ADS1292R
    ADS1292_START_CLR();
    ADS1292_CS_CLR();
    bsp_spi0_send_byte(SDATAC); // 发送停止连续读取数据命令
    bsp_delay_us(10);
    ADS1292_CS_SET();

    // 获取芯片ID
    device_id = ADS1292_Read_Reg(RREG | ID);
    while (device_id != 0x73)
    {
        printf("ERROR ID:%02x\r\n", device_id);
        device_id = ADS1292_Read_Reg(RREG | ID);
        bsp_led_toggle();
        bsp_delay_ms(100);
    }

    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | CONFIG2, 0XE0); // 使用内部参考电压
    bsp_delay_ms(10);                        // 等待内部参考电压稳定
    ADS1292_Write_Reg(WREG | CONFIG1, 0X03); // 设置转换速率为1KSPS
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | LOFF, 0XF0); // 该寄存器配置引出检测操作
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | CH1SET, 0X00); // 增益6，连接到电极
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | CH2SET, 0X00); // 增益6，连接到电极
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | RLD_SENS, 0xEF);
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | LOFF_SENS, 0x0F);
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | LOFF_STAT, 0x00);
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | RESP1, 0xEA); // 开启呼吸检测（ADS1292R特有）
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | RESP2, 0x03);
    bsp_delay_us(10);
    ADS1292_Write_Reg(WREG | GPIO, 0x0C);
    bsp_delay_us(10);
}

//-----------------------------------------------------------------
// void ADS1292_Write_Reg(u8 com, u8 data)
//-----------------------------------------------------------------
//
// 函数功能: 对ADS1292的内部寄存器进行写操作
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Write_Reg(uint8_t addr, uint8_t data)
{

    ADS1292_CS_CLR(); // 片选拉低
    bsp_spi0_rw(addr); // 包含命令操作码和寄存器地址
    bsp_delay_us(10);
    bsp_spi0_rw(0X00); // 要读取的寄存器数+1
    bsp_delay_us(10);
    bsp_spi0_rw(data); // 写入的数据
    bsp_delay_us(10);
    ADS1292_CS_SET(); // 片选置高
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Reg(u8 addr)
//-----------------------------------------------------------------
//
// 函数功能: 对ADS1292的内部寄存器进行读操作
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
uint8_t ADS1292_Read_Reg(uint8_t addr)
{

    uint8_t Rxdata;

    ADS1292_CS_CLR();
    bsp_delay_us(10);
    Rxdata = bsp_spi0_rw(addr); // 包含命令操作码和寄存器地址
    bsp_delay_us(10);
    Rxdata = bsp_spi0_rw(0x00); // 要读取的寄存器数+1
    bsp_delay_us(10);
    Rxdata = bsp_spi0_rw(0xff); // 读取的数据
    bsp_delay_us(10);
    ADS1292_CS_SET();
    return Rxdata;
}

//-----------------------------------------------------------------
// u8 ADS1292_Read_Data(u8 addr)
//-----------------------------------------------------------------
//
// 函数功能: 读取ADS1292的数据
// 入口参数: 无
// 返 回 值: 无
// 注意事项: 无
//
//-----------------------------------------------------------------
void ADS1292_Read_Data(uint8_t *data)
{
    uint8_t i;

    ADS1292_CS_CLR();
    bsp_spi0_rw(RDATAC); // 发送启动连续读取数据命令
    bsp_delay_us(10);
    ADS1292_CS_SET();
    ADS1292_START_SET(); // 启动转换
    while (ADS1292_DRDY == 1)
        ; // 等待DRDY信号拉低
    ADS1292_CS_CLR();
    for (i = 0; i < 9; i++) // 连续读取9个数据
    {
        *data = bsp_spi0_rw(0xff);
        data++;
    }
    ADS1292_START_CLR(); // 停止转换
    bsp_spi0_rw(SDATAC);  // 发送停止连续读取数据命令
    bsp_delay_us(10);
    ADS1292_CS_SET();
}
//-----------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------
