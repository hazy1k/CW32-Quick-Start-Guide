ʾ��Ŀ�ģ�
          ��ʾVCģ�������ź��½��ش����жϹ��ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. VC���˲�ʱ��ԴΪRC150K
          5. PB08/09����ΪPUSH-PULL���

��ʾ˵����
          VC����Pͨ��ΪPA00��Nͨ��ΪBGR��׼��ѹ1.2V��VC����˿�ΪPA11��
          ��ӿɵ�ֱ����Դ����VC����Pͨ����ѹ����Nͨ����ѹʱ�̣�PB09��LED�������ƽ��תһ�Ρ�
		  ����Pͨ����ѹ����Nͨ����ѹ�����ߵ���Pͨ����ѹ����Nͨ����ѹʱ��PB09�����ƽ���䣻
          
ʹ��˵����
+ EWARM
          1. ��vc_detect_falling_irq.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��vc_detect_falling_irq.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)