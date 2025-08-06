# 实验二：按键检测

## 1. 硬件设计

CW32F030电压电流表算上核心板一共有四个按键，一个复位和三个用户按键，复位作为单片机的特殊功能，不可以作为按键使用，故只有用户按键可以作为按键使用。

![](https://wiki.lckfb.com/storage/images/zh-hans/dwx-cw32f030c8t6/training/voltammeter-bootcamp/voltammeter/voltammeter_20240805_200445.png)

## 2. 软件设计

### 2.1 编程大纲

1. KEY GPIO配置

2. 按键扫描函数

3. 主函数测试

### 2.2 代码分析

#### 2.2.1 GPIO配置

```c
void KEY_Init(void)
{
  __RCC_GPIOB_CLK_ENABLE();
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.Pins = GPIO_PIN_14;
  GPIO_InitStructure.Mode = GPIO_MODE_INPUT_PULLUP;
  GPIO_InitStructure.IT = GPIO_IT_NONE;
  GPIO_Init(CW_GPIOB, &GPIO_InitStructure);
}
```

#### 2.2.2 按键扫描函数

```c
uint8_t KEY_Flag = 0;
uint8_t LED_Flag = 0;

void KEY_Scan(void)
{
	if(GPIO_ReadPin(CW_GPIOB, GPIO_PIN_14) == GPIO_Pin_RESET)
	{
		KEY_Flag = 1;
	}
	if(KEY_Flag)
	{
		if(GPIO_ReadPin(CW_GPIOB, GPIO_PIN_14))
		{
			KEY_Flag = 0;
			LED_Flag = 1 - LED_Flag;
		}
	}
}

void LED_Ctrl(void)
{
	if(LED_Flag)
	{
		LED_ON();
	}
	else
	{
		LED_OFF();
	}
}
```

#### 2.2.3 主函数测试

```c
int32_t main(void)
{
	LED_Init();
	SysTick_Init();
	KEY_Init();
	LED_ON();
	while(1)
	{
		KEY_Scan();
		LED_Ctrl();
	}
}
```