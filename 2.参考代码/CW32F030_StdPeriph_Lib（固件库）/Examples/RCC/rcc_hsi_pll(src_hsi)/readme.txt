示例目的：
          系统时钟源在HSI和PLL（参考时钟为HSI）之间进行切换。

硬件资源：
          1. CW32F030C8T6 StartKit  或者CW32F030CxT6 StartKit
          2. 时钟HSI和PLL
          3. HSI作为系统时钟时，设置为HSIOSC到HSI时钟的分频比为2，则SysClk为24MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. PLL作为系统时钟时，PLL输入参考时钟选择HSI时钟（24MHz），经过2倍频，PLL输出48MHz时钟。PCLK、HCLK不分频，PCLK=HCLK=SysClk=48MHz，此时需要注意设置FLASH的读访问等待周期，否则程序无法运行。

演示说明：
        程序循环在HSI和PLL时钟之间进行切换，通过观察LED灯，正常切换则LED快闪（100mS亮，100mS熄），如果切换失败，则LED灯会慢闪（1S亮，1S熄）。

使用说明：
+ EWARM
          1. 打开project.eww文件
          2. 编译所有文件：Project->Rebuild all
          3. 载入工程镜像：Project->Debug
          4. 运行程序：Debug->Go(F5)

+ MDK-ARM
          1. 打开rcc_hsi_pll(src_hsi).uvproj文件
          2. 编译所有文件：Project->Rebuild all target files
          3. 载入工程镜像：Debug->Start/Stop Debug Session
          4. 运行程序：Debug->Run(F5)
