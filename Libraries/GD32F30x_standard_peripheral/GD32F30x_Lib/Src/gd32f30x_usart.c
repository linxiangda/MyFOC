/*!
    \file    gd32f30x_usart.c
    \brief   USART driver

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

#include "gd32f30x_usart.h"

/* USART register bit offset */
#define GP_GUAT_OFFSET            ((uint32_t)8U)       /* bit offset of GUAT in USART_GP */
#define CTL3_SCRTNUM_OFFSET       ((uint32_t)1U)       /* bit offset of SCRTNUM in USART_CTL3 */
#define RT_BL_OFFSET              ((uint32_t)24U)      /* bit offset of BL in USART_RT */

/*!
    @描述       复位USART/UART 
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输出] 无
    @返回值     无
*/
void usart_deinit(uint32_t usart_periph)
{
    switch(usart_periph){
    case USART0:
        /* 复位 USART0 */
        rcu_periph_reset_enable(RCU_USART0RST);
        rcu_periph_reset_disable(RCU_USART0RST);
        break;
    case USART1:
        /* 复位 USART1 */
        rcu_periph_reset_enable(RCU_USART1RST);
        rcu_periph_reset_disable(RCU_USART1RST);
        break;
    case USART2:
        /* 复位 USART2 */
        rcu_periph_reset_enable(RCU_USART2RST);
        rcu_periph_reset_disable(RCU_USART2RST);
        break;
    case UART3:
        /* 复位 UART3 */
        rcu_periph_reset_enable(RCU_UART3RST);
        rcu_periph_reset_disable(RCU_UART3RST);
        break;
    case UART4:
        /* 复位 UART4 */
        rcu_periph_reset_enable(RCU_UART4RST);
        rcu_periph_reset_disable(RCU_UART4RST);
        break;
    default:
        break;
    }
}

/*!
    @描述       配置USART波特率
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  baudval: 波特率值
    @参数[输出] 无
    @返回值     无
*/ 
void usart_baudrate_set(uint32_t usart_periph, uint32_t baudval)
{
    uint32_t uclk=0U, intdiv=0U, fradiv=0U, udiv=0U;
    switch(usart_periph){
         /* 获取时钟频率 */
    case USART0:
         /* 得到USART0时钟 */
         uclk = rcu_clock_freq_get(CK_APB2);
         break;
    case USART1:
         /* 得到USART1时钟 */
         uclk = rcu_clock_freq_get(CK_APB1);
         break;
    case USART2:
         /* 得到USART2时钟 */
         uclk = rcu_clock_freq_get(CK_APB1);
         break;
    case UART3:
         /* 得到UART3时钟 */
         uclk = rcu_clock_freq_get(CK_APB1);
         break;
    case UART4:
         /* 得到UART4时钟 */
         uclk = rcu_clock_freq_get(CK_APB1);
         break;  
    default:
         break;
    }
    /* 16倍过采样，配置USART_BAUD寄存器值 */
    udiv = (uclk+baudval/2U)/baudval;
    intdiv = udiv & 0xfff0U;
    fradiv = udiv & 0xfU;
    USART_BAUD(usart_periph) = ((USART_BAUD_FRADIV | USART_BAUD_INTDIV) & (intdiv | fradiv));
}

/*!
    @描述       配置USART奇偶校验
    @参数[输入] usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入] paritycfg: 奇偶校验
               只能选择一个参数，如下所示:
      @arg       USART_PM_NONE: 无校验
      @arg       USART_PM_ODD:  奇校验
      @arg       USART_PM_EVEN: 偶校验
    @参数[输出] 无
    @返回值     无
*/
void usart_parity_config(uint32_t usart_periph, uint32_t paritycfg)
{
    /* 清除 USART_CTL0 PM,PCEN 位 */
    USART_CTL0(usart_periph) &= ~(USART_CTL0_PM | USART_CTL0_PCEN);
    /* 配置 USART 校验 */
    USART_CTL0(usart_periph) |= paritycfg ;
}

