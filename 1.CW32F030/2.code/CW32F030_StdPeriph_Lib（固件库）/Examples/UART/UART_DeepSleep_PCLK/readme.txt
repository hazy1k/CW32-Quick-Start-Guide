ʾ��Ŀ�ģ�
          UART�������ģʽʾ��������ʱ��ΪPCLK����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��6��Ƶ��8MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. UART�Ĵ���ʱ������ΪPCLK
		  5. PB09��LED1������ΪPUSH-PULL���
		  6. UART1_RXD(PA09)�½����ж�

��ʾ˵����
          ��������һ��ʱ�������������ģʽ��
		  PC�������ݿɻ���MCU�����Ѻ�UART��ѯ�������ݣ����洢��TxRxBuffer��������
		  UART���յ�'\n'���ٽ������ݣ�Ȼ��TxRxBuffer�������е����ݻش���PC��
		  ���������LED1��˸5s�����ٴν����������ģʽ��

Ӳ�����ӣ�
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
