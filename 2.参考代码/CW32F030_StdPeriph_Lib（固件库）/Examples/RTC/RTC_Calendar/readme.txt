ʾ��Ŀ�ģ�
          RTCʱ�Ӳ��ԣ���ʼ��������ʹ�ü���ж�0.5��ͨ��Log�������ʱ��

Ӳ����Դ��
          1. CW32F030C8T6 StartKit
          2. ʱ��HSI�� LSEʹ��
          3. ϵͳʱ������ΪHSIʱ��6��Ƶ��8MHz�� PCLK��HCLK����Ƶ��PCLK=HCLK=SysClk=8MHz
          4. RTCʹ��LSE��Ϊʱ������
          5. ����UART1��Ϊ���Դ���,��ӡ������Ϣ(PA8->TXD),������115200
          

��ʾ˵����
          ÿ0.5��ͨ��Log�������ʱ��.

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
