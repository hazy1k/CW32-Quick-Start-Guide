示例目的：
          SPI主从通信示例（带DMA）。

硬件资源：
          1. CW32F030C8T6 StartKit
          2. 时钟HSIOSC，PLL
          3. 系统时钟设置为HSIOSC时钟3分频，PLL的4倍频，64MHz，PCLK、HCLK不分频，PCLK=HCLK=SysClk=64MHz
          4. SPI，波特率为PCLK的4分频，16Mbps
		  5. DMA，硬件触发 BLOCK 传输模式

演示说明：
          - 
		  #define  SPI_MASTER //主机模式
          SPI_MASTER通过DMA发送TxBuffer缓冲区中的数据，通过DMA接收数据到RxBuffer缓冲区。
		  DMA传输完成后，LED1亮，比较TxBuffer和RxBuffer，如果数据一致，则LED2亮。
		  
		  -
		  #define  SPI_SLAVE  //从机模式
		  SPI_SLAVE通过DMA发送TxBuffer缓冲区中的数据，通过DMA接收数据到RxBuffer缓冲区。
		  DMA传输完成后，LED1亮，比较TxBuffer和RxBuffer，如果数据一致，则LED2亮。

硬件连接：
          SPI_MASTER SCK  (PA02)-- SPI_SLAVE SCK  (PA02)
          SPI_MASTER MISO (PA00)-- SPI_SLAVE MISO (PA00)
		  SPI_MASTER MOSI (PA01)-- SPI_SLAVE MOSI (PA01)
          SPI_MASTER CS   (PA03)-- SPI_SLAVE CS   (PA03)

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
