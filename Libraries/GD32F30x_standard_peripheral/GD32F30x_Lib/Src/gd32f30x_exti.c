/*!
    \file    gd32f30x_exti.c
    \brief   EXTI driver

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

#include "gd32f30x_exti.h"

/*!
    @简介        复位EXTI
    @参数[输入]  无
    @参数[输出]  无
    @返回值      无
*/
void exti_deinit(void)
{
    /* 重置所有EXTI寄存器的值 */
    EXTI_INTEN = (uint32_t)0x00000000U;
    EXTI_EVEN  = (uint32_t)0x00000000U;
    EXTI_RTEN  = (uint32_t)0x00000000U;
    EXTI_FTEN  = (uint32_t)0x00000000U;
    EXTI_SWIEV = (uint32_t)0x00000000U;
}

/*!
    @简介      初始化EXTI
    @参数[输入]  linex: EXTI线号, 参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI线x
    @参数[输入]  mode: 中断或事件模式，请参考exti_mode_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_INTERRUPT: 中断模式
      @arg        EXTI_EVENT: 事件模式
    @参数[输入]  trig_type: 中断触发类型，参考exti_trig_type_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_TRIG_RISING: 上升沿触发
      @arg        EXTI_TRIG_FALLING: 下降沿触发
      @arg        EXTI_TRIG_BOTH: 任意沿触发
    @参数[输出] 无
    @返回值     无
*/
void exti_init(exti_line_enum linex, exti_mode_enum mode, exti_trig_type_enum trig_type)
{
    /* 重置EXTI线x */
    EXTI_INTEN &= ~(uint32_t)linex;
    EXTI_EVEN &= ~(uint32_t)linex;
    EXTI_RTEN &= ~(uint32_t)linex;
    EXTI_FTEN &= ~(uint32_t)linex;
    
    /* 设置EXTI模式并启用来自EXTI线x的中断或事件 */
    switch(mode){
    case EXTI_INTERRUPT:
        EXTI_INTEN |= (uint32_t)linex;
        break;
    case EXTI_EVENT:
        EXTI_EVEN |= (uint32_t)linex;
        break;
    default:
        break;
    }
    
    /* 设置EXTI触发类型 */
    switch(trig_type){
    case EXTI_TRIG_RISING:
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN &= ~(uint32_t)linex;
        break;
    case EXTI_TRIG_FALLING:
        EXTI_RTEN &= ~(uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
        break;
    case EXTI_TRIG_BOTH:
        EXTI_RTEN |= (uint32_t)linex;
        EXTI_FTEN |= (uint32_t)linex;
        break;
    default:
        break;
    }
}

/*!
    @简介       使能EXTIx中断
    @参数[输入]  linex: EXTI 线号,请参考 exti_line_enum
                只能选择一个参数，如下所示:
      \arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_interrupt_enable(exti_line_enum linex)
{
    EXTI_INTEN |= (uint32_t)linex;
}

/*!
    @简介       EXTI线x事件使能
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_event_enable(exti_line_enum linex)
{
    EXTI_EVEN |= (uint32_t)linex;
}

/*!
    @简介       失能EXTIx中断
    @参数[输入]  linex: EXTI EXTI 线号,请参考 exti_line_enum
                只能选择一个参数，如下所示
      \arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_interrupt_disable(exti_line_enum linex)
{
    EXTI_INTEN &= ~(uint32_t)linex;
}

/*!
    @简介       EXTI线x事件禁能
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_event_disable(exti_line_enum linex)
{
    EXTI_EVEN &= ~(uint32_t)linex;
}

/*!
    @简介       获取EXTI线x标志位
    @参数[输入]  linex: EXTI线, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     FlagStatus: 标志位状态 (RESET or SET)
*/
FlagStatus exti_flag_get(exti_line_enum linex)
{
    if(RESET != (EXTI_PD & (uint32_t)linex)){
        return SET;
    }else{
        return RESET;
    } 
}

/*!
    @简介      清除EXTI线x标志位
    @参数[输入]  linex: EXTI线, 请参考 exti_line_enum
                EXTI线:
      \arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_flag_clear(exti_line_enum linex)
{
    EXTI_PD = (uint32_t)linex;
}

/*!
    @简介      获取EXTI线x中断标志位
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择                                                                                                                                                                                                                            一个参数，如下所示:
      \arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     FlagStatus: 中断标志位状态 (RESET or SET)
*/
FlagStatus exti_interrupt_flag_get(exti_line_enum linex)
{
    uint32_t flag_left, flag_right;
    
    flag_left = EXTI_PD & (uint32_t)linex;
    flag_right = EXTI_INTEN & (uint32_t)linex;
    
    if((RESET != flag_left) && (RESET != flag_right)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @简介      清除EXTI线x中断标志位
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_interrupt_flag_clear(exti_line_enum linex)
{
    EXTI_PD = (uint32_t)linex;
}

/*!
    @简介       EXTI线x软件中断使能
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      \arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_software_interrupt_enable(exti_line_enum linex)
{
    EXTI_SWIEV |= (uint32_t)linex;
}

/*!
    @简介       EXTI线x软件中断禁能
    @参数[输入]  linex: EXTI线x, 请参考 exti_line_enum
                只能选择一个参数，如下所示:
      @arg        EXTI_x (x=0..19): EXTI line x
    @参数[输出] 无
    @返回值     无
*/
void exti_software_interrupt_disable(exti_line_enum linex)
{
    EXTI_SWIEV &= ~(uint32_t)linex;
}
