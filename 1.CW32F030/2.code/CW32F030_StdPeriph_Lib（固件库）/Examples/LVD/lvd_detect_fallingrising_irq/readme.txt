ʾ��Ŀ�ģ�
          ��ʾLVDģ��������ء��½��ش����жϹ��ܡ�

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ��ԴΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. LVD���˲�ʱ��ԴΪRC150K
          5. PB08/09����ΪPUSH-PULL���

��ʾ˵����
          LVD����ͨ��ΪPB11������˿�ΪPA08�����޵�ѹΪ2V��
          ��ӿɵ�ֱ����Դ����LVD����ͨ����ѹ���ڻ��߸������޵�ѹʱ�̣�PB09�����ƽ��תһ�Ρ�
		  LVD����ͨ����ѹ���ֵ��ڻ��߸������޵�ѹ��PB09�����ƽ���䡣
          
ʹ��˵����
+ EWARM
          1. ��lvd_detect_fallingrising_irq.eww�ļ�
          2. ���������ļ���Project->Rebuild all
          3. ���빤�̾���Project->Debug
          4. ���г���Debug->Go(F5)

+ MDK-ARM
          1. ��lvd_detect_fallingrising_irq.uvproj�ļ�
          2. ���������ļ���Project->Rebuild all target files
          3. ���빤�̾���Debug->Start/Stop Debug Session
          4. ���г���Debug->Run(F5)