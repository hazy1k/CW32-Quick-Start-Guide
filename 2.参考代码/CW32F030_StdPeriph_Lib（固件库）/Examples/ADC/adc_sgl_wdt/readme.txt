ʾ��Ŀ�ģ�
          ��ʾADCģ�鵥ͨ��ģ�⿴�Ź����ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. ADC��ʱ��ԴΪPCLK��1��Ƶ
          5. PB08/09����ΪPUSH-PULL���

��ʾ˵����
          �������ADCģ����е�ͨ������ת����
          ADCģ�⿴�Ź�ͨ��ΪAIN1:PA01������ֵΪ0x600��
          ��ӿɵ�ֱ����Դ��PA01��ADCת����ɣ��������ֵС������ֵ����PB09�����ƽ��ת��

ʹ��˵����
+ EWARM
          1. ��adc_sgl_wdt.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��adc_sgl_wdt.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)