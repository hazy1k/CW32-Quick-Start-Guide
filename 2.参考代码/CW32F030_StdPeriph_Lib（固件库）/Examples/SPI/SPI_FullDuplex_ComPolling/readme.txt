示例目的：
          SPI全双工模式主从通信（查询方式）。

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟2分频，24MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. SPI，波特率为PCLK的8分频，3Mbps
		  5. PB09（LED1）设置为PUSH-PULL输出
		  6. PB08（LED2）设置为PUSH-PULL输出
		  

演示说明：
          SPIz（从机）将TxBuffer2缓冲区中的数据写入DR寄存器，
		  SPIy（主机）将TxBuffer1缓冲区中的数据写入DR寄存器，启动传输。
		  传输完成后，SPIz读取DR寄存器并存储到RxBuffer2缓冲区，SPIy读取DR寄存器并存储到RxBuffer1缓冲区。
		  比较TxBuffer1和RxBuffer2、TxBuffer2和RxBuffer1，
          如果数据一致，LED1亮，否则LED2亮。

硬件连接：
          SPIy_SCK  (PA10) -- SPIz_SCK  (PB13)
          SPIy_MISO (PA11) -- SPIz_MISO (PB14)
          SPIy_MOSI (PA12) -- SPIz_MOSI (PB15)

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
