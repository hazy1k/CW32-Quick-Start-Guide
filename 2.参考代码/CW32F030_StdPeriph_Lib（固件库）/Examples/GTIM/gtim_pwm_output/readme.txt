ʾ��Ŀ�ģ�
          ģ�������������ʱ�����ڿ��Ź�WWDT��λϵͳ�Ĺ��ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪPLL����HSI�ṩ��8MHzʱ��6��Ƶ��48MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1�ķ�Ƶϵ������Ϊ16��ARR����Ϊ1499�������ʱ��Ϊ500us
          5. PB08����ΪGTIM1��CH3
         

��ʾ˵����
          ϵͳʱ����HSI�ṩ��ͨ��PLL��Ƶ��48MHz��GTIM1��16��Ƶ����3MHz��Ƶ�ʼ�����
          ARR����Ϊ1499��GTIM1���������Ϊ500us��GTIM1ÿ500us����һ���жϣ�ÿ50ms�ı�
          һ��CH3��CCR�Ĵ�����ֵ�����ı�PWM������������Ϊ5us���ȵ�����ARR��Ȼ��ݼ�
          ��0����˷�����LED2���𲽱�����Ȼ���𲽱䰵����˷�����
          
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
