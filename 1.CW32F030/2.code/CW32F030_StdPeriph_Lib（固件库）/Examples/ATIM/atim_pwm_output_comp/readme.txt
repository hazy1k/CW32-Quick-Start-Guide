ʾ��Ŀ�ģ�
          ��ʾATIM��PWM�����������

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. ATIM���ؼ������ϼ���ģʽ��8��Ƶ������ʱ��1MHz,ARR����Ϊ99��100us���,
             ����Ƚ�, ʹ��ATIM��CH1A��CH1Bͨ�����������PWM�źš�
          5. ����UART1��Ϊ���Դ���,��ӡ������Ϣ(PA8->TXD),������115200
          6. PB13����ΪATIM_CH1Bͨ�������.
          7. PB02����ΪATIM_CH1Aͨ�������.

��ʾ˵����
          PB14�ƶ�һ��LED�ƣ�ʵ�ֺ����Ƶ�Ч�����趨5s���50���ȼ��ĸı�.

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
