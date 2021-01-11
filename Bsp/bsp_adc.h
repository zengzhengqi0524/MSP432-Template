/**
 * @file bsp_adc.h
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-27
 * @copyright Evan-GH
 */
#ifndef __BSP_ADC_H
#define __BSP_ADC_H

//三个adc通道采样的原始值
extern uint16_t bsp_adc_OriginalData[3][50];
//数组储存指针
extern uint8_t bsp_adc_index;

void bsp_adc_init(void);
void bsp_adc_calVoltage(void);

#endif
