ʾ��Ŀ�ģ�
          ��ʾGPIO����ΪPCLK��HCLKʱ�ӵ�����š�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. PA3��ΪPCLK�����PA4��ΪHCLK�����PA5��ͨ���

��ʾ˵����
          PA3��PA4���8MHz��ʱ�ӣ�PA5��ת��

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
