ʾ��Ŀ�ģ�
          SPIȫ˫��ģʽ����ͨ�ţ���ѯ��ʽ����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��2��Ƶ��24MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. SPI��������ΪPCLK��8��Ƶ��3Mbps
		  5. PB09��LED1������ΪPUSH-PULL���
		  6. PB08��LED2������ΪPUSH-PULL���
		  

��ʾ˵����
          SPIz���ӻ�����TxBuffer2�������е�����д��DR�Ĵ�����
		  SPIy����������TxBuffer1�������е�����д��DR�Ĵ������������䡣
		  ������ɺ�SPIz��ȡDR�Ĵ������洢��RxBuffer2��������SPIy��ȡDR�Ĵ������洢��RxBuffer1��������
		  �Ƚ�TxBuffer1��RxBuffer2��TxBuffer2��RxBuffer1��
          �������һ�£�LED1��������LED2����

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
