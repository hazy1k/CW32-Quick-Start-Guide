示例目的：
          控制GTIM输出正弦调制的PWM波形。

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI，不分频，48MHz， PCLK、HCLK不分频，
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1由PCLK提供时钟，ARR为（60100 -1），PWM的频率为48MHz/60100=800Hz
          5. PA06设置为输出复用，GTIM1的CH1

演示说明：
          GTIM1设置为比较输出的PWM模式，PWM的频率为800Hz。在中断中，通过查表的方式
          改变GTIM1的CH1通道的比较值，实现PWM的正弦调制。
          
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