/*!
    @描述       配置USART字长
    @参数[输入] usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入] wlen: 配置USART字长
               只能选择一个参数，如下所示:
      @arg       USART_WL_8BIT: 8位
      @arg       USART_WL_9BIT: 9位
    @参数[输出] 无
    @返回值     无
*/
void usart_word_length_set(uint32_t usart_periph, uint32_t wlen)
{
    /* 清除 USART_CTL0 WL 位 */
    USART_CTL0(usart_periph) &= ~USART_CTL0_WL;
    /* 配置USART字长 */
    USART_CTL0(usart_periph) |= wlen;
}

/*!
    @描述       配置USART停止位
    @参数[输入] usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入] stblen: 配置USART停止位
               只能选择一个参数，如下所示:
      @arg       USART_STB_1BIT:   1 位
      @arg       USART_STB_0_5BIT: 0.5 位,该位对UARTx(x=3,4)无效
      @arg       USART_STB_2BIT:   2 位
      @arg       USART_STB_1_5BIT: 1.5 位,该位对UARTx(x=3,4)无效
    @参数[输出] 无
    @返回值     无
*/
void usart_stop_bit_set(uint32_t usart_periph, uint32_t stblen)
{
    /* 清除 USART_CTL1 STB 位 */
    USART_CTL1(usart_periph) &= ~USART_CTL1_STB; 
    /* 配置USART停止位 */
    USART_CTL1(usart_periph) |= stblen;
}

/*!
    @描述       使能USART
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输出] 无
    @返回值     无
*/
void usart_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |= USART_CTL0_UEN;
}

/*!
    @描述       失能USART
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输出] 无
    @返回值     无
*/
void usart_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &= ~(USART_CTL0_UEN);
}

/*!
    @描述       USART发送器配置
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  txconfig: 使能/失能USART发送器
                只能选择一个参数，如下所示:
      @arg        USART_TRANSMIT_ENABLE: 使能USART发送
      @arg        USART_TRANSMIT_DISABLE: 失能USART发送
    @参数[输出] 无
    @返回值     无
*/
void usart_transmit_config(uint32_t usart_periph, uint32_t txconfig)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL0(usart_periph);
    ctl &= ~USART_CTL0_TEN;
    ctl |= txconfig;
    /* 配置发送模式 */
    USART_CTL0(usart_periph) = ctl;
}

/*!
    @描述        USART接收器配置
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  rxconfig: 使能/失能USART接收器
                 只能选择一个参数，如下所示:
      @arg         USART_RECEIVE_ENABLE: 使能USART接收
      @arg         USART_RECEIVE_DISABLE: 失能USART接收
    @参数[输出]] 无
    @返回值      无
*/
void usart_receive_config(uint32_t usart_periph, uint32_t rxconfig)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL0(usart_periph);
    ctl &= ~USART_CTL0_REN;
    ctl |= rxconfig;
    /* 配置接收模式 */
    USART_CTL0(usart_periph) = ctl;
}

/*!
    @描述       配置数据传输时低位在前或高位在前
    @参数[输入]  usart_periph: USARTx(x=0,1,2)
    @参数[输入]  msbf: LSB(低位)/MSB(高位)
                只能选择一个参数，如下所示:
      @arg        USART_MSBF_LSB: 数据传输时低位在前
      @arg        USART_MSBF_MSB: 数据传输时高位在前
    @参数[输出] 无
    @返回值     无
*/
void usart_data_first_config(uint32_t usart_periph, uint32_t msbf)
{
    USART_CTL3(usart_periph) &= ~(USART_CTL3_MSBF); 
    USART_CTL3(usart_periph) |= msbf;
}

