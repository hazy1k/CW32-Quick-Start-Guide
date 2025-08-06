示例目的：
          演示ATIM的输出比较功能,生成PWM波形

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，8分频，计数时钟1MHz,ARR设置为9999，10ms溢出,
             输出比较, 使用ATIM的CH2B通道输出PWM信号。PWM的频率设为100Hz
          5. 串口UART1作为调试串口,打印调试信息(PA8->TXD),波特率115200
          6. PB14设置为ATIM_CH2B通道的输出.

演示说明：
          PB14推动一个LED灯，实现呼吸灯的效果。设定5s完成50个等级的改变.

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开project.uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
