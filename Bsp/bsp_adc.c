/**
 * @file bsp_adc.c
 * @brief ADC班级支持包
 * @author Evan-GH (751191269@qq.com)
 * @version 1.0
 * @date 2020-09-27
 * @copyright Evan-GH
 * @par 日志:
 */
#include "rom.h"
#include "rom_map.h"
#include "driverlib.h"
#include "msp432.h"
#include "timer_a.h"

//三个adc通道采样的原始值
uint16_t bsp_adc_OriginalData[3][50] = {0};
//数组储存指针
uint8_t bsp_adc_index = 0;
//均值滤波后的结果
float bsp_adc_AvgVoltage[3] = {0.0f};

//Timer_A定时器配置
const Timer_A_UpModeConfig upModeConfig =
{
        TIMER_A_CLOCKSOURCE_SMCLK,            //SMCLK = 48M
        TIMER_A_CLOCKSOURCE_DIVIDER_48,       //48M/48 =1M
        20,                                 //1M/1000/50 = 20
        TIMER_A_TAIE_INTERRUPT_DISABLE,      // Disable Timer ISR
        TIMER_A_CCIE_CCR0_INTERRUPT_DISABLE, // Disable CCR0
        TIMER_A_DO_CLEAR                     // Clear Counter
};

const Timer_A_CompareModeConfig compareConfig =
{
        TIMER_A_CAPTURECOMPARE_REGISTER_1,          // Use CCR1
        TIMER_A_CAPTURECOMPARE_INTERRUPT_DISABLE,   // Disable CCR interrupt
        TIMER_A_OUTPUTMODE_SET_RESET,               // Toggle output but
        10                                       // 20/2
};

/**
 * @brief adc初始化
 * @note 采用定时器触发转换，三通道采集并转换，1MS采集50次，使用P5.5 P5.4 P5.3三个通道进行采集
 */
void bsp_adc_init(void)
{
    //使能adc
    ROM_ADC14_enableModule();
    //工作在24Mhz，adc最大工作频率25M
    ROM_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_2, ADC_NOROUTE);
    //使用14位ADC
    ROM_ADC14_setResolution(ADC_14BIT);
    //配置P5.5,P5.4,P5.3为输入引脚
    ROM_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P5, GPIO_PIN5 | GPIO_PIN4 | GPIO_PIN3,GPIO_TERTIARY_MODULE_FUNCTION);
    //储存在A0到A2
    ROM_ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM2, true);
    //配置AD采样存储器
    ROM_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A0, false);
    ROM_ADC14_configureConversionMemory(ADC_MEM1, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A1, false);
    ROM_ADC14_configureConversionMemory(ADC_MEM2, ADC_VREFPOS_AVCC_VREFNEG_VSS, ADC_INPUT_A2, false);

    //定时器配置
    ROM_Timer_A_configureUpMode(TIMER_A0_BASE, &upModeConfig);
    ROM_Timer_A_initCompare(TIMER_A0_BASE, &compareConfig);
    //设置触发源
    ROM_ADC14_setSampleHoldTrigger(ADC_TRIGGER_SOURCE1, false);
    //设置中断
    ROM_ADC14_enableInterrupt(ADC_INT2);
    ROM_ADC14_enableConversion();

    ROM_Interrupt_enableInterrupt(INT_ADC14);
    //优先级7
    ROM_Interrupt_setPriority(INT_ADC14,0XE0);

    //开启定时器
    ROM_Timer_A_startCounter(TIMER_A0_BASE, TIMER_A_UP_MODE);
}

/**
 * @brief 均值滤波器
 * 去掉最大值和最小值后求平均值
 * @param data 要处理的数据
 * @param num 数据个数
 * @return float 处理结果
 */
static float bsp_adc_MidSumFilter(uint16_t *data,uint32_t num)
{
	uint8_t i=0;
	uint32_t max=0,min=0;
	uint32_t sum=0;
	float average=0;

	max = *(data+0);
	min = *(data+0);
	sum += *(data+0);

	for (i=1; i<num; i++)
    {
		if (max < *(data+i)) max = *(data+i);
		if (min > *(data+i)) min = *(data+i);
		sum += *(data+i);
	}
	sum = sum - max - min;
	average = (float)sum/(num-2);
	return 	average;
}

/**
 * @brief 均值滤波并计算各通道的电压值
 */
void bsp_adc_calVoltage(void)
{
    for(int i=2; i>=0; i--)
    {
        bsp_adc_AvgVoltage[i] = bsp_adc_MidSumFilter(bsp_adc_OriginalData[i],50) * 0.000201416015625f; //3.3/16348
    }
}