/*!
    @描述       配置USART反转功能
    @参数[输入]  usart_periph: USARTx(x=0,1,2)
    @参数[输入]  invertpara: 参考枚举 usart_invert_enum
               只能选择一个参数，如下所示:
      @arg        USART_DINV_ENABLE: 数据位电平反转
      @arg        USART_DINV_DISABLE: 数据位电平不反转
      @arg        USART_TXPIN_ENABLE: TX引脚电平反转
      @arg        USART_TXPIN_DISABLE: TX引脚电平不反转
      @arg        USART_RXPIN_ENABLE: RX引脚电平反转
      @arg        USART_RXPIN_DISABLE: RX引脚电平不反转
    @输出[输出] 无
    @返回值     无
*/
void usart_invert_config(uint32_t usart_periph, usart_invert_enum invertpara)
{
    /* inverted or not the specified siginal */ 
    switch(invertpara){
    case USART_DINV_ENABLE:
        /* data bit level inversion */
        USART_CTL3(usart_periph) |= USART_CTL3_DINV;
        break;
    case USART_TXPIN_ENABLE:
        /* TX pin level inversion */
        USART_CTL3(usart_periph) |= USART_CTL3_TINV;
        break;
    case USART_RXPIN_ENABLE:
        /* RX pin level inversion */
        USART_CTL3(usart_periph) |= USART_CTL3_RINV;
        break;
    case USART_DINV_DISABLE:
        /* data bit level not inversion */
        USART_CTL3(usart_periph) &= ~(USART_CTL3_DINV);
        break;
    case USART_TXPIN_DISABLE:
        /* TX pin level not inversion */
        USART_CTL3(usart_periph) &= ~(USART_CTL3_TINV);
        break;
    case USART_RXPIN_DISABLE:
        /* RX pin level not inversion */
        USART_CTL3(usart_periph) &= ~(USART_CTL3_RINV);
        break;
    default:
        break;
    }
}

/*!
    @描述       使能USART接收超时
    @参数[输入]  usart_periph: USARTx(x=0,1,2)
    @参数[输出] 无
    @返回值     无
*/
void usart_receiver_timeout_enable(uint32_t usart_periph)
{
    USART_CTL3(usart_periph) |= USART_CTL3_RTEN;
}

/*!
    @描述       失能USART接收超时
    @参数[输入]  usart_periph: USARTx(x=0,1,2)
    @参数[输出] 无
    @返回值     无
*/
void usart_receiver_timeout_disable(uint32_t usart_periph)
{
    USART_CTL3(usart_periph) &= ~(USART_CTL3_RTEN);
}

/*!
    @描述       设置USART接收超时阈值
    @参数[输入]  usart_periph: USARTx(x=0,1,2)
    @参数[输入]  rtimeout:超时时间 0-0xFFFFFF
    @参数[输出] 无
    @返回值     无
*/
void usart_receiver_timeout_threshold_config(uint32_t usart_periph, uint32_t rtimeout)
{
    USART_RT(usart_periph) &= ~(USART_RT_RT);
    USART_RT(usart_periph) |= rtimeout;
}

/*!
    @描述       USART发送数据
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  data: 发送的数据 
    @参数[输出] 无
    @返回值     无
*/
void usart_data_transmit(uint32_t usart_periph, uint32_t data)
{
    USART_DATA(usart_periph) = ((uint16_t)USART_DATA_DATA & data);
}

/*!
    @描述       USART接收数据
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输出] 无
    @返回值     接收的数据
*/
uint16_t usart_data_receive(uint32_t usart_periph)
{
    return (uint16_t)(GET_BITS(USART_DATA(usart_periph), 0U, 8U));
}

/*!
    \brief      configure the address of the USART in wake up by address match mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[in]  addr: address of USART/UART
    \param[out] none
    \retval     none
*/
void usart_address_config(uint32_t usart_periph, uint8_t addr)
{
    USART_CTL1(usart_periph) &= ~(USART_CTL1_ADDR);
    USART_CTL1(usart_periph) |= (USART_CTL1_ADDR & addr);
}

/*!
    \brief      receiver in mute mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_mute_mode_enable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |= USART_CTL0_RWU;
}

/*!
    \brief      receiver in active mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_mute_mode_disable(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) &= ~(USART_CTL0_RWU);
}

/*!
    \brief      configure wakeup method in mute mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[in]  wmethod: two methods be used to enter or exit the mute mode
                only one parameter can be selected which is shown as below:
      \arg        USART_WM_IDLE: idle line
      \arg        USART_WM_ADDR: address mask
    \param[out] none
    \retval     none
*/
void usart_mute_mode_wakeup_config(uint32_t usart_periph, uint32_t wmethod)
{
    USART_CTL0(usart_periph) &= ~(USART_CTL0_WM);
    USART_CTL0(usart_periph) |= wmethod;
}

