ʾ��Ŀ�ģ�
          ��ʾGTIM��GATE���Ƽ�����

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSI������Ƶ��48MHz�� PCLK��HCLK����Ƶ��
             PCLK=HCLK=SysClk=48MHz
          4. GTIM1�ķ�Ƶϵ������Ϊ1024��ARR����Ϊ48000000/1024/5-1����GTIM1ÿ
             200ms���һ�Ρ�       
          5. ����PB2Ϊ���븴�ã�GTIM1_ETR, 
          6. ����PB09ΪPUSH-PULL���,����LED1
          7. ����GTIM1_ETR�ļ���Ϊ�͵�ƽ��Ч
          8. ����PA01Ϊ���롣
          9. ʹ��GTIM1���ж������

��ʾ˵����
          ʹ�öŰ��߽�PA01��PB2���ӣ�����KEY1���������֣���LED1��200ms�ļ����˸��
          
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
