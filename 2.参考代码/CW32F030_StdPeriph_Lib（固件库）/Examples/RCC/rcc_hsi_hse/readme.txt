ʾ��Ŀ�ģ�
          ϵͳʱ��Դ��HSI��HSE֮������л���

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI��LSE
          3. HSI��Ϊϵͳʱ��ʱ������ΪHSIOSC��HSIʱ�ӵķ�Ƶ��Ϊ1����SysClkΪ48MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=48MHz��ע���ʱ��Ҫ��ȷ����FLASH�Ķ����ʵȴ����ڡ�
          4. HSE��Ϊϵͳʱ��ʱ��16MHz��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=16MHz��

��ʾ˵����
        ����ѭ����HSI��HSEʱ��֮������л��������л���LED������100mS����100mSϨ��������л�ʧ�ܣ���LED�ƻ�������1S����1SϨ����

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��rcc_hsi_hse.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
