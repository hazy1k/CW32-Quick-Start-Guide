ʾ��Ŀ�ģ�
          ����PCLKʱ��������ܽ�PCLKʱ�Ӵ�PA03���������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��2��Ƶ��24MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz

��ʾ˵����
        ����Ĭ��ʱ�����ã�������PCLKʱ���������ʾ�����۲�PA03���ŵĲ��Σ�Ĭ�ϳ���Ϊ24MHz��

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��rcc_pclkout.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
