/*******************************************************************************
*
* ������ɺ�������Ϣ
* �人��Դ�뵼�����޹�˾������ʹ�����б�̴���ʾ���ķ�ר���İ�Ȩ��ɣ��������ɴ�
* ���ɸ��������ض���Ҫ�����Ƶ����ƹ��ܡ����ݲ��ܱ��ų����κη�����֤���人��Դ��
* �������޹�˾������򿪷��̺͹�Ӧ�̶Գ������֧�֣�����У����ṩ�κ���ʾ��
* ���ı�֤�������������������ڰ������й������ԡ�������ĳ���ض���;�ͷ���Ȩ�ı�֤
* ��������
* ���ۺ������Σ��人��Դ�뵼�����޹�˾������򿪷��̻�Ӧ�̾��������и����
* ��ʹ����֪�䷢���Ŀ�����ʱ��Ҳ����ˣ����ݵĶ�ʧ���𻵣�ֱ�ӵġ��ر�ġ�������
* ���ӵ��𺦣����κκ���Ծ����𺦣�������ҵ�����롢������Ԥ�ڿɽ�ʡ����
* ��ʧ��
* ĳЩ˾��Ͻ���������ֱ�ӵġ������Ļ����Ե������κε��ų������ƣ����ĳЩ��
* ȫ�������ų������ƿ��ܲ�������������
*
*******************************************************************************/
#include "cw32f030c8t6_startkit.h"
static void LED_On(void *LED)
{
    LED_InitTypeDef *_LED;
    _LED = (LED_InitTypeDef *)LED;

    _LED->LED_Port->BSRR = _LED->LED_Pin;
}

static void LED_Off(void *LED)
{
    LED_InitTypeDef *_LED;
    _LED = (LED_InitTypeDef *)LED;
    _LED->LED_Port->BRR = _LED->LED_Pin;
}

static void LED_Toggle(void *LED)
{
    LED_InitTypeDef *_LED;
    _LED = (LED_InitTypeDef *)LED;
    _LED->LED_Port->TOG = _LED->LED_Pin;
}

void LED_Init(void *LED)
{
    LED_InitTypeDef *_LED;
    _LED = (LED_InitTypeDef *)LED;

    switch ((uint32_t)_LED->LED_Port)
    {
        case (uint32_t)CW_GPIOA:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOA_Msk;
            break;
        case (uint32_t)CW_GPIOB:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOB_Msk;
            break;
        case (uint32_t)CW_GPIOC:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOC_Msk;
            break;
        case (uint32_t)CW_GPIOF:
            CW_SYSCTRL->AHBEN |= SYSCTRL_AHBEN_GPIOF_Msk;
            break;
        default:
            break;
    }

    _LED->LED_Port->ANALOG &= ~(_LED->LED_Pin);
    _LED->LED_Port->DIR &= ~(_LED->LED_Pin);

    _LED->LED_Off = LED_Off;
    _LED->LED_On = LED_On;
    _LED->LED_Toggle = LED_Toggle;
}
