ʾ��Ŀ�ģ�
          ϵͳʱ��Դ��HSI��PLL���ο�ʱ��ΪHSI��֮������л���

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI��PLL
          3. HSI��Ϊϵͳʱ��ʱ������ΪHSIOSC��HSIʱ�ӵķ�Ƶ��Ϊ2����SysClkΪ24MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. PLL��Ϊϵͳʱ��ʱ��PLL����ο�ʱ��ѡ��HSIʱ�ӣ�24MHz��������2��Ƶ��PLL���48MHzʱ�ӡ�PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=48MHz����ʱ��Ҫע������FLASH�Ķ����ʵȴ����ڣ���������޷����С�

��ʾ˵����
        ����ѭ����HSI��PLLʱ��֮������л���ͨ���۲�LED�ƣ������л���LED������100mS����100mSϨ��������л�ʧ�ܣ���LED�ƻ�������1S����1SϨ����

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��rcc_hsi_pll(src_hsi).uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
