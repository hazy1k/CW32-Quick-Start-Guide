ʾ��Ŀ�ģ�
          ��ʾ��ʱ���ⲿ��������ADCת������ʱ1s����AIN1������PWM�������ADC����ֵ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪPLL��PLLʱ��ԴΪHSIʱ��6��Ƶ��8MHz��PLL8��Ƶ��PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=64MHz
          4. GTIM1��GTIM2��ʱ��ԴΪPCLK��1024��Ƶ
          5. PB09����ΪPUSH-PULL���
          6. ADC��ʱ��ԴΪPCLK��128��Ƶ

��ʾ˵����
          ͨ�ö�ʱ��GTIM1��ʱ1s�Զ�����ADCģ�����ת����ADCͨ��ΪAIN1:PA01��
		  ͨ�ö�ʱ��GTIM2��AIN1��ADC����ֵת����Ƶ�ʿɱ��PWM������ռ�ձ�50%��
		  ADC����ֵΪ0ʱ��PWM����Ƶ��Ϊ1Hz��ADC����ֵΪ4095ʱ��PWM����Ƶ��Ϊ5KHz��
          PB09�����ƽ��ת��LED1��0.5Hz��Ƶ����˸��

ʹ��˵����
+ EWARM
          1. ��adc_sgl_pwm_out.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��adc_sgl_pwm_out.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)