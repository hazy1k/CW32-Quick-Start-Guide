ʾ��Ŀ�ģ�
          ����HCLKʱ��������ܽ�HCLKʱ�Ӵ�PA04���������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz

��ʾ˵����
        ����Ĭ��ʱ�����ã�������HCLKʱ���������ʾ�����۲�PA04���ŵĲ��Σ�Ĭ�ϳ���Ϊ8MHz��

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��rcc_hclkout.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
