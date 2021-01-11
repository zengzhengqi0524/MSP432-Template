本工程适用于MSP432 Launchpad
基本实现了板级硬件的初始化
添加了FreeRTOS的支持

中断相关：
#define  USER_INT0  0x00
#define  USER_INT1  0x20
#define  USER_INT2  0x40
#define  USER_INT3  0x60
#define  USER_INT4  0x80
#define  USER_INT5  0xA0
#define  USER_INT6  0xD0
#define  USER_INT7  0xE0

Uart计算配置网页小工具
http://software-dl.ti.com/msp430/msp430_public_sw/mcu/msp430/MSP430BaudRateConverter/index.html

注意事项：
msp432属于m4f内核，port文件使用这个内核port
fpu的开启记得多加两宏定义

踩坑记录：
用FreeRTOS的时候，不能对systick做分频，不然会进硬件错误