ʾ��Ŀ�ģ�
          ��ʾ�������ADC��ͨ��ת����֧�ֵ�ͨ�����Ρ���Ρ�������3��ת��ģʽ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. ADC��ʱ��ԴΪPCLK��1����4��Ƶ
          5. PB08/09����ΪPUSH-PULL���

��ʾ˵����
          ADCͨ��ΪAIN1:PA01��
          ADCת����ɣ�PB09�����ƽ��ת��
          ���ת��ģʽʱ�����ʵ�����ת���������趨ֵ����ʱ��PB08/09ȫ��������
          ע�⣺�޸�main.h�е�ת��ģʽ�궨�壬����ʵ�ָ���ת��ģʽ�Ĳ��ԡ�

ʹ��˵����
+ EWARM
          1. ��adc_sgl_sw.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��adc_sgl_sw.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)