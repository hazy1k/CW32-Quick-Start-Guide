#include "adc.h"

void ADCx_Init(void)
{
    /* 采集通道-PA05 */
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_ADC_CLK_ENABLE();
    /* ADC基础配置 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_OpMode = ADC_SingleChOneMode; // 单通道单次转换模式
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div4; // ADC时钟频率 = 64MHz / 4 = 16MHz
    ADC_InitStructure.ADC_SampleTime = ADC_SampTime5Clk; // 采样时间 = 5个ADC时钟周期
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_VDDA; // 参考电压为VDDA
    ADC_InitStructure.ADC_InBufEn = ADC_BufDisable; // 关闭跟随器
    ADC_InitStructure.ADC_TsEn = ADC_TsDisable; // 关闭内部温度传感器
    ADC_InitStructure.ADC_DMAEn = ADC_DmaDisable;
    ADC_InitStructure.ADC_Align = ADC_AlignRight; // 右对齐
    ADC_InitStructure.ADC_AccEn = ADC_AccDisable; // 禁止累加器
    /* ADC模式看门狗配置 */
    ADC_WdtTypeDef ADC_WdtStructure;
    ADC_WdtInit(&ADC_WdtStructure); // 初始化看门狗结构体
    /* ADC单通道配置 */
    ADC_SingleChTypeDef ADC_SingleChStructure;
    ADC_SingleChStructure.ADC_DiscardEn = ADC_DiscardNull; // 单通道ADC转换结果溢出保存
    ADC_SingleChStructure.ADC_Chmux = ADC_ExInputCH5; // 选择通道5，AIN5:PA05
    ADC_SingleChStructure.ADC_InitStruct = ADC_InitStructure; // 选择ADC基础配置
    ADC_SingleChStructure.ADC_WdtStruct = ADC_WdtStructure; // 选择ADC模式看门狗配置
    ADC_SingleChOneModeCfg(&ADC_SingleChStructure); // 使能单通道模式
    ADC_Enable();
}

// 读取一次ADC采集值
uint32_t ADC_GET(void)
{
    ADC_SoftwareStartConvCmd(ENABLE); // 软件启动ADC转换
    uint32_t adc_value = ADC_GetConversionValue(); // 获取ADC采集值
    return adc_value;
}
// 均值滤波
uint32_t adc_average(uint8_t times)
{
    uint32_t value = 0;
    uint8_t t;
    for(t = 0; t < times; t++)
    {
        uint32_t temp = ADC_GET(); // 获取ADC采集值
        if(temp == 0) // 采集值异常
        {
            times--; // 采集次数减1
            continue; // 跳过本次采集
        }
        value += temp; // 累加采集值
    }
    return value/times; // 计算均值
}
