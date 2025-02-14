# 第九章 ADC-电压采集

## 1. ADC介绍

CW32x030 内部集成一个 12 位精度、最高 1M SPS 转换速度的逐次逼近型模数转换器 (SAR ADC)，最多可将 16路模拟信号转换为数字信号。现实世界中的绝大多数信号都是模拟量，如光、电、声、图像信号等，都要由 ADC转换成数字信号，才能由 MCU 进行数字化处理。  
多种转换模式，全部支持转换累加功能：

- 单次转换
- 多次转换
- 连续转换
- 序列扫描转换
- 序列断续转换

## 2. 软件设计

### 2.1 编程大纲

1. ADC单次采集配置

2. ADC采集值处理函数

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 ADC配置

```c
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
```

#### 2.2.2 ADC采集值处理(均值滤波)

```c
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

```

#### 2.2.3 主函数测试

```c
#include "main.h"
#include "systick.h"
#include "uart.h"
#include "adc.h"

float32_t ADC_Result;

int32_t main(void)
{
	SysTick_Init();
	uart1_init();
	ADCx_Init();
	while(1)
	{
		ADC_Result = (float32_t)adc_average(10)/4096*3.3;
		printf("ADC Result: %.2f V\r\n", ADC_Result);
		printf("ADC Value: %d\r\n", adc_average(10));
		delay_ms(2000);
	}
}

```
