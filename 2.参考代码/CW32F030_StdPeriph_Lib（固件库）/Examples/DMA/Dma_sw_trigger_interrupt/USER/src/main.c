/**
 * @file main.c
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2021-06-25
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

/******************************************************************************
 * Local pre-processor symbols/macros ('#define')
 ******************************************************************************/

/*****************************************************************************
 * Function implementation - global ('extern') and local ('static')
 ******************************************************************************/


#define BUFFER_SIZE 32U
uint32_t aSRC_Const_Buffer[BUFFER_SIZE] =
{
  0x01020304, 0x05060708, 0x090A0B0C, 0x0D0E0F10,
  0x11121314, 0x15161718, 0x191A1B1C, 0x1D1E1F20,
  0x21222324, 0x25262728, 0x292A2B2C, 0x2D2E2F30,
  0x31323334, 0x35363738, 0x393A3B3C, 0x3D3E3F40,
  0x41424344, 0x45464748, 0x494A4B4C, 0x4D4E4F50,
  0x51525354, 0x55565758, 0x595A5B5C, 0x5D5E5F60,
  0x61626364, 0x65666768, 0x696A6B6C, 0x6D6E6F70,
  0x71727374, 0x75767778, 0x797A7B7C, 0x7D7E7F80
};

uint32_t aDST_Buffer[BUFFER_SIZE];

uint8_t TransOverFlag;

int32_t myMemcmp(uint8_t *mem1,uint8_t *mem2,uint32_t bytesize)
{
  int i = 0;
  uint8_t *p = mem1;
  uint8_t *q = mem2;
  if(p == NULL|| q == NULL)
    return -1;
  
  for(i = 0;i < bytesize;i++,p++,q++)
  {
    if(*p != *q)
    {
      return -1;
    }
  }
  return 0;
}

void ZeroMemory(uint8_t *buf,uint32_t size)
{
  uint32_t i = 0;
  for(i = 0;i < size;i ++)
  {
    *buf = 0x0;
  }
}

void Error_Handle()
{
  while(1);
}

void DMA_CHANNEL1_IRQ_FUNCTION(void)
{
  if( DMA_GetITStatus(DMA_IT_TC1) )
  {
    DMA_ClearITPendingBit(DMA_IT_TC1);
    TransOverFlag = 1;
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
 ** This sample toggle GPIOA.00
 **
 ******************************************************************************/
int32_t main(void)
{
    DMA_InitTypeDef   DMA_InitStruct;		

    RCC_AHBPeriphClk_Enable(RCC_AHB_PERIPH_DMA | RCC_AHB_PERIPH_GPIOB, ENABLE);  //Open DMA and GPIOA Clk
	
	  //PB8/PB9 ������� 
	  GPIO_InitTypeDef GPIO_InitStruct;
		GPIO_InitStruct.Pins = GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		GPIO_Init(CW_GPIOB, &GPIO_InitStruct);
		PB08_SETLOW();
		PB09_SETLOW();
	
    TransOverFlag = 0;
	
	  DMA_StructInit( &DMA_InitStruct );		
    ZeroMemory((uint8_t *)&aDST_Buffer[0],sizeof(aDST_Buffer));

    DMA_InitStruct.DMA_Mode = DMA_MODE_BLOCK;
    DMA_InitStruct.DMA_TransferWidth = DMA_TRANSFER_WIDTH_32BIT;
    DMA_InitStruct.DMA_SrcInc = DMA_SrcAddress_Increase;
    DMA_InitStruct.DMA_DstInc = DMA_DstAddress_Increase;
   
    DMA_InitStruct.DMA_TransferCnt = BUFFER_SIZE;

    DMA_InitStruct.DMA_SrcAddress = (uint32_t)&aSRC_Const_Buffer[0];

    DMA_InitStruct.DMA_DstAddress = (uint32_t)&aDST_Buffer[0];

    DMA_InitStruct.TrigMode = DMA_SWTrig;

    DMA_Init(CW_DMACHANNEL1,&DMA_InitStruct);

    DMA_ClearITPendingBit(DMA_IT_ALL);

    DMA_ITConfig(CW_DMACHANNEL1, DMA_IT_TC|DMA_IT_TE , ENABLE);  //ʹ��DMA_CHANNEL1�ж�
    
    NVIC_Configuration();   //ʹ��DMA_CHANNEL1�ж�

    DMA_Cmd(CW_DMACHANNEL1, ENABLE);  //ʹ��DMA

    DMA_SWTrigCmd(CW_DMACHANNEL1);    //�������DMA����

    while(1)
		{
			if(TransOverFlag == 1)
			{
				TransOverFlag = 0;
				if(myMemcmp((uint8_t *)&aDST_Buffer[0],(uint8_t *)&aSRC_Const_Buffer[0],BUFFER_SIZE) == -1)
				{
					Error_Handle();
				}
				else
				{
					while(1)
					{
						PB08_TOG();    
						PB09_TOG();
						FirmwareDelay(1000000);
					}	
				}
			}
			else
			{
				PB08_TOG();    
				PB09_TOG();
				FirmwareDelay(100000);
			}
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

