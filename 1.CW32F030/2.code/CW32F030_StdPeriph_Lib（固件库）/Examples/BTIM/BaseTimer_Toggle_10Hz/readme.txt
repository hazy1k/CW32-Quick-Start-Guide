ʾ��Ŀ�ģ�
          ��ʾ������ʱ��BTIM��TOG������ܣ�����Ҫ�жϺͿ��߼Ĵ����Ĳ�������ȫ��Ӳ��ʵ�ַ����������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSE��16MHz
          3. ϵͳʱ������ΪPLLʱ�ӣ�ΪHSEƵ�ʵ�4��Ƶ��64MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=64MHz
          4. BTIM1��ʱ������ΪPCLK��1024��Ƶ��
          5. PA07,PA09,PA10����ΪPUSH-PULL���, ����ΪBTIM1��TOG���
          6. PB09����ΪPUSH-PULL���
          7. PB08����ΪPUSH-PULL���



��ʾ˵����
          BTIM1�ļ���ʱ��Ϊ64MHz/1024=62500Hz������ARR�Ĵ���Ϊ62500 / 20 - 1��BTIM1ÿ50ms���һ�Σ�
          ����󣬴����жϷ�������жϷ�������У�PB09�����ת��LED1��˸������ָʾ��
          PA07��PA09���TOGP��PA10���TOGN������ʾ�����۲Ⲩ�Ρ�
          

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
