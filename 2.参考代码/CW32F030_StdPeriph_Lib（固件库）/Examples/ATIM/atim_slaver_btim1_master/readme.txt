ʾ��Ŀ�ģ�
          ��ʾATIM��Ϊ���豸�����BTIM���豸��ʹ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��12��Ƶ��4MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=4MHz
          4. ATIM���ؼ������ϼ���ģʽ������Ƶ������ʱ��4MHz,ARR����Ϊ39999��10ms���,
             �������С�����жϳ����з�תPB09�ĵ�ƽ�����
          5. ����UART1��Ϊ���Դ���,��ӡ������Ϣ(PA8->TXD),������115200
          6. PB09����ΪPUSH-PULL���.
          7. PB00����ΪBTIM1��TOGN.
          8. BTIM1ʹ���ڲ�ʱ��PCLK������4��Ƶ��Ƶ��Ϊ1MHz���������У�50ms���������ATIM����

��ʾ˵����
          �������к�BTIM1ÿ50ms�������ATIM����һ�μ�ʱ��ATIM��ʱ10ms��ֹͣ��
          BTIM1ÿ�������PB00�ĵ�ƽ��תһ�Σ�ATIM����󣬼�ʱֹͣ������תPB09�ĵ�ƽ
          ��PB00��PB09�ĳ�ʼ��ƽ��ͬ��PB09�Ĳ��α�PB00��ʱ10ms

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
