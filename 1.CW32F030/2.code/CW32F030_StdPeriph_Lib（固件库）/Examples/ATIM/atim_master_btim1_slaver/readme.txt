ʾ��Ŀ�ģ�
          ��ʾATIM��Ϊ������BTIM��Ϊ�ӻ��ļ���ʹ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. ATIM���ؼ������ϼ���ģʽ��8��Ƶ��ARR����Ϊ99���������100us, ��ģʽ
          5. BTIM1ʹ��ATIM���������λUG��Ϊ����ʱ�ӵ���Դ����100us����һ�Σ�ARR����Ϊ99��
             ��BTIM1ÿ10ms���һ�Ρ�
          6. PB00����ΪBTIM1TOGN�������
          7. ����UART1��Ϊ���Դ���,��ӡ������Ϣ(PA8->TXD),������115200
��ʾ˵����
          ATIM��1MHzʱ�ӽ��м���������ARR�Ĵ���Ϊ99����ATIM��ÿ100us���һ�Σ�������
          �жϷ�������жϷ�������У���CR.UGλд1,��BTIM1�ṩ��ʱ�źš�PB00��ÿ10ms
          ��תһ��.

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
