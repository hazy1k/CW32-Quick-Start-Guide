ʾ��Ŀ�ģ�
          SPI����ģʽ����ͨ�ţ��жϷ�ʽ����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��2��Ƶ��24MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. SPI��������ΪPCLK��8��Ƶ��3Mbps
		  5. PB09��LED1������ΪPUSH-PULL���
		  6. PB08��LED2������ΪPUSH-PULL���
		  
��ʾ˵����
		  SPIy���������жϷ�ʽ����TxBuffer�������е����ݡ�
		  SPIz���ӻ����жϷ�ʽ�������ݣ����洢��RxBuffer��������
		  �Ƚ�TxBuffer��RxBuffer��
          �������һ�£�LED1��������LED2����

Ӳ�����ӣ�
          SPIy_SCK  (PA10) -- SPIz_SCK  (PB13)
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