/*!
    \brief      enable LIN mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_lin_mode_enable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) |= USART_CTL1_LMEN;
}

/*!
    \brief      disable LIN mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_lin_mode_disable(uint32_t usart_periph)
{   
    USART_CTL1(usart_periph) &= ~(USART_CTL1_LMEN);
}

/*!
    \brief      configure lin break frame length
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[in]  lblen: lin break frame length
                only one parameter can be selected which is shown as below:
      \arg        USART_LBLEN_10B: 10 bits
      \arg        USART_LBLEN_11B: 11 bits
    \param[out] none
    \retval     none
*/
void usart_lin_break_detection_length_config(uint32_t usart_periph, uint32_t lblen)
{
    USART_CTL1(usart_periph) &= ~(USART_CTL1_LBLEN);
    USART_CTL1(usart_periph) |= (USART_CTL1_LBLEN & lblen);
}

/*!
    \brief      send break frame
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_send_break(uint32_t usart_periph)
{
    USART_CTL0(usart_periph) |= USART_CTL0_SBKCMD;
}

/*!
    \brief      enable half duplex mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_halfduplex_enable(uint32_t usart_periph)
{   
    USART_CTL2(usart_periph) |= USART_CTL2_HDEN;
}

/*!
    \brief      disable half duplex mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_halfduplex_disable(uint32_t usart_periph)
{  
    USART_CTL2(usart_periph) &= ~(USART_CTL2_HDEN);
}

/*!
    \brief      enable CK pin in synchronous mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_synchronous_clock_enable(uint32_t usart_periph)
{
    USART_CTL1(usart_periph) |= USART_CTL1_CKEN;
}

/*!
    \brief      disable CK pin in synchronous mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_synchronous_clock_disable(uint32_t usart_periph)
{
    USART_CTL1(usart_periph) &= ~(USART_CTL1_CKEN);
}

/*!
    \brief      configure USART synchronous mode parameters
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  clen: CK length
                only one parameter can be selected which is shown as below:
      \arg        USART_CLEN_NONE: there are 7 CK pulses for an 8 bit frame and 8 CK pulses for a 9 bit frame 
      \arg        USART_CLEN_EN:   there are 8 CK pulses for an 8 bit frame and 9 CK pulses for a 9 bit frame
    \param[in]  cph: clock phase
                only one parameter can be selected which is shown as below:
      \arg        USART_CPH_1CK: first clock transition is the first data capture edge 
      \arg        USART_CPH_2CK: second clock transition is the first data capture edge
    \param[in]  cpl: clock polarity
                only one parameter can be selected which is shown as below:
      \arg        USART_CPL_LOW:  steady low value on CK pin 
      \arg        USART_CPL_HIGH: steady high value on CK pin
    \param[out] none
    \retval     none
*/
void usart_synchronous_clock_config(uint32_t usart_periph, uint32_t clen, uint32_t cph, uint32_t cpl)
{
    uint32_t ctl = 0U;
    
    /* read USART_CTL1 register */
    ctl = USART_CTL1(usart_periph);
    ctl &= ~(USART_CTL1_CLEN | USART_CTL1_CPH | USART_CTL1_CPL);
    /* set CK length, CK phase, CK polarity */
    ctl |= (USART_CTL1_CLEN & clen) | (USART_CTL1_CPH & cph) | (USART_CTL1_CPL & cpl);

    USART_CTL1(usart_periph) = ctl;
}

/*!
    \brief      configure guard time value in smartcard mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  guat: guard time value, 0-0xFF
    \param[out] none
    \retval     none
*/
void usart_guard_time_config(uint32_t usart_periph,uint32_t guat)
{
    USART_GP(usart_periph) &= ~(USART_GP_GUAT);
    USART_GP(usart_periph) |= (USART_GP_GUAT & ((guat)<<GP_GUAT_OFFSET));
}

