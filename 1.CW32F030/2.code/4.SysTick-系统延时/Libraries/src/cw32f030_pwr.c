/**
 * @file cw32f030_pwr.c
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-08-09
 *
 * @copyright Copyright (c) 2021
 *
 */
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

/*******************************************************************************
 * Include files
 ******************************************************************************/
#include "cw32f030.h"
#include "cw32f030_rcc.h"
#include "cw32f030_pwr.h"

/**
 *******************************************************************************
 ** \addtogroup LpmGroup
 ******************************************************************************/
//@{

/*******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/
/*
#define IS_VALID_SEVONPEND(x)           (SevPndDisable == (x) ||\
                                         SevPndEnable  == (x))
#define IS_VALID_SLEEPDEEP(x)           (SlpDpDisable  == (x) ||\
                                         SlpDpEnable   == (x))
#define IS_VALID_SLEEPONEXIT(x)         (SlpExtDisable == (x) ||\
                                         SlpExtEnable  == (x))                                         
*/

/*******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/*******************************************************************************
 * Local variable definitions ('static')
 ******************************************************************************/

/*******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/

/*******************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
/**
 * @brief �͹���ģʽ����
 *
 * @param PWR_InitStruct      �͹���ģʽ���ýṹ��ָ��
 *
 * @return
 */
void PWR_Config(PWR_InitTypeDef* PWR_InitStruct)
{
    ASSERT(NULL != PWR_InitStruct);
    assert_param(IS_PWR_SEVONPEND(PWR_InitStruct->PWR_Sevonpend));
    assert_param(IS_PWR_SLEEPDEEP(PWR_InitStruct->PWR_SleepDeep));
    assert_param(IS_PWR_SLEEPONEXIT(PWR_InitStruct->PWR_SleepOnExit));

    REGBITS_MODIFY(SCB->SCR, SCB_SCR_SEVONPEND_Msk | \
					SCB_SCR_SLEEPDEEP_Msk | \
					SCB_SCR_SLEEPONEXIT_Msk, \
					PWR_InitStruct->PWR_Sevonpend | \
					PWR_InitStruct->PWR_SleepDeep | \
					PWR_InitStruct->PWR_SleepOnExit);
}

/**
 * @brief ����˯��ģʽ
 *       ע��1.����˯��ģʽǰ��HCLK��ʱ��Ƶ����ҪС�ڻ����4MHz
 *           2.��ȴ�VCx_ISR.Ready��־��1�󣬲ſɽ���˯��ģʽ��
 * @return
 */
void PWR_GotoLpmMode(void)
{
    static uint32_t RegSavedCR0, RegSavedCR1;
    static uint32_t RegSavedHSI;

    RegSavedCR0 = CW_SYSCTRL->CR0;
    RegSavedCR1 = CW_SYSCTRL->CR1;   
    RegSavedHSI = CW_SYSCTRL->HSI;      

    if (CW_SYSCTRL->APBEN2 & RCC_APB2_PERIPH_VC)
    {
        if (CW_VC1->CR0_f.EN)
        {
            while (!CW_VC1->SR_f.READY);
        }

        if (CW_VC2->CR0_f.EN)
        {
            while (!CW_VC2->SR_f.READY);
        }        
    }

    RCC_HSI_Enable(RCC_HSIOSC_DIV12);
    RCC_SysClk_Switch(RCC_SYSCLKSRC_HSI);
    __WFI();

    CW_SYSCTRL->CR1 = (RegSavedCR1 & (~SYSCTRL_BYPASS_MASK)) | SYSCTRL_BYPASS;
    CW_SYSCTRL->HSI = RegSavedHSI;
    for (uint32_t i; i < 30000; i++);
    CW_SYSCTRL->CR0 = (RegSavedCR0 & (~SYSCTRL_BYPASS_MASK)) | SYSCTRL_BYPASS;
}
                        
//@} // LpmGroup                                                                           

/*******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
