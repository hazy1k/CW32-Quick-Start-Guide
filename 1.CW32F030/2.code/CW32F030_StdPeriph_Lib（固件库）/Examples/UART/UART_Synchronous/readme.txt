ʾ��Ŀ�ģ�
          UART��SPI֮�����ͬ����˫��ģʽ��ͨѶ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��2��Ƶ��24MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. UART�Ĵ���ʱ������ΪPCLK��12��Ƶ
		  5. SPI��ʱ������ΪPCLK��2��Ƶ
          6. PB09��LED1������ΪPUSH-PULL���
		  7. PB08��LED2������ΪPUSH-PULL���

��ʾ˵����
          UART��ѯ��ʽ��TXE������TxBuffer1�����������ݵ�SPI��
          SPI��ѯ��ʽ��RXNE���������ݣ����洢��RxBuffer2��������

          SPI��ѯ��ʽ��TXE������TxBuffer2�����������ݵ�UART��
          UART��ѯ��ʽ��RC���������ݣ����洢��RxBuffer1��������

          �Ƚ�TxBuffer1��RxBuffer2��TxBuffer2��RxBuffer1��
          �������һ�£�LED1��������LED2����

Ӳ�����ӣ�
          UART1_TXD (PA08) -- SPI1_SCK  (PA10)
          UART1_RXD (PA09) -- SPI1_MISO (PA11)
          UART   CS (PA00) -- SPI1_CS   (PA15) (��ѡ Ӳ��NSS) 

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
