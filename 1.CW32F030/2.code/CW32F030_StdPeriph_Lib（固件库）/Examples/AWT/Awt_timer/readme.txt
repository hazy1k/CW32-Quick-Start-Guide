ʾ��Ŀ�ģ�
          ����AWT��ʱ���Թ̶���ʱ���������жϣ������ж��п��ƿ��ߵ�ƽ��ת

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. AWT��ʱ������ΪHSIOSC��32768��Ƶ
          5. PB08��PB09����ΪPUSH-PULL���

��ʾ˵����
          AWT��48/32768 MHzʱ�ӽ��м���������ARR�Ĵ���Ϊ145����AWT��ÿ100ms���һ�Σ������������־������ѭ����ѯ��������־�������־��Ч�������־��������PB08��PB09�˿ڷ�ת��

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
