#include "adc.h"

uint16_t ADC_Value[10] = {0}; // 存放ADC采集值

void ADC_Mode_Init(void)
{
    /* 采集通道-PA05 */
    __RCC_GPIOA_CLK_ENABLE();
    __RCC_ADC_CLK_ENABLE();
    PA05_ANALOG_ENABLE(); // 设置PA05为模拟输入
    /* ADC基础配置 */
    ADC_InitTypeDef ADC_InitStructure;
    ADC_InitStructure.ADC_OpMode = ADC_SingleChOneMode; // 单通道单次转换模式
    ADC_InitStructure.ADC_ClkDiv = ADC_Clk_Div128; 
    ADC_InitStructure.ADC_SampleTime = ADC_SampTime5Clk; // 采样时间 = 5个ADC时钟周期
    ADC_InitStructure.ADC_VrefSel = ADC_Vref_VDDA; // 参考电压为VDDA
    ADC_InitStructure.ADC_InBufEn = ADC_BufEnable; // 开启跟随器
    ADC_InitStructure.ADC_TsEn = ADC_TsDisable; // 关闭内部温度传感器
    ADC_InitStructure.ADC_DMAEn = ADC_DmaEnable; // ADC转换完成触发DMA传输
    ADC_InitStructure.ADC_Align = ADC_AlignRight; // 右对齐
    ADC_InitStructure.ADC_AccEn = ADC_AccDisable; // 禁止累加器
    ADC_Init(&ADC_InitStructure);
    CW_ADC -> CR1_f.DISCARD = FALSE; // 数据覆盖更新
    CW_ADC -> CR1_f.CHMUX = ADC_ExInputCH5; // 配置ADC输入通道5
    ADC_ClearITPendingBit(ADC_IT_EOC); 
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
    ADC_EnableNvic(ADC_INT_PRIORITY);
}

void DMA_Mode_Init(void)
{
    __RCC_DMA_CLK_ENABLE();
    DMA_InitTypeDef DMA_InitStructure;
    DMA_StructInit(&DMA_InitStructure);
    DMA_InitStructure.DMA_Mode = DMA_MODE_BLOCK; // 使用BLOCK触发
    DMA_InitStructure.DMA_TransferWidth = DMA_TRANSFER_WIDTH_16BIT; // 16位数据传输
    DMA_InitStructure.DMA_SrcInc = DMA_SrcAddress_Fix; // ADC数据地址固定
    DMA_InitStructure.DMA_DstInc = DMA_DstAddress_Increase; // 存储器地址递增
    DMA_InitStructure.DMA_TransferCnt = 10; // 自动转换10次
    DMA_InitStructure.DMA_SrcAddress = (uint32_t)(&(CW_ADC->RESULT0)); // 源地址为ADC数据寄存器地址
    DMA_InitStructure.DMA_DstAddress = (uint32_t)ADC_Value;
    DMA_InitStructure.TrigMode = DMA_HardTrig; // 硬件触发
    DMA_InitStructure.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE; // ADC触发
    DMA_Init(CW_DMACHANNEL1, &DMA_InitStructure); // 配置DMA通道1
    DMA_ClearITPendingBit(DMA_IT_ALL);
    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC, ENABLE);
    __disable_irq();
    NVIC_ClearPendingIRQ(DMACH1_IRQn);
    NVIC_EnableIRQ(DMACH1_IRQn);
    __enable_irq();
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE); // 软件启动ADC转换
}

void ADC_DMA_Init(void)
{
    ADC_Mode_Init();
    DMA_Mode_Init();
}

// 均值滤波
uint32_t adc_average(void)
{
	uint64_t averageValue = 0;
	uint32_t Count = 10;
	for(int i = 0; i < Count; i++)
	{
		uint32_t temp = ADC_Value[i]; // 读取ADC数据
		if(temp == 0)
		{
			Count--; 
			continue;
		}
		averageValue += temp;
	}
	return(averageValue/Count); // 计算平均值
}
