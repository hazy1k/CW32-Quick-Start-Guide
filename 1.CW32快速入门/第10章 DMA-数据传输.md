# 第十章 DMA-数据传输

## 1. DMA介绍

CW32x030 支持直接内存访问（DMA），无需 CPU 干预，即可实现外设和存储器之间、外设和外设之间、存储器和存储器之间的高速数据传输。DMA 控制器内部的优先级仲裁器，可实现 DMA 和 CPU 对外设总线控制权的裁，以及多 DMA 通道之间的调度执行。

每个通道都有专门的硬件DMA请求逻辑，同时可以由软件触发每个通道；传输的长度、传输的源地址和目标地址都可以通过软件单独设置。

关于DMA的主要特性在用户手册的第124页，如图所示（部分截图）。

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/beginner/dma-principle/dma-principle_20240628_144621.png)

DMA 支持 4 种传输模式：软件触发 BLOCK 传输模式，软件触发 BULK 传输模式，硬件触发 BLOCK 传输模式，硬件触发 BULK 传输模式。

- 软件触发 BLOCK 传输模式
  - DMA 通道每传输完成 1 个数据块，DMA 插入一个传输间隙，仲裁器在该传输间隙内进行传输优先级仲裁。如果在该传输间隙内，有 CPU 或更高优先级的 DMA 请求要访问当前 DMA 通道所占用的 AHB 设备，仲裁器会释放当前 DMA 通道对该设备的控制权，让 CPU 或者更高优先级的 DMA 通道优先访问；当 CPU 或更高优先级的 DMA通道释放设备访问权后，本 DMA 通道未完成的数据传输将继续进行。
- 软件触发 BULK 传输模式
  - 软件触发 BULK 传输模式没有传输间隙，在 DMA_CNTy.CNT 个数据块传输完成之前，该 DMA 通道将一直占用外设，CPU 或更高优先级的 DMA 通道只能等待该 DMA 传输完成后才能访问该外设。
- 硬件触发 BLOCK 传输模式
  - 同软件 BLOCK 传输模式一样，硬件 BLOCK 传输模式每传输完成 1 个数据块，DMA 插入一个传输间隙，允许CPU 或更高优先级的 DMA 通道传输请求抢占外设控制权。
- 硬件触发 BULK 传输模式
  - 硬件触发 BULK 传输模式没有传输间隙，在 DMA_CNTy.CNT 个数据块传输完成之前，DMA 将一直占据外设访问权，CPU 或更高优先级的 DMA 通道只能等待该操作完成后才能访问该外设。

## 2. 软件设计

### 2.1 编程大纲

1. 配置ADC单通道采集

2. 配置DMA传输数据

3. ADC和DMA中断服务函数

4. 均值滤波

5. 主函数测试

### 2.2 代码分析

#### 2.2.1 ADC通道配置

```c
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
```

#### 2.2.2 DMA配置

```c
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
```

#### 2.2.3 均值滤波

```c
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
```

#### 2.2.4 ADC及DMA中断服务函数

```c
void DMACH1_IRQHandler(void)
{
  if(DMA_GetITStatus(DMA_IT_TC1)) // 产生DMA中断
  {
    DMA_ClearITPendingBit(DMA_IT_TC1);
    CW_DMACHANNEL1->CNT = 0x10010; // 重置CNT计数
    CW_DMACHANNEL1->DSTADDR = (uint32_t)ADC_Value; // 重置目的地址
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);
  }
}

void ADC_IRQHandler(void)
{
  if(ADC_GetITStatus(ADC_IT_EOC) != RESET)
  {
    ADC_ClearITPendingBit(ADC_IT_EOC);
    ADC_SoftwareStartConvCmd(ENABLE);
  }
}
```

#### 2.2.5 主函数测试

```c
#include "main.h"
#include "systick.h"
#include "uart.h"
#include "adc.h"

float32_t ADC_Result;
extern uint16_t ADC_Value[10];

int32_t main(void)
{
	SysTick_Init();
	uart1_init();
	ADC_DMA_Init();
	while(1)
	{
		ADC_Result = (float32_t)adc_average()/4096*3.3;
		printf("ADC Value: %d\r\n", adc_average());
		printf("ADC Result: %.2f\r\n", ADC_Result);
		delay_ms(2000);
	}
}

```
