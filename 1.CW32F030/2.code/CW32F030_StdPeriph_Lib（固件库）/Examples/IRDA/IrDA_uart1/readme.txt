ʾ��Ŀ�ģ�
          ��ʾGTIM1��UART1���ϲ���IrDA���������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��PLL 8��Ƶ��64MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=64MHz
          4. GTIM1����115.2kHz�ز�
          5. UART1��TXD����GTIM1�Ĳ������ز���

��ʾ˵����
         PB09���IrDA���ƵĲ��Ρ�

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
