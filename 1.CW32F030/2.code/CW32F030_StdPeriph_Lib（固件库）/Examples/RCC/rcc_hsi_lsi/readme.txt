ʾ��Ŀ�ģ�
          ϵͳʱ��Դ��HSI��LSI֮������л���

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI��LSI
          3. HSI��Ϊϵͳʱ��ʱ������ΪHSIOSC��HSIʱ�ӵķ�Ƶ��Ϊ2����SysClkΪ24MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. LSI��Ϊϵͳʱ��ʱ��32.8K��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=32.8KHz��

��ʾ˵����
        ����ѭ����HSI��LSIʱ��֮������л��������л���LED������100mS����100mSϨ��������л�ʧ�ܣ���LED�ƻ�������1S����1SϨ����

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��rcc_hsi_lsi.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