/*!
    \brief      enable smartcard mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_enable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) |= USART_CTL2_SCEN;
}

/*!
    \brief      disable smartcard mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_disable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) &= ~(USART_CTL2_SCEN);
}

/*!
    \brief      enable NACK in smartcard mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_nack_enable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) |= USART_CTL2_NKEN;
}

/*!
    \brief      disable NACK in smartcard mode
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[out] none
    \retval     none
*/
void usart_smartcard_mode_nack_disable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) &= ~(USART_CTL2_NKEN);
}

/*!
    \brief      configure smartcard auto-retry number
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  scrtnum: smartcard auto-retry number
    \param[out] none
    \retval     none
*/
void usart_smartcard_autoretry_config(uint32_t usart_periph, uint32_t scrtnum)
{
    USART_CTL3(usart_periph) &= ~(USART_CTL3_SCRTNUM);
    USART_CTL3(usart_periph) |= (USART_CTL3_SCRTNUM & ((scrtnum)<<CTL3_SCRTNUM_OFFSET));
}

/*!
    \brief      configure block length in Smartcard T=1 reception
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  bl: block length
    \param[out] none
    \retval     none
*/
void usart_block_length_config(uint32_t usart_periph, uint32_t bl)
{
    USART_RT(usart_periph) &= ~(USART_RT_BL);
    USART_RT(usart_periph) |= (USART_RT_BL & ((bl)<<RT_BL_OFFSET));
}

/*!
    \brief      enable IrDA mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_irda_mode_enable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) |= USART_CTL2_IREN;
}

/*!
    \brief      disable IrDA mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[out] none
    \retval     none
*/
void usart_irda_mode_disable(uint32_t usart_periph)
{
    USART_CTL2(usart_periph) &= ~(USART_CTL2_IREN);
}

/*!
    \brief      configure the peripheral clock prescaler in USART IrDA low-power mode
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[in]  psc: 0x00-0xFF
    \param[out] none
    \retval     none
*/
void usart_prescaler_config(uint32_t usart_periph, uint8_t psc)
{
    USART_GP(usart_periph) &= ~(USART_GP_PSC);
    USART_GP(usart_periph) |= psc;
}

/*!
    \brief      configure IrDA low-power
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    \param[in]  irlp: IrDA low-power or normal
                only one parameter can be selected which is shown as below:
      \arg        USART_IRLP_LOW: low-power
      \arg        USART_IRLP_NORMAL: normal
    \param[out] none
    \retval     none
*/
void usart_irda_lowpower_config(uint32_t usart_periph, uint32_t irlp)
{
    USART_CTL2(usart_periph) &= ~(USART_CTL2_IRLP);
    USART_CTL2(usart_periph) |= (USART_CTL2_IRLP & irlp);
}

/*!
    \brief      configure hardware flow control RTS
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  rtsconfig: enable or disable RTS
                only one parameter can be selected which is shown as below:
      \arg        USART_RTS_ENABLE:  enable RTS
      \arg        USART_RTS_DISABLE: disable RTS
    \param[out] none
    \retval     none
*/
void usart_hardware_flow_rts_config(uint32_t usart_periph, uint32_t rtsconfig)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_RTSEN;
    ctl |= rtsconfig;
    /* configure RTS */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    \brief      configure hardware flow control CTS
    \param[in]  usart_periph: USARTx(x=0,1,2)
    \param[in]  ctsconfig: enable or disable CTS
                only one parameter can be selected which is shown as below:
      \arg        USART_CTS_ENABLE:  enable CTS
      \arg        USART_CTS_DISABLE: disable CTS
    \param[out] none
    \retval     none
*/
void usart_hardware_flow_cts_config(uint32_t usart_periph, uint32_t ctsconfig)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_CTSEN;
    ctl |= ctsconfig;
    /* configure CTS */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    \brief      configure USART DMA reception
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3)
    \param[in]  dmacmd: enable or disable DMA for reception
                only one parameter can be selected which is shown as below:
      \arg        USART_DENR_ENABLE:  DMA enable for reception
      \arg        USART_DENR_DISABLE: DMA disable for reception
    \param[out] none
    \retval     none
