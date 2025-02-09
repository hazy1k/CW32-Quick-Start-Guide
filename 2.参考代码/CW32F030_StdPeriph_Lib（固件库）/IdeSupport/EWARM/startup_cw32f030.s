;*******************************************************************************
; This software is owned and published by:
; CW Semiconductor Co.,Ltd ("CW").
;
; BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND
; BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
;
; This software contains source code for use with CW
; components. This software is licensed by CW to be adapted only
; for use in systems utilizing CW components. CW shall not be
; responsible for misuse or illegal use of this software for devices not
; supported herein. CW is providing this software "AS IS" and will
; not be responsible for issues arising from incorrect user implementation
; of the software.
;
; Disclaimer:
; CW MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE,
; REGARDING THE SOFTWARE (INCLUDING ANY ACOOMPANYING WRITTEN MATERIALS),
; ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED USE, INCLUDING,
; WITHOUT LIMITATION, THE IMPLIED WARRANTY OF MERCHANTABILITY, THE IMPLIED
; WARRANTY OF FITNESS FOR A PARTICULAR PURPOSE OR USE, AND THE IMPLIED
; WARRANTY OF NONINFRINGEMENT.
; CW SHALL HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT,
; NEGLIGENCE OR OTHERWISE) FOR ANY DAMAGES WHATSOEVER (INCLUDING, WITHOUT
; LIMITATION, DAMAGES FOR LOSS OF BUSINESS PROFITS, BUSINESS INTERRUPTION,
; LOSS OF BUSINESS INFORMATION, OR OTHER PECUNIARY LOSS) ARISING FROM USE OR
; INABILITY TO USE THE SOFTWARE, INCLUDING, WITHOUT LIMITATION, ANY DIRECT,
; INDIRECT, INCIDENTAL, SPECIAL OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA,
; SAVINGS OR PROFITS,
; EVEN IF Disclaimer HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
; YOU ASSUME ALL RESPONSIBILITIES FOR SELECTION OF THE SOFTWARE TO ACHIEVE YOUR
; INTENDED RESULTS, AND FOR THE INSTALLATION OF, USE OF, AND RESULTS OBTAINED
; FROM, THE SOFTWARE.
;
; This software may be replicated in part or whole for the licensed use,
; with the restriction that this Disclaimer and Copyright notice must be
; included with each copy of this software, whether used in part or whole,
; at all times.
;/
;/*****************************************************************************/
;/*  Startup for IAR                                                          */
;/*  Version     V1.0                                                         */
;/*  Date        2021-02-24                                                   */
;/*  Target-mcu  M0+ Device                                                   */
;/*****************************************************************************/


                MODULE  ?cstartup

                ;; Forward declaration of sections.
                SECTION CSTACK:DATA:NOROOT(3)

                EXTERN  __iar_program_start
                EXTERN  SystemInit
                PUBLIC  __vector_table

                SECTION .intvec:CODE:ROOT(8)
                DATA
__vector_table
                DCD     sfe(CSTACK)               ; Top of Stack
                DCD     Reset_Handler             ; Reset
                DCD     NMI_Handler               ; NMI
                DCD     HardFault_Handler         ; Hard Fault
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     SVC_Handler               ; SVCall
                DCD     0                         ; Reserved
                DCD     0                         ; Reserved
                DCD     PendSV_Handler            ; PendSV
                DCD     SysTick_Handler           ; SysTick

; Numbered IRQ handler vectors

; Note: renaming to device dependent ISR function names are done in

                DCD     WDT_IRQHandler
                DCD     LVD_IRQHandler
                DCD     RTC_IRQHandler
                DCD     FLASHRAM_IRQHandler
                DCD     RCC_IRQHandler
                DCD     GPIOA_IRQHandler
                DCD     GPIOB_IRQHandler
                DCD     GPIOC_IRQHandler
                DCD     GPIOF_IRQHandler
                DCD     DMACH1_IRQHandler
                DCD     DMACH23_IRQHandler
                DCD     DMACH45_IRQHandler
                DCD     ADC_IRQHandler
                DCD     ATIM_IRQHandler
                DCD     VC1_IRQHandler
                DCD     VC2_IRQHandler
                DCD     GTIM1_IRQHandler
                DCD     GTIM2_IRQHandler
                DCD     GTIM3_IRQHandler
                DCD     GTIM4_IRQHandler
                DCD     BTIM1_IRQHandler
                DCD     BTIM2_IRQHandler
                DCD     BTIM3_IRQHandler
                DCD     I2C1_IRQHandler
                DCD     I2C2_IRQHandler
                DCD     SPI1_IRQHandler
                DCD     SPI2_IRQHandler
                DCD     UART1_IRQHandler
                DCD     UART2_IRQHandler
                DCD     UART3_IRQHandler
                DCD     AWT_IRQHandler
                DCD     FAULT_IRQHandler

                THUMB

                PUBWEAK Reset_Handler
                SECTION .text:CODE:NOROOT:REORDER(2)
