ʾ��Ŀ�ģ�
          ����GTIM������ҵ��Ƶ�PWM���Ρ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSI������Ƶ��48MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1��PCLK�ṩʱ�ӣ�ARRΪ��60100 -1����PWM��Ƶ��Ϊ48MHz/60100=800Hz
          5. PA06����Ϊ������ã�GTIM1��CH1

��ʾ˵����
          GTIM1����Ϊ�Ƚ������PWMģʽ��PWM��Ƶ��Ϊ800Hz�����ж��У�ͨ�����ķ�ʽ
          �ı�GTIM1��CH1ͨ���ıȽ�ֵ��ʵ��PWM�����ҵ��ơ�
          
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
