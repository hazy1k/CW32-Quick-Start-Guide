ʾ��Ŀ�ģ�
          ��ʾ��ʱ���ⲿ��������ADCת������ʱ1s����оƬ�¶ȡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. GTIM1��ʱ��ԴΪPCLK��256��Ƶ
          5. PB09����ΪPUSH-PULL���
          6. ADC��ʱ��ԴΪPCLK��2��Ƶ

��ʾ˵����
          ͨ�ö�ʱ��GTIM1��ʱ1s�Զ�����ADCģ�����ת����ADCͨ��ΪMCU�ڲ��¶ȴ�����TS��
          PB09�����ƽ��ת��LED1��0.5Hz��Ƶ����˸��

ʹ��˵����
+ EWARM
          1. ��adc_sgl_ext_trig.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��adc_sgl_ext_trig.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)