Reset_Handler
                ;reset NVIC if in rom debug
                LDR     R0, =0x20000000
                LDR     R2, =0x0              ; vector offset
                cmp     PC, R0
                bls     ROMCODE

              ; ram code base address.
                ADD     R2, R0,R2
ROMCODE
              ; reset Vector table address.
                LDR     R0, =0xE000ED08
                STR     R2, [R0]

                LDR     R0, =SystemInit
                BLX     R0
                LDR     R0, =__iar_program_start
                BX      R0

                PUBWEAK NMI_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
NMI_Handler
                B       NMI_Handler

                PUBWEAK HardFault_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
HardFault_Handler
                B       HardFault_Handler


                PUBWEAK SVC_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SVC_Handler
                B       SVC_Handler

                PUBWEAK PendSV_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
PendSV_Handler
                B       PendSV_Handler

                PUBWEAK SysTick_Handler
                SECTION .text:CODE:NOROOT:REORDER(1)
SysTick_Handler
                B       SysTick_Handler

 
                PUBWEAK WDT_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
WDT_IRQHandler
                B       WDT_IRQHandler


                PUBWEAK LVD_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
LVD_IRQHandler
                B       LVD_IRQHandler


                PUBWEAK RTC_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
RTC_IRQHandler
                B       RTC_IRQHandler


                PUBWEAK FLASHRAM_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
FLASHRAM_IRQHandler
                B       FLASHRAM_IRQHandler


                PUBWEAK RCC_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
RCC_IRQHandler
                B       RCC_IRQHandler


                PUBWEAK GPIOA_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GPIOA_IRQHandler
                B       GPIOA_IRQHandler


                PUBWEAK GPIOB_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GPIOB_IRQHandler
                B       GPIOB_IRQHandler


                PUBWEAK GPIOC_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GPIOC_IRQHandler
                B       GPIOC_IRQHandler


                PUBWEAK GPIOF_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GPIOF_IRQHandler
                B       GPIOF_IRQHandler


                PUBWEAK DMACH1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
DMACH1_IRQHandler
                B       DMACH1_IRQHandler


                PUBWEAK DMACH23_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
DMACH23_IRQHandler
                B       DMACH23_IRQHandler


                PUBWEAK DMACH45_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
DMACH45_IRQHandler
                B       DMACH45_IRQHandler


                PUBWEAK ADC_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
ADC_IRQHandler
                B       ADC_IRQHandler


                PUBWEAK ATIM_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
ATIM_IRQHandler
                B       ATIM_IRQHandler


                PUBWEAK VC1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
VC1_IRQHandler
                B       VC1_IRQHandler


                PUBWEAK VC2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
VC2_IRQHandler
                B       VC2_IRQHandler


                PUBWEAK GTIM1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GTIM1_IRQHandler
                B       GTIM1_IRQHandler


                PUBWEAK GTIM2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GTIM2_IRQHandler
                B       GTIM2_IRQHandler


                PUBWEAK GTIM3_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GTIM3_IRQHandler
                B       GTIM3_IRQHandler


                PUBWEAK GTIM4_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
GTIM4_IRQHandler
                B       GTIM4_IRQHandler


                PUBWEAK BTIM1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
BTIM1_IRQHandler
                B       BTIM1_IRQHandler


                PUBWEAK BTIM2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
BTIM2_IRQHandler
                B       BTIM2_IRQHandler


                PUBWEAK BTIM3_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
BTIM3_IRQHandler
                B       BTIM3_IRQHandler


                PUBWEAK I2C1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
I2C1_IRQHandler
                B       I2C1_IRQHandler


                PUBWEAK I2C2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
I2C2_IRQHandler
                B       I2C2_IRQHandler


                PUBWEAK SPI1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
SPI1_IRQHandler
                B       SPI1_IRQHandler


                PUBWEAK SPI2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
SPI2_IRQHandler
                B       SPI2_IRQHandler


                PUBWEAK UART1_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
UART1_IRQHandler
                B       UART1_IRQHandler


                PUBWEAK UART2_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
UART2_IRQHandler
                B       UART2_IRQHandler


                PUBWEAK UART3_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
UART3_IRQHandler
                B       UART3_IRQHandler


                PUBWEAK AWT_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
AWT_IRQHandler
                B       AWT_IRQHandler


                PUBWEAK FAULT_IRQHandler
                SECTION .text:CODE:NOROOT:REORDER(1)
FAULT_IRQHandler
                B       FAULT_IRQHandler





                END
