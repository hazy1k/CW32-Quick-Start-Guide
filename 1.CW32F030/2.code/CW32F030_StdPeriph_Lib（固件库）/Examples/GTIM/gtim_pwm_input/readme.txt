ʾ��Ŀ�ģ�
          ��ʾGTIM�����벶���ܣ�����PWM��Ƶ�ʺ�����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSI��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=8MHz
          4. GTIM1����Ƶ       
          5. ����PA06����ΪGTIM1_CH1, �����ش�������
          6. ����PA07����ΪGTIM1_CH2���½��ش�������
  
��ʾ˵����
          ʹ��GTIM1��CH1��CH2����ͨ����PWM�����źŽ��в�����CH1�������ڣ�
          CH2�������� ��ͬһ��PWM�����ź����뵽PA06��PA07�ϡ�
          
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
