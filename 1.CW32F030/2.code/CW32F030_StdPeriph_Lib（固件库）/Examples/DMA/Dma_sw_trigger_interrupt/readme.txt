ʾ��Ŀ�ģ�
          ͨ���������DMA��ʽʵ���ڴ浽�ڴ��DMA���䣺��CPU��������µ�Դ��ַBUFFER��Ŀ�ĵ�ַBUFFER��DMA���䡣

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz

��ʾ˵����
          ��ȷ����DMA�Ĵ��䷽ʽ������DMA��������жϲ�ʹ�ܣ�ͨ�������������DMA���䡣DMA��������жϷ���������ô�����ɱ�־TransOverFlagΪ1����ѭ���н��д�����ɱ�־TransOverFlag�жϣ�δ��ɴ���ʱLED�ƿ���������ɴ�����Ƚ�Ŀ�ĵ�ַBUFFER��Դ��ַBUFFER�������Ƿ���ͬ����ͬ��LED���������������ơ�ע�⣺����DMA������̷ǳ��죬LED�ƿ������̻�����������������������

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
