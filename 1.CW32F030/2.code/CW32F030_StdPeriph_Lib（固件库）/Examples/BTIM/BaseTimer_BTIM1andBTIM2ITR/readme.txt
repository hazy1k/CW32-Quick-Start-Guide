ʾ��Ŀ�ģ�
          ���ƻ�����ʱ��BTIM1��BTIM2�ڲ���������չ��ʱ���ļ���/��ʱ��Χ

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. BTIM1��ʱ������ΪPCLK��8��Ƶ
          5. BTIM2��ʱ��ΪBTIM1��OV�źţ�����Ƶ
          6. PB09����ΪPUSH-PULL���
          7. PB08����ΪPUSH-PULL���
          8. PA05����ΪBTIM2��TOGP���

��ʾ˵����
          BTIM1��1MHzʱ�ӽ��м���������ARR�Ĵ���Ϊ49999����BTIM1��ÿ50ms���һ�Σ�������
          �жϷ�������жϷ�������У�PB09�����ƽ��ת��LED1��10Hz��Ƶ����˸��
          BTIM2��BTIM1������ź�OV���м���������ARR�Ĵ���Ϊ9����BTIM1��ÿ500ms���һ�Σ�������
          �жϷ�������жϷ�������У�PB08�����ƽ��ת��LED2��1Hz��Ƶ����˸��

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
