ʾ��Ŀ�ģ�
          SPI����ͨ��ʾ������DMA����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC��PLL
          3. ϵͳʱ������ΪHSIOSCʱ��3��Ƶ��PLL��4��Ƶ��64MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=64MHz
          4. SPI��������ΪPCLK��4��Ƶ��16Mbps
		  5. DMA��Ӳ������ BLOCK ����ģʽ

��ʾ˵����
          - 
		  #define  SPI_MASTER //����ģʽ
          SPI_MASTERͨ��DMA����TxBuffer�������е����ݣ�ͨ��DMA�������ݵ�RxBuffer��������
		  DMA������ɺ�LED1�����Ƚ�TxBuffer��RxBuffer���������һ�£���LED2����
		  
		  -
		  #define  SPI_SLAVE  //�ӻ�ģʽ
		  SPI_SLAVEͨ��DMA����TxBuffer�������е����ݣ�ͨ��DMA�������ݵ�RxBuffer��������
		  DMA������ɺ�LED1�����Ƚ�TxBuffer��RxBuffer���������һ�£���LED2����

Ӳ�����ӣ�
          SPI_MASTER SCK  (PA02)-- SPI_SLAVE SCK  (PA02)
          SPI_MASTER MISO (PA00)-- SPI_SLAVE MISO (PA00)
		  SPI_MASTER MOSI (PA01)-- SPI_SLAVE MOSI (PA01)
          SPI_MASTER CS   (PA03)-- SPI_SLAVE CS   (PA03)

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��project.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
