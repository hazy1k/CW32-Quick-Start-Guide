ʾ��Ŀ�ģ�
          ��ʾSWD�˿�����ΪGPIO��UART�˿ڹ��ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. PA13��PA14

��ʾ˵����
          �����ϵ�2S��PA13��PA14��Ĭ�ϵ�SWD�����л�ΪGPIO��PA13��PA14�ڵ�ƽ��ת10��
          �󣬹����л�ΪUART1��RXD��TXD.

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
