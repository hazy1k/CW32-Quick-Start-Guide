ʾ��Ŀ�ģ�
          ��ʾ������ʱ��BTIM�Ĵ����������ܡ�����BTIM��ENΪ0���������ź���Чʱ����ʱ������������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. BTIM1��ʱ������ΪPCLK��512��Ƶ����������ģʽ�����������½���
          5. PA12���ù�������ΪBTIM_ETR����Ϊ�ⲿ�����źŵ�����˿�
          6. PB09����ΪPUSH-PULL���
          7. PB08����ΪPUSH-PULL���
          8. PA01����Ϊ�ж�����ڣ��ж������½���


��ʾ˵����
          BTIM1����Ϊ��������ģʽ���������У���15625Hzʱ��Ƶ�ʼ���������ARR�Ĵ���Ϊ15624��
          BTIM1��ʱ��1s�����������󣬴����жϷ�������жϷ�������У�PB09����͵�ƽ��
          LED1��
          ʹ�öŰ��߽�PA01����PA12�����ӣ�������KEY1ʱ��������BTIM1����������ͬʱGPIOA��
          �жϷ�������У�PB09����ߵ�ƽ��LED1����
          ÿ����KEY1һ�Σ�LED1��1s��Ϩ��
          LED2������Ϊ��ѹָʾ�ȡ�
          

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
