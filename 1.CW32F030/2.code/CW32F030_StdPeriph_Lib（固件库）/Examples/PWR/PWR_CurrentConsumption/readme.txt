ʾ��Ŀ�ģ�
          ��ʾMCUͨ����������/�˳����ߺ��������ģʽ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI��LSE
          3. ϵͳʱ��ԴΪHSIʱ��2��Ƶ��24MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=24MHz
          4. PA01����Ϊ����(KEY1)�ж�����
          5. PB08/09����ΪPUSH-PULL���

��ʾ˵����
          �ϵ��MCU������������ģʽ��PB09(LED1)�����ƽ��ʱ1s���Ϸ�ת��
		  ��һ�ΰ���KEY1��MCU�������߻����������ģʽ��LED1Ϩ��
		  ��һ�ΰ���KEY1��MCU�˳����߻����������ģʽ��LED1�����Ҳ��Ϸ�ת��MCU������������ģʽ��
		  ע�⣺�޸�cw32f030_pwr_modes.h�еĺ궨�壬����ѡ��͹���ģʽ��
		  
          
ʹ��˵����
+ EWARM
          1. ��PWR_CurrentConsumption.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��PWR_CurrentConsumption.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)