*/
void usart_dma_receive_config(uint32_t usart_periph, uint32_t dmacmd)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_DENR;
    ctl |= dmacmd;
    /* configure DMA reception */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    \brief      configure USART DMA transmission
    \param[in]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3)
    \param[in]  dmacmd: enable or disable DMA for transmission
                only one parameter can be selected which is shown as below:
      \arg        USART_DENT_ENABLE:  DMA enable for transmission
      \arg        USART_DENT_DISABLE: DMA disable for transmission
    \param[out] none
    \retval     none
*/
void usart_dma_transmit_config(uint32_t usart_periph, uint32_t dmacmd)
{
    uint32_t ctl = 0U;
    
    ctl = USART_CTL2(usart_periph);
    ctl &= ~USART_CTL2_DENT;
    ctl |= dmacmd;
    /* configure DMA transmission */
    USART_CTL2(usart_periph) = ctl;
}

/*!
    @描述       获取USART状态寄存器标志位
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  flag: USART标志位，参考枚举 usart_flag_enum
                只能选择一个参数，如下所示:
      @arg        USART_FLAG_CTS: CTS变化标志
      @arg        USART_FLAG_LBD: LIN断开检测标志
      @arg        USART_FLAG_TBE: 发送数据缓冲区空标志 
      @arg        USART_FLAG_TC: 发送完成标志 
      @arg        USART_FLAG_RBNE: 读数据缓冲区非空标志 
      @arg        USART_FLAG_IDLE: 空闲线检测标志
      @arg        USART_FLAG_ORERR: 溢出错误标志
      @arg        USART_FLAG_NERR: 噪声错误标志 
      @arg        USART_FLAG_FERR: 帧错误标志
      @arg        USART_FLAG_PERR: 校验错误标志
      @arg        USART_FLAG_BSY: 忙状态标志
      @arg        USART_FLAG_EB: 块结束标志 
      @arg        USART_FLAG_RT: 接收超时标志
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus usart_flag_get(uint32_t usart_periph, usart_flag_enum flag)
{
    if(RESET != (USART_REG_VAL(usart_periph, flag) & BIT(USART_BIT_POS(flag)))){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @描述       清除USART状态寄存器标志位
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  flag: USART标志位，参考枚举 usart_flag_enum
                只能选择一个参数，如下所示:
      @arg        USART_FLAG_CTS: CTS变化标志
      @arg        USART_FLAG_LBD: LIN断开检测标志
      @arg        USART_FLAG_TC: 发送完成标志 
      @arg        USART_FLAG_RBNE: 读数据缓冲区非空标志
      @arg        USART_FLAG_EB: 块结束标志
      @arg        USART_FLAG_RT: 接收超时标志
    @参数[输出] 无
    @返回值     无
*/
void usart_flag_clear(uint32_t usart_periph, usart_flag_enum flag)
{
    USART_REG_VAL(usart_periph, flag) &= ~BIT(USART_BIT_POS(flag));
}

/*!
    @描述       使能USART中断
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  interrupt: USART 中断, 参考枚举 usart_interrupt_enum
                只能选择一个参数，如下所示:
      @arg        USART_INT_PERR: 校验错误中断
      @arg        USART_INT_TBE: 发送缓冲区空中断
      @arg        USART_INT_TC: 发送完成中断
      @arg        USART_INT_RBNE: 读数据缓冲区非空中断和过载错误中断
      @arg        USART_INT_IDLE: IDLE线检测中断
      @arg        USART_INT_LBD: LIN断开信号检测中断
      @arg        USART_INT_ERR: 错误中断
      @arg        USART_INT_CTS:CTS中断
      @arg        USART_INT_RT: 接收超时事件中断
      @arg        USART_INT_EB: 块结束事件中断
    @参数[输出] 无
    @返回值     无
*/
void usart_interrupt_enable(uint32_t usart_periph, usart_interrupt_enum interrupt)
{
    USART_REG_VAL(usart_periph, interrupt) |= BIT(USART_BIT_POS(interrupt));
}

