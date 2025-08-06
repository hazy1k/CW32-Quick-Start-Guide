示例目的：
          演示ATIM作为主机，BTIM作为从机的级联使用

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSI
          3. 系统时钟设置为HSI时钟6分频，8MHz， PCLK、HCLK不分频，PCLK=HCLK=SysClk=8MHz
          4. ATIM边沿计数，上计数模式，不分频，ARR设置为0xFFFF，
             输入捕获, 使用CH1A双边沿捕获
          5. BTIM1以500KHz的频率计时,ARR设置为49,以5KHz的频率硬件翻转TOGP引脚
          6. PA07设置为BTIM1TOGP的输出。
          7. 串口UART1作为调试串口,打印调试信息(PA8->TXD),波特率115200
          8. PB02设置为ATIM_CH1A

演示说明：
          使用杜邦线将PA07和PB02连接, UART1将打印PA02引脚信号的周期和正脉宽.

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
