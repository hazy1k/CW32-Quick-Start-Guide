ʾ��Ŀ�ģ�
          SPIͨ��DMA�շ�����ʾ����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��2��Ƶ��24MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. SPI��������ΪPCLK��8��Ƶ��3Mbps
		  5. DMA��Ӳ������ BLOCK ����ģʽ
		  6. PB09��LED1������ΪPUSH-PULL���
		  7. PB08��LED2������ΪPUSH-PULL���

��ʾ˵����
          SPIy����������ѯ��ʽ��TXE������TxBuffer1�������е����ݣ�
		  SPIz���ӻ���ͨ��DMA�������ݣ��洢��RxBuffer2��������
		  
		  SPIy����������ѯ��ʽ��TXE������TxBuffer1�������е����ݣ�
		  ͬʱSPIz���ӻ���ͨ��DMA����TxBuffer2�������е����ݣ�
		  SPIy����������ѯ��ʽ��RXNE���������ݣ����洢��RxBuffer1��������
		  
		  �Ƚ�TxBuffer1��RxBuffer2��TxBuffer2��RxBuffer1��
		  �������һ�£���LED1��������LED2����

Ӳ�����ӣ�
          SPIy_SCK  (PA10) -- SPIz_SCK  (PB13)
          SPIy_MISO (PA11) -- SPIz_MISO (PB14)
          SPIy_MOSI (PA12) -- SPIz_MOSI (PB15)

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
