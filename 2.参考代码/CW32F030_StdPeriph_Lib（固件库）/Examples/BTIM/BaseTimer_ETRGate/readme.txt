ʾ��Ŀ�ģ�
          ��ʾ������ʱ��BTIM���ſع��ܡ�BTIM��ʹ���ź�EN����Ϊ��Чʱ�����ſ��ź���Ч����ʱ�����ż�����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. BTIM1��ʱ������ΪPCLK��8��Ƶ���ſ�ģʽ
          5. PA12���ù�������ΪBTIM_ETR����ΪGATE�źŵ�����˿�, PCLK,6���˲�
          6. PB09����ΪPUSH-PULL���
          7. PB08����ΪPUSH-PULL���
          8. BTIM2��ʱ������ΪPCLK�� 512��Ƶ
          9. PA05����ΪBTIM2��TOGP���


��ʾ˵����
          BTIM1����Ϊ�ſع��ܣ���1MHzʱ�ӽ��м���������ARR�Ĵ���Ϊ49999��
          BTIM2����Ϊ��ʱģʽ����15625Hzʱ��Ƶ�ʼ���������ARR�Ĵ���Ϊ15624������TOGP���ʹ�ܣ�
          PA05�ĵ�ƽÿ1s��תһ�Ρ�
          BTIM2ÿ�������һ�Σ�����󣬴����жϷ�������жϷ�������У�PB08�����ƽ��ת����
          ���ֺ�PA05��ͬ�ļ��ԣ�LED2��2Hz��Ƶ����˸��
          ʹ�öŰ��߽�PA05����PA12�����ӣ���PA05����ߵ�ƽʱ��LED2����BTIM1��ÿ50ms���һ�Σ�
          �������жϷ�������жϷ�������У�PB09�����ƽ��ת��LED1��10Hz��Ƶ����˸��
          ��PA05����͵�ƽʱ��LED2��BTIM1ֹͣ������LED1����˸��
          

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
