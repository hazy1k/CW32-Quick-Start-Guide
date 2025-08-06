/**
 * @file main.c
 * @author P&S (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2021-06-21
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

/******************************************************************************
 * Include files
 ******************************************************************************/
#include "main.h"
#include "cw32f030_rcc.h"
#include "cw32f030_dma.h"
#include "cw32f030_adc.h"
#include "cw32f030_gpio.h"

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/******************************************************************************
 * Global variable definitions (declared in header file with 'extern')
 ******************************************************************************/

/******************************************************************************
 * Local type definitions ('typedef')
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes ('static')
 ******************************************************************************/


/******************************************************************************
 * Local variable definitions ('static')                                      *
 ******************************************************************************/
#define DMA_CYCLE_EN  1
uint32_t ADC_Result_Array[16] = {0};

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/
static void ZeroMemory(uint8_t *buf,uint32_t size)
{
  uint32_t i = 0;
  for(i = 0;i < size;i ++)
  {
    *buf = 0x0;
  }
}

static void Error_Handle()
{
  while(1);
}

void DMA_CHANNEL1_IRQ_FUNCTION(void)
{
  if( DMA_GetITStatus(DMA_IT_TC1) )
  {
    DMA_ClearITPendingBit(DMA_IT_TC1);
 
#ifdef DMA_CYCLE_EN
		CW_DMACHANNEL1->CNT      = 0x10010;                              //����CNT����
		CW_DMACHANNEL1->DSTADDR  = (uint32_t)( &ADC_Result_Array[0] );   //����Ŀ�ĵ�ַ
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);  //ʹ��DMA
#else
    ADC_Disable();    //������ɣ�ֹͣADC
#endif
  }

  if( DMA_GetITStatus(DMA_IT_TE1) )
  {
    DMA_ClearITPendingBit(DMA_IT_TE1);
    Error_Handle();
  }
}
/**
  * @brief  Configure the nested vectored interrupt controller.
  * @param  None
  * @retval None
  */
void NVIC_Configuration(void)
{
    __disable_irq(); 

    NVIC_ClearPendingIRQ(DMACH1_IRQn);

    NVIC_EnableIRQ(DMACH1_IRQn); 

    __enable_irq();  
}


/**
 ******************************************************************************
 ** \brief  Main function of project
 **
 ** \return uint32_t return value, if needed
 **
 ** 
 **
 ******************************************************************************/

int32_t main(void)
{
    ADC_InitTypeDef   ADC_InitStruct;	  
	  DMA_InitTypeDef   DMA_InitStruct;

    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOA | RCC_AHB_PERIPH_GPIOB, ENABLE);  //Open DMA and GPIOA/B Clk
    RCC_APBPeriphClk_Enable2(RCC_APB2_PERIPH_ADC, ENABLE);    //Open ADC Clk

    //PB8/PB9 ������� 
	  GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
		PB08_SETLOW();
		PB09_SETLOW();
	
	  //����ADC����IO�� 
    PA00_ANALOG_ENABLE() ;      //PA00 (AIN0)
	
    //ADC��ʼ��
    ADC_InitStruct.ADC_OpMode = ADC_SingleChOneMode; //��ͨ������ת��ģʽ
    ADC_InitStruct.ADC_ClkDiv = ADC_Clk_Div128; //PCLK
    ADC_InitStruct.ADC_SampleTime = ADC_SampTime5Clk; //5��ADCʱ������
    ADC_InitStruct.ADC_VrefSel = ADC_Vref_VDDA; //VDDA�ο���ѹ
    ADC_InitStruct.ADC_InBufEn = ADC_BufEnable; //����������
    ADC_InitStruct.ADC_TsEn = ADC_TsDisable; //�����¶ȴ���������
    ADC_InitStruct.ADC_DMAEn = ADC_DmaEnable; //ADCת����ɴ���DMA����
    ADC_InitStruct.ADC_Align = ADC_AlignRight; //ADCת������Ҷ���
    ADC_InitStruct.ADC_AccEn = ADC_AccDisable; //ת������ۼӲ�ʹ��
    ADC_Init(&ADC_InitStruct);                                  //��ʼ��ADC����
    CW_ADC->CR1_f.DISCARD = FALSE;                              //�������ݸ��²��ԣ���������ADC�ṹ����               
    CW_ADC->CR1_f.CHMUX = ADC_Vref1P2Input;   //ADC_ExInputCH0; //����ADC����ͨ������������ADC�ṹ����

    ADC_ClearITPendingBit(ADC_IT_EOC);
    ADC_ITConfig(ADC_IT_EOC, ENABLE);
    ADC_EnableNvic(ADC_INT_PRIORITY);
    
    DMA_StructInit( &DMA_InitStruct );
    ZeroMemory((uint8_t *)&ADC_Result_Array[0],sizeof(ADC_Result_Array));
    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_32BIT;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Fix;
	#ifdef DMA_CYCLE_EN
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Increase;
	#else
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Fix;
	#endif

    DMA_InitStruct.DMA_TransferCnt = 16;
    DMA_InitStruct.DMA_SrcAddress = (uint32_t) &(CW_ADC->RESULT0);
    DMA_InitStruct.DMA_DstAddress = (uint32_t)&ADC_Result_Array[0];
    DMA_InitStruct.TrigMode = DMA_HardTrig;
    DMA_InitStruct.HardTrigSource = DMA_HardTrig_ADC_TRANSCOMPLETE;
    DMA_Init(CW_DMACHANNEL1,&DMA_InitStruct);
    DMA_ClearITPendingBit(DMA_IT_ALL);
    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC|DMA_IT_TE , ENABLE);  //ʹ��DMA_CHANNEL1�ж�
    NVIC_Configuration();   //ʹ��DMA_CHANNEL1�ж�
    DMA_Cmd(CW_DMACHANNEL1, ENABLE);  //ʹ��DMA

    //ADCʹ��
    ADC_Enable();
    ADC_SoftwareStartConvCmd(ENABLE);

		while(1)
		{
			PB08_TOG();    
			PB09_TOG();
			FirmwareDelay(1000000);
		}
    
}





/******************************************************************************
 * EOF (not truncated)
 ******************************************************************************/
#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

