示例目的：
          模拟软件发生故障时，窗口看门狗WWDT复位系统的功能。

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为PLL，由HSI提供的8MHz时钟6倍频到48MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=48MHz
          4. WWDT由PCLK经262144分频，即5.46ms计数一次
          5. PB09设置为PUSH-PULL输出
          6. PB08设置为PUSH-PULL输出
          7. PA01设置为输入
          8. 系统节拍定时器设置为250ms产生一次中断

演示说明：
          系统时钟由HSI提供，通过PLL倍频到48MHz。配置WWDT喂狗窗口246~350ms（窗口范围0x52~0x40），允许预溢出中断，
          系统节拍配置为每 250 ms 产生一个中断，中断复位程序中闪烁 LED1（PB9），并喂狗。
          PA1 引脚连接的按钮S1配置为在其下降沿上产生中断。
          在 NVIC 中，将PA1对应中断向量且优先级等于0，并且 SysTick 中断向量优先级设置为1
          （PA1的中断优先于 SysTick 中断）。
          按钮KEY1（PA1）用于模拟软件故障：KEY1按下后，进入对应的中断服务程序，且中断标志
          位不清零，CPU不会进入系统节拍中断服务程序，因此不会重载WWDT计数器。
          当WWDT计数器预溢出时，WWDT将产生一个POV中断。
          程序发生WWDT中断后，LED1长亮，延后2个喂狗周期后，系统复位。
          若系统发生过WWDT复位，则LED2亮。
          
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
