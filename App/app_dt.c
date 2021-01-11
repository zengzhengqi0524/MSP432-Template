/**
 * @file app_dt.c
 * @brief 与上位机通讯用模块
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-28
 * @copyright Evan-GH
 * @par 日志:
 */
#include "app_dt.h"
#include "bsp_uart.h"

//使用匿名上位机传输数据需要的宏定义，用于拆分一个整型数的高低位
#define BYTE0(Data)      (*(char *)(&Data))
#define BYTE1(Data)      (*((char *)(&Data)+1))
#define BYTE2(Data)      (*((char *)(&Data)+2))
#define BYTE3(Data)      (*((char *)(&Data)+3))

static uint8_t ANO_DT_Txbuffer[50] = {0};  //串口发送用缓存，匿名上位机用
/**
 * @brief 向匿名上位机发送用户自定义数据
 * @param _Frame_ID 帧ID
 * @param Data1 数据1
 * @param Data2 数据2
 * @param Data3 数据3
 * @return HAL_StatusTypeDef 发送结果
 * @retval HAL_OK 发送成功
 * @retval HAL_ERROR 发送失败
 */
void app_dt_ANO_Send_UserData(uint8_t _Frame_ID, float Data1, float Data2, float Data3)
{
	uint8_t cnt = 0;
	ANO_DT_Txbuffer[cnt++] = 0xAA;  //帧头
	ANO_DT_Txbuffer[cnt++] = 0xFF;  //目标地址
	ANO_DT_Txbuffer[cnt++] = _Frame_ID;  //发送用户自定义数据
	ANO_DT_Txbuffer[cnt++] = 12;  //数据长度

	int32_t temp = Data1;  //填充Data1
	ANO_DT_Txbuffer[cnt++] = BYTE0(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE1(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE2(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE3(temp);
	temp = Data2;  //填充Data2
	ANO_DT_Txbuffer[cnt++] = BYTE0(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE1(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE2(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE3(temp);
	temp = Data3;  //填充Data3
	ANO_DT_Txbuffer[cnt++] = BYTE0(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE1(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE2(temp);
	ANO_DT_Txbuffer[cnt++] = BYTE3(temp);

	uint8_t sc=0,ac=0;  //校验
	for(int i=0;i<ANO_DT_Txbuffer[3]+4;i++)
	{
		sc += ANO_DT_Txbuffer[i];
		ac += sc;
	}
	ANO_DT_Txbuffer[cnt++] = sc;
	ANO_DT_Txbuffer[cnt++] = ac;

	bsp_uart0_transmitString_DMA((uint8_t *)&ANO_DT_Txbuffer,cnt);
}

//卸载宏定义
#undef BYTE0
#undef BYTE1
#undef BYTE2
#undef BYTE3

