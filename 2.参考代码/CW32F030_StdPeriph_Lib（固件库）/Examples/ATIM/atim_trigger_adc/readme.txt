ʾ��Ŀ�ģ�
          ��ʾATIM����ADC�����Ĺ��ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��ͨ��PLL��Ƶ��64MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=64MHz
          4. ATIM���ؼ������ϼ���ģʽ��64��Ƶ��ARR����Ϊ9999���������10ms, 
             ��OC3Bƥ���¼�����ʱ����ADC
          5. ADC ʹ��ͨ��8����ת����ת�������󴥷��ж�
          6. PB00����ΪADC������
          7. PB09����ΪPUSH-PULL���

��ʾ˵����
          �����У�ATIM��PWMģʽ������ATIM��OC3Bʱ�����ڴ���ADC

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