/*!
    @描述       失能USART中断
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  interrupt: USART 中断, 参考枚举 usart_interrupt_enum
                只能选择一个参数，如下所示:
      @arg        USART_INT_PERR: 校验错误中断
      @arg        USART_INT_TBE: 发送缓冲区空中断
      @arg        USART_INT_TC: 发送完成中断
      @arg        USART_INT_RBNE: 读数据缓冲区非空中断和过载错误中断
      @arg        USART_INT_IDLE: IDLE线检测中断
      @arg        USART_INT_LBD: LIN断开信号检测中断
      @arg        USART_INT_ERR: 错误中断
      @arg        USART_INT_CTS:CTS中断
      @arg        USART_INT_RT: 接收超时事件中断
      @arg        USART_INT_EB: 块结束事件中断
    @参数[输出] 无
    @返回值     无
*/
void usart_interrupt_disable(uint32_t usart_periph, usart_interrupt_enum interrupt)
{
    USART_REG_VAL(usart_periph, interrupt) &= ~BIT(USART_BIT_POS(interrupt));
}

/*!
    @描述      获取USART中断标志位状态
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  int_flag: USART中断标志，参考枚举 usart_interrupt_flag_enum
                只能选择一个参数，如下所示:
      @arg        USART_INT_FLAG_PERR: 校验错误中断标志
      @arg        USART_INT_FLAG_TBE: 发送缓冲区空中断标志
      @arg        USART_INT_FLAG_TC: 发送完成中断标志
      @arg        USART_INT_FLAG_RBNE: 读数据缓冲区非空中断标志
      @arg        USART_INT_FLAG_RBNE_ORERR: 读数据缓冲区非空中断和溢出错误中断标志
      @arg        USART_INT_FLAG_IDLE: IDLE线检测中断标志
      @arg        USART_INT_FLAG_LBD: LIN断开检测中断标志
      @arg        USART_INT_FLAG_CTS: CTS中断标志
      @arg        USART_INT_FLAG_ERR_ORERR: 过载错误中断标志
      @arg        USART_INT_FLAG_ERR_NERR: 噪声错误中断标志
      @arg        USART_INT_FLAG_ERR_FERR: 帧错误中断标志
      @arg        USART_INT_FLAG_EB: 块结束事件中断标志
      @arg        USART_INT_FLAG_RT: 超时事件中断标志
    @参数[输出] none
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus usart_interrupt_flag_get(uint32_t usart_periph, usart_interrupt_flag_enum int_flag)
{
    uint32_t intenable = 0U, flagstatus = 0U;
    /* get the interrupt enable bit status */
    intenable = (USART_REG_VAL(usart_periph, int_flag) & BIT(USART_BIT_POS(int_flag)));
    /* get the corresponding flag bit status */
    flagstatus = (USART_REG_VAL2(usart_periph, int_flag) & BIT(USART_BIT_POS2(int_flag)));

    if((0U != flagstatus) && (0U != intenable)){
        return SET;
    }else{
        return RESET; 
    }
}

/*!
    @描述       清除USART中断标志位状态
    @参数[输入]  usart_periph: USARTx(x=0,1,2)/UARTx(x=3,4)
    @参数[输入]  int_flag: USART中断标志，参考枚举 usart_interrupt_flag_enum
                只能选择一个参数，如下所示:
      @arg        USART_INT_FLAG_CTS: CTS变化中断标志
      @arg        USART_INT_FLAG_LBD: LIN断开检测中断标志
      @arg        USART_INT_FLAG_TC: 发送完成中断标志
      @arg        USART_INT_FLAG_RBNE: 读数据缓冲区非空中断标志
      @arg        USART_INT_FLAG_EB: 块结束事件中断标志
      @arg        USART_INT_FLAG_RT: 超时事件中断标志
    @参数[输出] 无
    @返回值     无
*/
void usart_interrupt_flag_clear(uint32_t usart_periph, usart_interrupt_flag_enum int_flag)
{
    USART_REG_VAL2(usart_periph, int_flag) &= ~BIT(USART_BIT_POS2(int_flag));
}
