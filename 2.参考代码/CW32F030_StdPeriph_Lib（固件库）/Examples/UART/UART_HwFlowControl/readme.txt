ʾ��Ŀ�ģ�
          UARTӲ�����أ�RTS��CTS��ʾ����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��6��Ƶ��8MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. UART�Ĵ���ʱ������ΪPCLK

��ʾ˵����
          UART����TxBuffer�������е�������PC��
		  PC�������ݣ�UART�������ݲ��洢��RxBuffer��������
		  UART��RxBuffer�������е����ݻش���PC��
		  ��UART���յ�'\n'�����������������RxBufferSize�󣬴��������

Ӳ�����ӣ�
          UART1_TXD (PA08) -- PC_RX
          UART1_RXD (PA09) -- PC_TX
          UART1_CTS (PA10) -- CTS
          UART1_RTS (PA11) -- RTS

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
