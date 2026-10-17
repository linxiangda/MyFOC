/*!
    \file    gd32f30x_it.c
    \brief   interrupt service routines

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x 
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#include "gd32f30x_it.h"
#include "systick.h"

/*!
    \brief      this function handles NMI exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void NMI_Handler(void)
{
	;
}

/*!
    \brief      this function handles HardFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void HardFault_Handler(void)
{
    /* if Hard Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles MemManage exception
    \param[in]  none
    \param[out] none
    \retval     none
*/

void MemManage_Handler(void)
{
    /* if Memory Manage exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles BusFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void BusFault_Handler(void)
{
    /* if Bus Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles UsageFault exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void UsageFault_Handler(void)
{
    /* if Usage Fault exception occurs, go to infinite loop */
    while (1){
    }
}

/*!
    \brief      this function handles SVC exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SVC_Handler(void)
{
}

/*!
    \brief      this function handles DebugMon exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void DebugMon_Handler(void)
{
}

/*!
    \brief      this function handles PendSV exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void PendSV_Handler(void)
{
}

/*!
    \brief      this function handles SysTick exception
    \param[in]  none
    \param[out] none
    \retval     none
*/
void SysTick_Handler(void)
{
	delay_decrement();
}


//以下为中断服务函数，用户可根据需要，在对应函数内添加内容

/* EXTI0 中断服务函数 */
void EXTI0_IRQHandler(void)
{
    ;
}

/* EXTI1 中断服务函数 */
void EXTI1_IRQHandler(void)
{
    ;
}

///* EXTI2 中断服务函数 */
//void EXTI2_IRQHandler(void)
//{
//    ;
//}
/* EXTI3 中断服务函数 */
void EXTI3_IRQHandler(void)
{
    ;
}
/* EXTI4 中断服务函数 */
void EXTI4_IRQHandler(void)
{
    ;
}
/* EXTI5_9 中断服务函数 */
void EXTI5_9_IRQHandler(void)
{
    ;
}
/* EXTI10_15 中断服务函数 */
void EXTI10_15_IRQHandler(void)
{
		;
}

///* TIMER0_Channel 中断服务函数 */
//void TIMER0_Channel_IRQHandler(void)
//{
//    ;
//}

/* TIMER1 中断服务函数 */
void TIMER1_IRQHandler(void)
{
    ;
}

/* TIMER2 中断服务函数 */
void TIMER2_IRQHandler(void)
{
    ;
}

/* TIMER3 中断服务函数 */
void TIMER3_IRQHandler(void)
{
    ;
}

/* TIMER4 中断服务函数 */
void TIMER4_IRQHandler(void)
{
    ;
}

/* TIMER5 中断服务函数 */
//void TIMER5_IRQHandler(void)
//{
//  if(timer_interrupt_flag_get(TIMER5, TIMER_INT_UP) == SET)
//	{

//		timer_interrupt_flag_clear(TIMER5, TIMER_INT_FLAG_UP);
//	}
//}

/* TIMER6 中断服务函数 */
void TIMER6_IRQHandler(void)
{
    ;
}

/* TIMER7 中断服务函数 */
void TIMER7_Channel_IRQHandler(void)
{
    ;
}

/* I2C0_EV 中断服务函数 */
void I2C0_EV_IRQHandler(void)
{
    ;
}

/* I2C1_EV 中断服务函数 */
void I2C1_EV_IRQHandler(void)
{
    ;
}

/* SPI0 中断服务函数 */
void SPI0_IRQHandler(void)
{
    ;
}

/* SPI1 中断服务函数 */
void SPI1_IRQHandler(void)
{
    ;
}

/* SPI2 中断服务函数 */
void SPI2_IRQHandler(void)
{
    ;
}

/* USART0 中断服务函数 */
void USART0_IRQHandler(void)
{
	;
}

/* USART1 中断服务函数 */
void USART1_IRQHandler(void)
{
    ;
}
//uint8_t rx_char;
///* USART2 中断服务函数 */
//void USART2_IRQHandler(void)
//{
//	if(usart_interrupt_flag_get(USART2,USART_INT_FLAG_RBNE))		//读数据缓冲区非空中断标志
//	{
//		rx_char=usart_data_receive(USART2);							//接收数据
//		usart_data_transmit(USART2, rx_char);						//将接受的数据通过串口发送出去
//		usart_interrupt_flag_clear(USART2,USART_INT_FLAG_RBNE);		//清除读数据缓冲区非空中断标志
//	}
//}

/* UART3 中断服务函数 */
void UART3_IRQHandler(void)
{
    ;
}

/* USART4 中断服务函数 */
void UART4_IRQHandler(void)
{
    ;
}
