ʾ��Ŀ�ģ�
          GTIM��BTIM�ڲ���������ʵ�ֶ�ʱ���������Ƶ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSI������Ƶ��48MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=48MHz
          4. BTIM1��PCLK�ṩ����ʱ�ӣ�����ARRΪ48000-1����BTIM1 1ms���һ�Σ�          
          5. GTIM1�ļ���ʱ����BTIM1������ź�OV�ṩ������ARRΪ1000-1����GTIM1
             1s���һ�Ρ�
          6. ����PB09ΪPUSH-PULL���,����LED1

��ʾ˵����
          �������к�,LED1ÿ2s�ı�һ������״̬��
          
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
