ʾ��Ŀ�ģ�
          UART���ͨ��ģʽʾ����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSIOSC
          3. ϵͳʱ������ΪHSIOSCʱ��6��Ƶ��8MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. UART�Ĵ���ʱ������ΪPCLK
          5. PB09��LED1������ΪPUSH-PULL���
		  6. PB08��LED2������ΪPUSH-PULL���

��ʾ˵����
          USARTy�����������͵�ַ֡0xA5Ѱַ��USARTz���ӻ�����
		  
          USARTy����������ѯ��ʽ��TXE������TxBuffer1�������е����ݣ�
          USARTz���ӻ����жϷ�ʽ��������֡�����洢��RxBuffer2��������ͬʱ����TxBuffer2�������е����ݵ�USARTy����������
          USARTy����������ѯ��ʽ��RC���������ݣ����洢��RxBuffer1��������
		  
          �Ƚ�TxBuffer1��RxBuffer2��TxBuffer2��RxBuffer1��
          �������һ�£�LED1��������LED2����

Ӳ�����ӣ�
          UART1_TXD (PA08) -- UART2_RXD (PA07)
          UART1_RXD (PA09) -- UART2_TXD (PA06)

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
