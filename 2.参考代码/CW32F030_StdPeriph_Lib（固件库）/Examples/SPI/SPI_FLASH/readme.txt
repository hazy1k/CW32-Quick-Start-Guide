ʾ��Ŀ�ģ�
          SPI��д�ⲿFLASH��W25Q64����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��2��Ƶ��24MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. SPI��������ΪPCLK��8��Ƶ��3Mbps
		  5. UART�Ĵ���ʱ������ΪPCLK
		  6. PB09��LED1������ΪPUSH-PULL���
		  7. PB08��LED2������ΪPUSH-PULL���
		  

��ʾ˵����
          ���ⲿFLASH��DeviceID��ManufactDeviceID��JedecID��UniqueID����ͨ��UART��ӡ���������֡�
		  
		  ����ȡ��JedecID��ȷʱ��
		      д��TxBuffer�������е����ݵ�FLASH��1ҳ��
		      ��ȡFLASH��1ҳ���ݣ����洢��RxBuffer��������
		      ���д�������������������Ƿ�һ�£�
		          һ�£���ӡ��FLASH Success����LED1����
		          ���򣬴�ӡ��FLASH Error 1����LED2����
		      ����4KB��������ȡFLASH��1ҳ���ݣ����洢��RxBuffer��������
		      RxBuffer�����������ݴ��ڡ�1��ʱ����ӡ��FLASH Error 2����LED2����
			  
		  ����ȡ��JedecID����ȷʱ��
		      ��ӡ��FLASH Error 3����LED2����

Ӳ�����ӣ�
          SPI1_SCK  (PA10) -- FLASH SCK  
          SPI1_MISO (PA11) -- FLASH MISO
		  SPI1_MOSI (PA12) -- FLASH MOSI 
          SPI1_CS   (PA15) -- FLASH CS
		  
		  UART1_TXD (PA08) -- PC_RX
          UART1_RXD (PA09) -- PC_TX

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
