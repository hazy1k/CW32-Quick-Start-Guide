ʾ��Ŀ�ģ�
          ����systick��ʱ�����100mS��ʱ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit  ����CW32F030CxT6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��2��Ƶ��24MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz

��ʾ˵����
        ����Ĭ��ʱ�����ã�����ʼ��systickΪ1mS��ʱ����systick�ж�32λ��uwTick�������г�����1���������Ϊ0��������������ͨ����ȡuwTick�Ĳ�ֵ����ʱ��������������systickʵ��100mS�Ķ�ʱ����LED����˸��

ʹ��˵����
+ EWARM
          1. ��project.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��systick_delay.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)
