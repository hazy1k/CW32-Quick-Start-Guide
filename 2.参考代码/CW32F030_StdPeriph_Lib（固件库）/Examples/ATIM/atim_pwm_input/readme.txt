ʾ��Ŀ�ģ�
          ��ʾATIM��Ϊ������BTIM��Ϊ�ӻ��ļ���ʹ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. ATIM���ؼ������ϼ���ģʽ������Ƶ��ARR����Ϊ0xFFFF��
             ���벶��, ʹ��CH1A˫���ز���
          5. BTIM1��500KHz��Ƶ�ʼ�ʱ,ARR����Ϊ49,��5KHz��Ƶ��Ӳ����תTOGP����
          6. PA07����ΪBTIM1TOGP�������
          7. ����UART1��Ϊ���Դ���,��ӡ������Ϣ(PA8->TXD),������115200
          8. PB02����ΪATIM_CH1A

��ʾ˵����
          ʹ�öŰ��߽�PA07��PB02����, UART1����ӡPA02�����źŵ����ں�������.

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
