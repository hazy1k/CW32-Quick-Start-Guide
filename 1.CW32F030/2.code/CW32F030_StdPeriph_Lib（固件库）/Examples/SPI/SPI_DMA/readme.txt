示例目的：
          SPI通过DMA收发数据示例。

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSIOSC
          3. 系统时钟设置为HSIOSC时钟2分频，24MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=24MHz
          4. SPI，波特率为PCLK的8分频，3Mbps
		  5. DMA，硬件触发 BLOCK 传输模式
		  6. PB09（LED1）设置为PUSH-PULL输出
		  7. PB08（LED2）设置为PUSH-PULL输出

演示说明：
          SPIy（主机）查询方式（TXE）发送TxBuffer1缓冲区中的数据，
		  SPIz（从机）通过DMA接收数据，存储到RxBuffer2缓冲区。
		  
		  SPIy（主机）查询方式（TXE）发送TxBuffer1缓冲区中的数据，
		  同时SPIz（从机）通过DMA发送TxBuffer2缓冲区中的数据，
		  SPIy（主机）查询方式（RXNE）接收数据，并存储到RxBuffer1缓冲区。
		  
		  比较TxBuffer1和RxBuffer2，TxBuffer2和RxBuffer1，
		  如果数据一致，则LED1亮，否则LED2亮。

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
