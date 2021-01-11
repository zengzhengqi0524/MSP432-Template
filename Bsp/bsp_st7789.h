//////////////////////////////////////////////////////////////////////////////////
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中景园电子
//店铺地址：http://shop73023976.taobao.com/?spm=2013.1.0.0.M4PqC2
//
//  文 件 名   : main.c
//  版 本 号   : v2.0
//  作    者   : HuangKai
//  生成日期   : 2014-0101
//  最近修改   :
//  功能描述   : OLED 4接口演示例程(51系列)
//              说明:
//              ----------------------------------------------------------------
//              GND    电源地
//              VCC  接5V或3.3v电源
//              CLK   接PB13（SCL）
//              MOSI   接PB15（SDA）
//              RES  接PB11
//              DC   接PB10
//							BLK  接PB12 可以悬空
//							MISO 可以不接
//              ----------------------------------------------------------------
// 修改历史   :
// 日    期   :
// 作    者   : HuangKai
// 修改内容   : 创建文件
//版权所有，盗版必究。
//Copyright(C) 中景园电子2014/3/16
//All rights reserved
//******************************************************************************/
#ifndef __OLED_H
#define __OLED_H
#include "stdlib.h"
#include "gpio.h"
#define LCD_W 240
#define LCD_H 240

//-----------------OLED端口定义----------------
#define OLED_RST_Pin GPIO_PIN6
#define OLED_RST_GPIO_Port GPIO_PORT_P5
#define OLED_DC_Pin GPIO_PIN7
#define OLED_DC_GPIO_Port GPIO_PORT_P5
#define OLED_BLK_Pin GPIO_PIN0
#define OLED_BLK_GPIO_Port GPIO_PORT_P3

#define OLED_RST_Clr() GPIO_setOutputLowOnPin(OLED_RST_GPIO_Port,OLED_RST_Pin)//RES
#define OLED_RST_Set() GPIO_setOutputHighOnPin(OLED_RST_GPIO_Port,OLED_RST_Pin)

#define OLED_DC_Clr() GPIO_setOutputLowOnPin(OLED_DC_GPIO_Port,OLED_DC_Pin)//DC
#define OLED_DC_Set() GPIO_setOutputHighOnPin(OLED_DC_GPIO_Port,OLED_DC_Pin)

#define OLED_BLK_Clr()  GPIO_setOutputLowOnPin(OLED_BLK_GPIO_Port,OLED_BLK_Pin)//BLK
#define OLED_BLK_Set()  GPIO_setOutputHighOnPin(OLED_BLK_GPIO_Port,OLED_BLK_Pin)

#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

extern uint16_t BACK_COLOR, POINT_COLOR;   //背景色，画笔色

void Lcd_Init(void);
void LCD_Clear(uint16_t Color);
void Address_set(unsigned int x1,unsigned int y1,unsigned int x2,unsigned int y2);
void LCD_WR_DATA8(char da); //发送数据-8位参数
void LCD_WR_DATA(int da);
void LCD_WR_REG(char da);

void LCD_DrawPoint(uint16_t x,uint16_t y);//画点
void LCD_DrawPoint_big(uint16_t x,uint16_t y);//画一个大点
uint16_t  LCD_ReadPoint(uint16_t x,uint16_t y); //读点
void Draw_Circle(uint16_t x0,uint16_t y0,uint8_t r);
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_ShowChar(uint16_t x,uint16_t y,uint8_t num,uint8_t mode);//显示一个字符
void LCD_ShowNum(uint16_t x,uint16_t y,uint32_t num,uint8_t len);//显示数字
void LCD_Show2Num(uint16_t x,uint16_t y,uint16_t num,uint8_t len);//显示2个数字
void LCD_ShowString(uint16_t x,uint16_t y,const uint8_t *p);		 //显示一个字符串,16字体

void showhanzi(unsigned int x,unsigned int y,unsigned char index);
//画笔颜色
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红色
#define GRAY  			 0X8430 //灰色
//GUI颜色
#define DARKBLUE      	 0X01CF	//深蓝色
#define LIGHTBLUE      	 0X7D7C	//浅蓝色
#define GRAYBLUE       	 0X5458 //灰蓝色
//以上三色为PANEL的颜色

#define LIGHTGREEN     	 0X841F //浅绿色
#define LGRAY 			 0XC618 //浅灰色(PANNEL),窗体背景色

#define LGRAYBLUE        0XA651 //浅灰蓝色(中间层颜色)
#define LBBLUE           0X2B12 //浅棕蓝色(选择条目的反色)

#endif
