/*!
    \file    gd32f30x_timer.c
    \brief   TIMER driver

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

#include "gd32f30x_timer.h"

/*!
    @描述       复位外设TIMERx
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_deinit(uint32_t timer_periph)
{
    switch(timer_periph){
    case TIMER0:
        /* 复位 TIMER0 */
        rcu_periph_reset_enable(RCU_TIMER0RST);
        rcu_periph_reset_disable(RCU_TIMER0RST);
        break;
    case TIMER1:
        /* 复位 TIMER1 */
        rcu_periph_reset_enable(RCU_TIMER1RST);
        rcu_periph_reset_disable(RCU_TIMER1RST);
        break;
    case TIMER2:
        /* 复位 TIMER2 */
        rcu_periph_reset_enable(RCU_TIMER2RST);
        rcu_periph_reset_disable(RCU_TIMER2RST);
        break;
    case TIMER3:
        /* 复位 TIMER3 */
        rcu_periph_reset_enable(RCU_TIMER3RST);
        rcu_periph_reset_disable(RCU_TIMER3RST);
        break;
    case TIMER4:
        /* 复位 TIMER4 */
        rcu_periph_reset_enable(RCU_TIMER4RST);
        rcu_periph_reset_disable(RCU_TIMER4RST);
        break;
    case TIMER5:
        /* 复位 TIMER5 */
        rcu_periph_reset_enable(RCU_TIMER5RST);
        rcu_periph_reset_disable(RCU_TIMER5RST);
        break;
    case TIMER6:
        /* 复位 TIMER6 */
        rcu_periph_reset_enable(RCU_TIMER6RST);
        rcu_periph_reset_disable(RCU_TIMER6RST);
        break;
    case TIMER7:
        /* 复位 TIMER7 */
        rcu_periph_reset_enable(RCU_TIMER7RST);
        rcu_periph_reset_disable(RCU_TIMER7RST);
        break;
#ifndef GD32F30X_HD
    case TIMER8:
        /* 复位 TIMER8 */
        rcu_periph_reset_enable(RCU_TIMER8RST);
        rcu_periph_reset_disable(RCU_TIMER8RST);
        break;
    case TIMER9:
        /* 复位 TIMER9 */
        rcu_periph_reset_enable(RCU_TIMER9RST);
        rcu_periph_reset_disable(RCU_TIMER9RST);
        break;
    case TIMER10:
        /* 复位 TIMER10 */
        rcu_periph_reset_enable(RCU_TIMER10RST);
        rcu_periph_reset_disable(RCU_TIMER10RST);
        break;
    case TIMER11:
        /* 复位 TIMER11 */
        rcu_periph_reset_enable(RCU_TIMER11RST);
        rcu_periph_reset_disable(RCU_TIMER11RST);
        break;
    case TIMER12:
        /* 复位 TIMER12 */
        rcu_periph_reset_enable(RCU_TIMER12RST);
        rcu_periph_reset_disable(RCU_TIMER12RST);
        break;
    case TIMER13:
        /* 复位 TIMER13 */
        rcu_periph_reset_enable(RCU_TIMER13RST);
        rcu_periph_reset_disable(RCU_TIMER13RST);
        break;
#endif /* GD32F30X_HD */
    default:
        break;
    }
}

/*!
    @描述       将TIMER初始化参数结构体中所有参数初始化为默认值
    @参数[输入]  initpara: 初始化参数结构，详情见timer_parameter_struct
    @参数[输出] 无
    @返回值     无
*/
void timer_struct_para_init(timer_parameter_struct* initpara)
{
    /* 用默认值初始化结构体成员 */
    initpara->prescaler         = 0U;
    initpara->alignedmode       = TIMER_COUNTER_EDGE;
    initpara->counterdirection  = TIMER_COUNTER_UP;
    initpara->period            = 65535U;
    initpara->clockdivision     = TIMER_CKDIV_DIV1;
    initpara->repetitioncounter = 0U;
}

/*!
    @描述       初始化定时器
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输入]  initpara: TIMER初始化结构体，详情见 timer_parameter_struct
                  prescaler: 计数器的预分频值, 0~65535
                  alignedmode: 对齐方式，TIMER_COUNTER_EDGE, TIMER_COUNTER_CENTER_DOWN, TIMER_COUNTER_CENTER_UP, TIMER_COUNTER_CENTER_BOTH
                  counterdirection: 计数方向，TIMER_COUNTER_UP, TIMER_COUNTER_DOWN
                  period: 计数器自动装填值, 0~65535
                  clockdivision: 时钟分频 TIMER_CKDIV_DIV1, TIMER_CKDIV_DIV2, TIMER_CKDIV_DIV4
                  repetitioncounter: 重复计数值, 0~255
    @参数[输出] 无
    @返回值     无
*/
void timer_init(uint32_t timer_periph, timer_parameter_struct* initpara)
{
    /* 配置计数器预分频器值 */
    TIMER_PSC(timer_periph) = (uint16_t)initpara->prescaler;

    /* 配置计数器方向和对齐方式 */
    if((TIMER0 == timer_periph) || (TIMER1 == timer_periph) || (TIMER2 == timer_periph)
        || (TIMER3 == timer_periph) || (TIMER4 == timer_periph) || (TIMER7 == timer_periph)){
        TIMER_CTL0(timer_periph) &= ~(uint32_t)(TIMER_CTL0_DIR|TIMER_CTL0_CAM);
        TIMER_CTL0(timer_periph) |= (uint32_t)initpara->alignedmode;
        TIMER_CTL0(timer_periph) |= (uint32_t)initpara->counterdirection;
    }

    /* 配置自动加载值 */
    TIMER_CAR(timer_periph) = (uint32_t)initpara->period;

    if((TIMER5 != timer_periph) && (TIMER6 != timer_periph)){
        /* 复位 CKDIV 位 */
        TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_CKDIV;
        TIMER_CTL0(timer_periph) |= (uint32_t)initpara->clockdivision;
    }

    if((TIMER0 == timer_periph) || (TIMER7 == timer_periph)){
        /* 配置重复计数值 */
        TIMER_CREP(timer_periph) = (uint32_t)initpara->repetitioncounter;
    }

    /* 产生更新事件 */
    TIMER_SWEVG(timer_periph) |= (uint32_t)TIMER_SWEVG_UPG;
}

/*!
    @描述       使能定时器
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_enable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) |= (uint32_t)TIMER_CTL0_CEN;
}

/*!
    @brief      失能定时器
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_disable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_CEN;
}

/*!
    @描述       定时器自动重载影子使能
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_auto_reload_shadow_enable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) |= (uint32_t)TIMER_CTL0_ARSE;
}

/*!
    @描述       定时器自动重载影子禁能
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_auto_reload_shadow_disable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_ARSE;
}

/*!
    @描述       更新使能
    @参数[输入] timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_update_event_enable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_UPDIS;
}

/*!
    @描述       更新禁能
    @参数[输入] timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     无
*/
void timer_update_event_disable(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) |= (uint32_t) TIMER_CTL0_UPDIS;
}

/*!
    @描述       设置定时器的对齐模式
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  aligned:对齐模式
                只能选择一个参数，如下所示:
      @arg        TIMER_COUNTER_EDGE: 边沿对齐模式)
      @arg        TIMER_COUNTER_CENTER_DOWN: 中央对齐向下计数模式
      @arg        TIMER_COUNTER_CENTER_UP: 中央对齐向上计数模式
      @arg        TIMER_COUNTER_CENTER_BOTH: 中央对齐上下计数模式
    @参数[输出] 无
    @返回值     无
*/
void timer_counter_alignment(uint32_t timer_periph, uint16_t aligned)
{
    TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_CAM;
    TIMER_CTL0(timer_periph) |= (uint32_t)aligned;
}

/*!
    @描述       设置定时器向上计数
	@条件       计数器设置为无中央对齐计数模式（边沿对齐模式）
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_counter_up_direction(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_DIR;
}

/*!
    @描述       设置定时器向下计数
	@条件       计数器设置为无中央对齐计数模式（边沿对齐模式）
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_counter_down_direction(uint32_t timer_periph)
{
    TIMER_CTL0(timer_periph) |= (uint32_t)TIMER_CTL0_DIR;
}

/*!
    @描述       配置定时器预分频器
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输入]  prescaler: 预分频值,0~65535
    @参数[输入]  pscreload: 预分频值加载模式
                只能选择一个参数，如下所示:
      @arg        TIMER_PSC_RELOAD_NOW: 预分频值立即加载
      @arg        TIMER_PSC_RELOAD_UPDATE: 预分频值在下次更新事件发生时加载
    @参数[输出] 无
    @返回值     无
*/
void timer_prescaler_config(uint32_t timer_periph, uint16_t prescaler, uint8_t pscreload)
{
    TIMER_PSC(timer_periph) = (uint32_t)prescaler;
    
    if(TIMER_PSC_RELOAD_NOW == pscreload){
        TIMER_SWEVG(timer_periph) |= (uint32_t)TIMER_SWEVG_UPG;
    }
}

/*!
    \brief      configure TIMER repetition register value
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  repetition: the counter repetition value,0~255
    \param[out] none
    \retval     none
*/
void timer_repetition_value_config(uint32_t timer_periph, uint16_t repetition)
{
    TIMER_CREP(timer_periph) = (uint32_t)repetition;
} 
 
/*!
    @描述       配置定时器自动重载寄存器
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输入]  autoreload: 计数器自动重载值,0~65535
    @参数[输出] 无
    @返回值     无
*/         
void timer_autoreload_value_config(uint32_t timer_periph, uint16_t autoreload)
{
    TIMER_CAR(timer_periph) = (uint32_t)autoreload;
}

/*!
    @描述       配置定时器的计数器值
    @参数[输入] timer_periph: TIMERx(x=0..13)
    @参数[输入] counter: 计数器,0~65535
    @参数[输出] 无
    @返回值     无
*/         
void timer_counter_value_config(uint32_t timer_periph, uint16_t counter)
{
    TIMER_CNT(timer_periph) = (uint32_t)counter;
}

/*!
    @描述      读取定时器的计数器值
    @参数[in]  timer_periph: TIMERx(x=0..13)
    @参数[out] none
    @返回值     计数值
*/         
uint32_t timer_counter_read(uint32_t timer_periph)
{
    uint32_t count_value = 0U;
    count_value = TIMER_CNT(timer_periph);
    return (count_value);
}

/*!
    @描述       读取TIMER的预分频器值
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输出] 无
    @返回值     预分频器值
*/         
uint16_t timer_prescaler_read(uint32_t timer_periph)
{
    uint16_t prescaler_value = 0U;
    prescaler_value = (uint16_t)(TIMER_PSC(timer_periph));
    return (prescaler_value);
}

/*!
    @描述       配置TIMER的单脉冲模式
    @参数[输入]  timer_periph: TIMERx(x=0..8,11)
    @参数[输入]  spmode:
                只能选择一个参数，如下所示:
      @arg        TIMER_SP_MODE_SINGLE: 单脉冲模式
      @arg        TIMER_SP_MODE_REPETITIVE: 重复脉冲模式
    @参数[输出] 无
    @返回值     无
*/
void timer_single_pulse_mode_config(uint32_t timer_periph, uint32_t spmode)
{
    if(TIMER_SP_MODE_SINGLE == spmode){
        TIMER_CTL0(timer_periph) |= (uint32_t)TIMER_CTL0_SPM;
    }else if(TIMER_SP_MODE_REPETITIVE == spmode){
        TIMER_CTL0(timer_periph) &= ~((uint32_t)TIMER_CTL0_SPM);
    }else{
        /* 非法输入参数  */
    }
}

/*!
    @描述       配置TIMER的更新源 
    @参数[输入]  timer_periph: TIMERx(x=0..13)
    @参数[输入]  update:更新源
				只能选择一个参数，如下所示:
      @arg        TIMER_UPDATE_SRC_GLOBAL: 下述任一事件产生更新中断或DMA请求：
											– UPG位被置1 
											– 计数器溢出/下溢
											– 从模式控制器产生的更新
	  @arg        TIMER_UPDATE_SRC_REGULAR: 只有计数器溢出/ 下溢才产生更新中断或DMA请求
    @参数[输出] 无
    @返回值     无
*/
void timer_update_source_config(uint32_t timer_periph, uint32_t update)
{
    if(TIMER_UPDATE_SRC_REGULAR == update){
        TIMER_CTL0(timer_periph) |= (uint32_t)TIMER_CTL0_UPS;
    }else if(TIMER_UPDATE_SRC_GLOBAL == update){
        TIMER_CTL0(timer_periph) &= ~(uint32_t)TIMER_CTL0_UPS;
    }else{
        /* 非法输入参数 */
    }
}

/*!
    @描述       定时器中断使能 
    @参数[输入]  timer_periph: 请参考以下参数 
    @参数[输入]  interrupt: 中断源
                只能选择一个参数，如下所示:
      @arg        TIMER_INT_UP: 更新中断, TIMERx(x=0..13)
      @arg        TIMER_INT_CH0: 通道0比较/捕获中断, TIMERx(x=0..4,7..13)
      @arg        TIMER_INT_CH1: 通道1比较/捕获中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_CH2: 通道2比较/捕获中断, TIMERx(x=0..4,7)
      @arg        TIMER_INT_CH3: 通道3比较/捕获中断 , TIMERx(x=0..4,7)
      @arg        TIMER_INT_CMT: 换相更新中断, TIMERx(x=0,7)
      @arg        TIMER_INT_TRG: 触发中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_BRK: 中止中断, TIMERx(x=0,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_interrupt_enable(uint32_t timer_periph, uint32_t interrupt)
{
    TIMER_DMAINTEN(timer_periph) |= (uint32_t) interrupt; 
}

/*!
    @描述       定时器中断禁能
    @参数[输入]  timer_periph: 请参考以下参数 
    @参数[输入]  interrupt: 中断源
                只能选择一个参数，如下所示:
      @arg        TIMER_INT_UP: 更新中断, TIMERx(x=0..13)
      @arg        TIMER_INT_CH0: 通道0比较/捕获中断, TIMERx(x=0..4,7..13)
      @arg        TIMER_INT_CH1: 通道1比较/捕获中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_CH2: 通道2比较/捕获中断, TIMERx(x=0..4,7)
      @arg        TIMER_INT_CH3: 通道3比较/捕获中断 , TIMERx(x=0..4,7)
      @arg        TIMER_INT_CMT: 换相更新中断, TIMERx(x=0,7)
      @arg        TIMER_INT_TRG: 触发中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_BRK: 中止中断, TIMERx(x=0,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_interrupt_disable(uint32_t timer_periph, uint32_t interrupt)
{
    TIMER_DMAINTEN(timer_periph) &= (~(uint32_t)interrupt); 
}

/*!
    @描述       获取定时器中断标志
    @参数[输入]  timer_periph: 请参考以下参数 
    @参数[输入]  interrupt: 中断源
                只能选择一个参数，如下所示:
      @arg        TIMER_INT_FLAG_UP: 更新中断, TIMERx(x=0..13)
      @arg        TIMER_INT_FLAG_CH0: 通道0比较/捕获中断, TIMERx(x=0..4,7..13)
      @arg        TIMER_INT_FLAG_CH1: 通道1比较/捕获中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_FLAG_CH2: 通道2比较/捕获中断, TIMERx(x=0..4,7)
      @arg        TIMER_INT_FLAG_CH3: 通道3比较/捕获中断 , TIMERx(x=0..4,7)
      @arg        TIMER_INT_FLAG_CMT: 换相更新中断, TIMERx(x=0,7)
      @arg        TIMER_INT_FLAG_TRG: 触发中断, TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_FLAG_BRK: 中止中断, TIMERx(x=0,7)
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus timer_interrupt_flag_get(uint32_t timer_periph, uint32_t interrupt)
{
    uint32_t val;
    val = (TIMER_DMAINTEN(timer_periph) & interrupt);
    if((RESET != (TIMER_INTF(timer_periph) & interrupt) ) && (RESET != val)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @描述       清除定时器中断标志
    @参数[输入]  timer_periph: 请参考以下参数 
    @参数[输入]  interrupt: 中断源
                只能选择一个参数，如下所示:
      @arg        TIMER_INT_FLAG_UP: 更新中断,TIMERx(x=0..13)
      @arg        TIMER_INT_FLAG_CH0: 通道0比较/捕获中断,TIMERx(x=0..4,7..13)
      @arg        TIMER_INT_FLAG_CH1: 通道1比较/捕获中断,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_INT_FLAG_CH2: 通道2比较/捕获中断,TIMERx(x=0..4,7)
      @arg        TIMER_INT_FLAG_CH3: 通道3比较/捕获中断,TIMERx(x=0..4,7)
      @arg        TIMER_INT_FLAG_CMT: 换相更新中断,TIMERx(x=0,7) 
      @arg        TIMER_INT_FLAG_TRG: 触发中断,TIMERx(x=0,7,8,11)
      @arg        TIMER_INT_FLAG_BRK:  中止中断,TIMERx(x=0,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_interrupt_flag_clear(uint32_t timer_periph, uint32_t interrupt)
{
    TIMER_INTF(timer_periph) = (~(uint32_t)interrupt);
}

/*!
    @描述      获取定时器的状态标志
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输出]  flag: 状态标志
                只能选择一个参数，如下所示:
      @arg        TIMER_FLAG_UP: 更新标志,TIMERx(x=0..13)
      @arg        TIMER_FLAG_CH0: 通道0比较/捕获标志,TIMERx(x=0..4,7..13)
      @arg        TIMER_FLAG_CH1: 通道1比较/捕获标志,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_FLAG_CH2: 通道2比较/捕获标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CH3: 通道3比较/捕获标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CMT: 通道换相更新标志,TIMERx(x=0,7) 
      @arg        TIMER_FLAG_TRG: 触发标志,TIMERx(x=0,7,8,11) 
      @arg        TIMER_FLAG_BRK: 中止标志位,TIMERx(x=0,7)
      @arg        TIMER_FLAG_CH0O: 通道0捕获溢出标志,TIMERx(x=0..4,7..11)
      @arg        TIMER_FLAG_CH1O: 通道1捕获溢出标志,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_FLAG_CH2O: 通道2捕获溢出标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CH3O: 通道3捕获溢出标志,TIMERx(x=0..4,7)
    @参数[输出] none
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus timer_flag_get(uint32_t timer_periph, uint32_t flag)
{
    if(RESET != (TIMER_INTF(timer_periph) & flag)){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @描述      清除定时器的状态标志
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输出]  flag: 状态标志
                只能选择一个参数，如下所示:
      @arg        TIMER_FLAG_UP: 更新标志,TIMERx(x=0..13)
      @arg        TIMER_FLAG_CH0: 通道0比较/捕获标志,TIMERx(x=0..4,7..13)
      @arg        TIMER_FLAG_CH1: 通道1比较/捕获标志,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_FLAG_CH2: 通道2比较/捕获标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CH3: 通道3比较/捕获标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CMT: 通道换相更新标志,TIMERx(x=0,7) 
      @arg        TIMER_FLAG_TRG: 触发标志,TIMERx(x=0,7,8,11) 
      @arg        TIMER_FLAG_BRK: 中止标志位,TIMERx(x=0,7)
      @arg        TIMER_FLAG_CH0O: 通道0捕获溢出标志,TIMERx(x=0..4,7..11)
      @arg        TIMER_FLAG_CH1O: 通道1捕获溢出标志,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_FLAG_CH2O: 通道2捕获溢出标志,TIMERx(x=0..4,7)
      @arg        TIMER_FLAG_CH3O: 通道3捕获溢出标志,TIMERx(x=0..4,7)
    @参数[输出] none
    @返回值     无
*/
void timer_flag_clear(uint32_t timer_periph, uint32_t flag)
{
    TIMER_INTF(timer_periph) = (~(uint32_t)flag);
}

/*!
    @描述       使能定时器的DMA
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  dma: 指定要启用哪个DMA
                只能选择一个参数，如下所示:
      @arg        TIMER_DMA_UPD:  更新DMA请求,TIMERx(x=0..7)
      @arg        TIMER_DMA_CH0D: 通道0比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH1D: 通道1比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH2D: 通道2比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH3D: 通道3比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CMTD: 换相DMA更新请求,TIMERx(x=0,7)
      @arg        TIMER_DMA_TRGD: 触发DMA请求使能,TIMERx(x=0..4,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_dma_enable(uint32_t timer_periph, uint16_t dma)
{
    TIMER_DMAINTEN(timer_periph) |= (uint32_t) dma; 
}

/*!
    @描述       禁能定时器的DMA
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  dma: 指定要启用哪个DMA
                指定要启用哪个DMA
      @arg        TIMER_DMA_UPD:  更新DMA请求 ,TIMERx(x=0..7)
      @arg        TIMER_DMA_CH0D: 通道0比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH1D: 通道0比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH2D: 通道0比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CH3D: 通道0比较/捕获 DMA请求,TIMERx(x=0..4,7)
      @arg        TIMER_DMA_CMTD: 换相DMA更新请求 ,TIMERx(x=0,7)
      @arg        TIMER_DMA_TRGD: 触发DMA请求使能,TIMERx(x=0..4,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_dma_disable(uint32_t timer_periph, uint16_t dma)
{
    TIMER_DMAINTEN(timer_periph) &= (~(uint32_t)(dma)); 
}

/*!
    @描述       定时器的通道DMA请求源选择
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  dma_request: 通道的DMA请求源选择
                只能选择一个参数，如下所示:
       @arg        TIMER_DMAREQUEST_CHANNELEVENT: 当通道捕获/比较事件发生时，发送通道n的DMA请求
       @arg        TIMER_DMAREQUEST_UPDATEEVENT: 当更新事件发生，发送通道n的DMA请求 
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_dma_request_source_select(uint32_t timer_periph, uint8_t dma_request)
{
    if(TIMER_DMAREQUEST_UPDATEEVENT == dma_request){
        TIMER_CTL1(timer_periph) |= (uint32_t)TIMER_CTL1_DMAS;
    }else if(TIMER_DMAREQUEST_CHANNELEVENT == dma_request){
        TIMER_CTL1(timer_periph) &= ~(uint32_t)TIMER_CTL1_DMAS;
    }else{
        /* 非法输入参数 */
    }
}

/*!
    @描述       配置定时器的DMA模式
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  dma_baseaddr: DMA传输起始地址
                只能选择一个参数，如下所示:
       @arg        TIMER_DMACFG_DMATA_CTL0: DMA传输起始地址： TIMER_CTL0,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CTL1: DMA传输起始地址： TIMER_CTL1,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_SMCFG: DMA传输起始地址： TIMER_SMCFG,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_DMAINTEN: DDMA传输起始地址： TIMER_DMAINTEN,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_INTF: DMA传输起始地址： TIMER_INTF,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_SWEVG: DMA传输起始地址： TIMER_SWEVG,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CHCTL0: DMA传输起始地址： TIMER_CHCTL0,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CHCTL1: DMA传输起始地址： TIMER_CHCTL1,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CHCTL2: DMA传输起始地址： TIMER_CHCTL2,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CNT: DMA传输起始地址： TIMER_CNT,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_PSC: DMA传输起始地址： TIMER_PSC,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CAR: DMA传输起始地址： TIMER_CAR,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CREP: DMA传输起始地址： TIMER_CREP,TIMERx(x=0,7)
       @arg        TIMER_DMACFG_DMATA_CH0CV: DMA传输起始地址： TIMER_CH0CV,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CH1CV: DMA传输起始地址： TIMER_CH1CV,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CH2CV: DMA传输起始地址： TIMER_CH2CV,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CH3CV: DMA传输起始地址： TIMER_CH3CV,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_CCHP: DMA传输起始地址： TIMER_CCHP,TIMERx(x=0,7)
       @arg        TIMER_DMACFG_DMATA_DMACFG: DMA传输起始地址： TIMER_DMACFG,TIMERx(x=0..4,7)
       @arg        TIMER_DMACFG_DMATA_DMATB: DMA传输起始地址： TIMER_DMATB,TIMERx(x=0..4,7)
    @参数[输入]  dma_lenth: DMA传输长度
                只能选择一个参数，如下所示:
       @arg        TIMER_DMACFG_DMATC_xTRANSFER(x=1..18): DMA传输 x 次
    @参数[输出] 无
    @返回值     无
*/
void timer_dma_transfer_config(uint32_t timer_periph, uint32_t dma_baseaddr, uint32_t dma_lenth)
{
    TIMER_DMACFG(timer_periph) &= (~(uint32_t)(TIMER_DMACFG_DMATA | TIMER_DMACFG_DMATC));
    TIMER_DMACFG(timer_periph) |= (uint32_t)(dma_baseaddr | dma_lenth);
}

/*!
    @描述       软件产生事件
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  event: 该定时器软件的事件生成源
                可以选择一个或多个参数，如下所示:
      @arg        TIMER_EVENT_SRC_UPG: 更新事件产生,TIMERx(x=0..13)
      @arg        TIMER_EVENT_SRC_CH0G: 通道0捕获或比较事件发生,TIMERx(x=0..4,7..13) 
      @arg        TIMER_EVENT_SRC_CH1G: 通道1捕获或比较事件发生,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_EVENT_SRC_CH2G: 通道2捕获或比较事件发生,TIMERx(x=0..4,7) 
      @arg        TIMER_EVENT_SRC_CH3G: 通道3捕获或比较事件发生,TIMERx(x=0..4,7) 
      @arg        TIMER_EVENT_SRC_CMTG: 通道换相更新事件发生,TIMERx(x=0,7) 
      @arg        TIMER_EVENT_SRC_TRGG: 触发事件产生,TIMERx(x=0..4,7,8,11)
      @arg        TIMER_EVENT_SRC_BRKG: 产生中止事件,TIMERx(x=0,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_event_software_generate(uint32_t timer_periph, uint16_t event)
{
    TIMER_SWEVG(timer_periph) |= (uint32_t)event;
}

/*!
    \brief      initialize TIMER break parameter struct with a default value
    \param[in]  breakpara: TIMER break parameter struct
    \param[out] none
    \retval     none
*/
void timer_break_struct_para_init(timer_break_parameter_struct* breakpara)
{
    /* initialize the break parameter struct member with the default value */
    breakpara->runoffstate     = TIMER_ROS_STATE_DISABLE;
    breakpara->ideloffstate    = TIMER_IOS_STATE_DISABLE;
    breakpara->deadtime        = 0U;
    breakpara->breakpolarity   = TIMER_BREAK_POLARITY_LOW;
    breakpara->outputautostate = TIMER_OUTAUTO_DISABLE;
    breakpara->protectmode     = TIMER_CCHP_PROT_OFF;
    breakpara->breakstate      = TIMER_BREAK_DISABLE;
}

/*!
    \brief      configure TIMER break function 
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  breakpara: TIMER break parameter struct
                runoffstate: TIMER_ROS_STATE_ENABLE,TIMER_ROS_STATE_DISABLE
                ideloffstate: TIMER_IOS_STATE_ENABLE,TIMER_IOS_STATE_DISABLE
                deadtime: 0~255
                breakpolarity: TIMER_BREAK_POLARITY_LOW,TIMER_BREAK_POLARITY_HIGH
                outputautostate: TIMER_OUTAUTO_ENABLE,TIMER_OUTAUTO_DISABLE
                protectmode: TIMER_CCHP_PROT_OFF,TIMER_CCHP_PROT_0,TIMER_CCHP_PROT_1,TIMER_CCHP_PROT_2
                breakstate: TIMER_BREAK_ENABLE,TIMER_BREAK_DISABLE
    \param[out] none
    \retval     none
*/
void timer_break_config(uint32_t timer_periph, timer_break_parameter_struct* breakpara)
{
    TIMER_CCHP(timer_periph) = (uint32_t)(((uint32_t)(breakpara->runoffstate))|
                                          ((uint32_t)(breakpara->ideloffstate))|
                                          ((uint32_t)(breakpara->deadtime))|
                                          ((uint32_t)(breakpara->breakpolarity))|
                                          ((uint32_t)(breakpara->outputautostate)) |
                                          ((uint32_t)(breakpara->protectmode))|
                                          ((uint32_t)(breakpara->breakstate))) ;
}

/*!
    \brief      enable TIMER break function
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[out] none
    \retval     none
*/
void timer_break_enable(uint32_t timer_periph)
{
    TIMER_CCHP(timer_periph) |= (uint32_t)TIMER_CCHP_BRKEN;
}

/*!
    \brief      disable TIMER break function
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[out] none
    \retval     none
*/
void timer_break_disable(uint32_t timer_periph)
{
    TIMER_CCHP(timer_periph) &= ~(uint32_t)TIMER_CCHP_BRKEN;
}

/*!
    \brief      enable TIMER output automatic function
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[out] none
    \retval     none
*/
void timer_automatic_output_enable(uint32_t timer_periph)
{
    TIMER_CCHP(timer_periph) |= (uint32_t)TIMER_CCHP_OAEN;
}

/*!
    \brief      disable TIMER output automatic function
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[out] none
    \retval     none
*/
void timer_automatic_output_disable(uint32_t timer_periph)
{
    TIMER_CCHP(timer_periph) &= ~(uint32_t)TIMER_CCHP_OAEN;
}

/*!
    \brief      定时器所有的通道输出使能
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  newvalue: ENABLE or DISABLE
    \param[out] none
    \retval     none
*/
void timer_primary_output_config(uint32_t timer_periph, ControlStatus newvalue)
{
    if(ENABLE == newvalue){
        TIMER_CCHP(timer_periph) |= (uint32_t)TIMER_CCHP_POEN;
    }else{
        TIMER_CCHP(timer_periph) &= (~(uint32_t)TIMER_CCHP_POEN);
    }
}

/*!
    \brief      enable or disable channel capture/compare control shadow register
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  newvalue: ENABLE or DISABLE 
    \param[out] none
    \retval     none
*/
void timer_channel_control_shadow_config(uint32_t timer_periph, ControlStatus newvalue)
{
     if(ENABLE == newvalue){
        TIMER_CTL1(timer_periph) |= (uint32_t)TIMER_CTL1_CCSE;
    }else{
        TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_CCSE);
    }
}

/*!
    \brief      configure TIMER channel control shadow register update control
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  ccuctl: channel control shadow register update control
                only one parameter can be selected which is shown as below:
      \arg        TIMER_UPDATECTL_CCU: the shadow registers update by when CMTG bit is set
      \arg        TIMER_UPDATECTL_CCUTRI: the shadow registers update by when CMTG bit is set or an rising edge of TRGI occurs 
    \param[out] none
    \retval     none
*/              
void timer_channel_control_shadow_update_config(uint32_t timer_periph, uint8_t ccuctl)
{
    if(TIMER_UPDATECTL_CCU == ccuctl){
        TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_CCUC);
    }else if(TIMER_UPDATECTL_CCUTRI == ccuctl){
        TIMER_CTL1(timer_periph) |= (uint32_t)TIMER_CTL1_CCUC;
    }else{
        /* illegal parameters */
    }
}

/*!
    @描述       将定时器通道输出参数结构体中所有参数初始化为默认值 
    @参数[输入]  ocpara: 输出通道结构体，详见结构体  timer_oc_parameter_struct
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_struct_para_init(timer_oc_parameter_struct* ocpara)
{
    /* 用默认值初始化通道输出参数结构体成员 */
    ocpara->outputstate  = (uint16_t)TIMER_CCX_DISABLE;
    ocpara->outputnstate = TIMER_CCXN_DISABLE;
    ocpara->ocpolarity   = TIMER_OC_POLARITY_HIGH;
    ocpara->ocnpolarity  = TIMER_OCN_POLARITY_HIGH;
    ocpara->ocidlestate  = TIMER_OC_IDLE_STATE_LOW;
    ocpara->ocnidlestate = TIMER_OCN_IDLE_STATE_LOW;
}

/*!
    @描述        定时器的通道输出配置
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel:  待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
    @参数[输入]  ocpara: 输出通道结构体，详见结构体 timer_oc_parameter_struct
      @arg        outputstate:  通道输出状态： TIMER_CCX_ENABLE,TIMER_CCX_DISABLE
      @arg        outputnstate: 互补通道输出状态(高级定时器)： TIMER_CCXN_ENABLE,TIMER_CCXN_DISABLE
      @arg        ocpolarity:   通道输出极性： TIMER_OC_POLARITY_HIGH,TIMER_OC_POLARITY_LOW
      @arg        ocnpolarity:  互补通道输出极性(高级定时器)： TIMER_OCN_POLARITY_HIGH,TIMER_OCN_POLARITY_LOW
      @arg        ocidlestate:  空闲状态下通道输出(高级定时器)： TIMER_OC_IDLE_STATE_LOW,TIMER_OC_IDLE_STATE_HIGH
      @arg        ocnidlestate: 空闲状态下互补通道输出(高级定时器)： TIMER_OCN_IDLE_STATE_LOW,TIMER_OCN_IDLE_STATE_HIGH
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_config(uint32_t timer_periph, uint16_t channel, timer_oc_parameter_struct* ocpara)
{
    switch(channel){
    /* 配置 TIMER_CH_0 */
    case TIMER_CH_0:
        /* 复位 CH0EN 位 */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);
        TIMER_CHCTL0(timer_periph) &= ~(uint32_t)TIMER_CHCTL0_CH0MS;
        /* 设置 CH0EN 位 */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)ocpara->outputstate;
        /* 复位 CH0P 位 */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0P);
        /* 设置 CH0P 位 */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)ocpara->ocpolarity;

        if((TIMER0 == timer_periph) || (TIMER7 == timer_periph)){
            /* 复位 CH0NEN 位 */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0NEN);
            /* 设置 CH0NEN 位 */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)ocpara->outputnstate;
            /* 复位 CH0NP 位 */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0NP);
            /* 设置 CH0NP 位 */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)ocpara->ocnpolarity;
            /* 复位 ISO0 位 */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO0);
            /* 设置 ISO0 位 */
            TIMER_CTL1(timer_periph) |= (uint32_t)ocpara->ocidlestate;
            /* 复位 ISO0N 位 */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO0N);
            /* 设置 ISO0N 位 */
            TIMER_CTL1(timer_periph) |= (uint32_t)ocpara->ocnidlestate;
        }
        break;
    /* 配置 TIMER_CH_1 */
    case TIMER_CH_1:
        /* reset the CH1EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);
        TIMER_CHCTL0(timer_periph) &= ~(uint32_t)TIMER_CHCTL0_CH1MS;
        /* set the CH1EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpara->outputstate << 4U);
        /* reset the CH1P bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1P);
        /* set the CH1P bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocpolarity) << 4U);

        if((TIMER0 == timer_periph) || (TIMER7 == timer_periph)){
            /* reset the CH1NEN bit */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1NEN);
            /* set the CH1NEN bit */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->outputnstate) << 4U);
            /* reset the CH1NP bit */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1NP);
            /* set the CH1NP bit */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocnpolarity) << 4U);
            /* reset the ISO1 bit */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO1);
            /* set the ISO1 bit */
            TIMER_CTL1(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocidlestate) << 2U);
            /* reset the ISO1N bit */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO1N);
            /* set the ISO1N bit */
            TIMER_CTL1(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocnidlestate) << 2U);
        }
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        /* reset the CH2EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2EN);
        TIMER_CHCTL1(timer_periph) &= ~(uint32_t)TIMER_CHCTL1_CH2MS;
        /* set the CH2EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpara->outputstate << 8U);
        /* reset the CH2P bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2P);
        /* set the CH2P bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocpolarity) << 8U);

        if((TIMER0 == timer_periph) || (TIMER7 == timer_periph)){
            /* reset the CH2NEN bit */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2NEN);
            /* set the CH2NEN bit */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->outputnstate) << 8U);
            /* reset the CH2NP bit */
            TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2NP);
            /* set the CH2NP bit */
            TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocnpolarity) << 8U);
            /* reset the ISO2 bit */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO2);
            /* set the ISO2 bit */
            TIMER_CTL1(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocidlestate) << 4U);
            /* reset the ISO2N bit */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO2N);
            /* set the ISO2N bit */
            TIMER_CTL1(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocnidlestate) << 4U);
        }
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        /* reset the CH3EN bit */
        TIMER_CHCTL2(timer_periph) &=(~(uint32_t)TIMER_CHCTL2_CH3EN);
        TIMER_CHCTL1(timer_periph) &= ~(uint32_t)TIMER_CHCTL1_CH3MS;
        /* set the CH3EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpara->outputstate << 12U);
        /* reset the CH3P bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH3P);
        /* set the CH3P bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocpolarity) << 12U);

        if((TIMER0 == timer_periph) || (TIMER7 == timer_periph)){
            /* reset the ISO3 bit */
            TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_ISO3);
            /* set the ISO3 bit */
            TIMER_CTL1(timer_periph) |= (uint32_t)((uint32_t)(ocpara->ocidlestate) << 6U);
        }
        break;
    default:
        break;
    }
}

/*!
    @描述       配置定时器通道输出比较模式
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0 (TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1 (TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2 (TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3 (TIMERx(x=0..4,7))
    @参数[输入]  ocmode: 通道输出比较模式
                只能选择一个参数，如下所示:
      @arg        TIMER_OC_MODE_TIMING: 冻结模式
      @arg        TIMER_OC_MODE_ACTIVE: 匹配时设置为高
      @arg        TIMER_OC_MODE_INACTIVE: 匹配时设置为低
      @arg        TIMER_OC_MODE_TOGGLE: 匹配时翻转
      @arg        TIMER_OC_MODE_LOW: 强制为低
      @arg        TIMER_OC_MODE_HIGH: 强制为高
      @arg        TIMER_OC_MODE_PWM0: PWM模式0
      @arg        TIMER_OC_MODE_PWM1: PWM模式1
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_mode_config(uint32_t timer_periph, uint16_t channel, uint16_t ocmode)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0COMCTL);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)ocmode;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1COMCTL);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(ocmode) << 8U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2COMCTL);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)ocmode;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3COMCTL);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(ocmode) << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       配置定时器的通道输出比较值
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel:  待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0 (TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1 (TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2 (TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3 (TIMERx(x=0..4,7))
    @参数[输入]  pulse: 通道输出比较值,0~65535
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_pulse_value_config(uint32_t timer_periph, uint16_t channel, uint32_t pulse)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CH0CV(timer_periph) = (uint32_t)pulse;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CH1CV(timer_periph) = (uint32_t)pulse;
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CH2CV(timer_periph) = (uint32_t)pulse;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
         TIMER_CH3CV(timer_periph) = (uint32_t)pulse;
        break;
    default:
        break;
    }
}

/*!
    @藐视       配置定时器通道输出比较影子寄存器功能
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel:待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
    @参数[输入]  ocshadow: 输出比较影子寄存器功能状态
                只能选择一个参数，如下所示:
      @arg        TIMER_OC_SHADOW_ENABLE: 使能输出比较影子寄存器
      @arg        TIMER_OC_SHADOW_DISABLE: 禁能输出比较影子寄存器
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_shadow_config(uint32_t timer_periph, uint16_t channel, uint16_t ocshadow)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0COMSEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)ocshadow;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1COMSEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(ocshadow) << 8U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2COMSEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)ocshadow;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3COMSEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(ocshadow) << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       配置定时器通道输出比较快速功能
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel:待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
    @参数[输入]  ocfast: 通道输出比较快速功能状态
                只能选择一个参数，如下所示:
      @arg        TIMER_OC_FAST_ENABLE: 通道输出比较快速功能使能
      @arg        TIMER_OC_FAST_DISABLE: 通道输出比较快速功能禁能
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_fast_config(uint32_t timer_periph, uint16_t channel, uint16_t ocfast)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0COMFEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)ocfast;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1COMFEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)ocfast << 8U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2COMFEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)ocfast;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3COMFEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)ocfast << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       配置定时器的通道输出比较清0功能
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0
      @arg        TIMER_CH_1: 通道 1
      @arg        TIMER_CH_2: 通道 2
      @arg        TIMER_CH_3: 通道 3
    @param[输入]  occlear: 通道比较输出清0功能状态
                只能选择一个参数，如下所示:
      @arg        TIMER_OC_CLEAR_ENABLE: 通道比较输出清0功能使能
      @arg        TIMER_OC_CLEAR_DISABLE: 通道比较输出清0功能禁能
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_clear_config(uint32_t timer_periph, uint16_t channel, uint16_t occlear)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0COMCEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)occlear;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1COMCEN);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)occlear << 8U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2COMCEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)occlear;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3COMCEN);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)occlear << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述      配置定时器通道输出极性
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0
      @arg        TIMER_CH_1: 通道 1
      @arg        TIMER_CH_2: 通道 2
      @arg        TIMER_CH_3: 通道 3
    @参数[输入]  ocpolarity: 通道输出极性
                只能选择一个参数，如下所示:
      @arg        TIMER_OC_POLARITY_HIGH: 通道输出极性高电平有效
      @arg        TIMER_OC_POLARITY_LOW: 通道输出极性低电平有效
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_polarity_config(uint32_t timer_periph, uint16_t channel, uint16_t ocpolarity)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0P);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)ocpolarity;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1P);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpolarity << 4U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2P);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpolarity << 8U);
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH3P);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocpolarity << 12U);
        break;
    default:
        break;
    }
}

/*!
    \brief      configure TIMER channel complementary output polarity 
    \param[in]  timer_periph: please refer to the following parameters
    \param[in]  channel:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_CH_0: TIMER channel0(TIMERx(x=0,7..13))
      \arg        TIMER_CH_1: TIMER channel1(TIMERx(x=0,7,8,11))
      \arg        TIMER_CH_2: TIMER channel2(TIMERx(x=0,7))
    \param[in]  ocnpolarity: channel complementary output polarity 
                only one parameter can be selected which is shown as below:
      \arg        TIMER_OCN_POLARITY_HIGH: channel complementary output polarity is high
      \arg        TIMER_OCN_POLARITY_LOW: channel complementary output polarity is low
    \param[out] none
    \retval     none
*/
void timer_channel_complementary_output_polarity_config(uint32_t timer_periph, uint16_t channel, uint16_t ocnpolarity)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0NP);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)ocnpolarity;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1NP);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocnpolarity << 4U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2NP);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocnpolarity << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       配置定时器通道状态
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0 (TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1 (TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2 (TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3 (TIMERx(x=0..4,7))
    @参数[输入]  state: 通道状态
                只能选择一个参数，如下所示:
      @arg        TIMER_CCX_ENABLE: 通道使能
      @arg        TIMER_CCX_DISABLE: 通道禁能
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_output_state_config(uint32_t timer_periph, uint16_t channel, uint32_t state)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)state;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)state << 4U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2EN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)state << 8U);
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH3EN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)state << 12U);
        break;
    default:
        break;
    }
}

/*!
    \brief      configure TIMER channel complementary output enable state
    \param[in]  timer_periph: please refer to the following parameters
    \param[in]  channel: 
                only one parameter can be selected which is shown as below:
      \arg        TIMER_CH_0: TIMER channel0(TIMERx(x=0,7))
      \arg        TIMER_CH_1: TIMER channel1(TIMERx(x=0,7))
      \arg        TIMER_CH_2: TIMER channel2(TIMERx(x=0,7))
    \param[in]  ocnstate: TIMER channel complementary output enable state
                only one parameter can be selected which is shown as below:
      \arg        TIMER_CCXN_ENABLE: channel complementary enable 
      \arg        TIMER_CCXN_DISABLE: channel complementary disable 
    \param[out] none
    \retval     none
*/
void timer_channel_complementary_output_state_config(uint32_t timer_periph, uint16_t channel, uint16_t ocnstate)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0NEN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)ocnstate;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1NEN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocnstate << 4U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2NEN);
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)ocnstate << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       将定时器通道输入参数结构体中所有参数初始化为默认值
    @参数[输入]  icpara: 通道输入结构体，详见 timer_ic_parameter_struct
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_input_struct_para_init(timer_ic_parameter_struct* icpara)
{
    /* 用默认值初始化通道输入参数结构体成员 */
    icpara->icpolarity  = TIMER_IC_POLARITY_RISING;
    icpara->icselection = TIMER_IC_SELECTION_DIRECTTI;
    icpara->icprescaler = TIMER_IC_PSC_DIV1;
    icpara->icfilter    = 0U;
}

/*!
    @描述       配置定时器输入捕获参数 
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
     @参数[输入]  icpara: 定时器通道输入参数结构，详见 timer_ic_parameter_struct
                 icpolarity: 通道输入极性： TIMER_IC_POLARITY_RISING,TIMER_IC_POLARITY_FALLING
                 icselection: 通道输入模式选择： TIMER_IC_SELECTION_DIRECTTI,TIMER_IC_SELECTION_INDIRECTTI,TIMER_IC_SELECTION_ITS
                 icprescaler: 通道输入捕获预分频： TIMER_IC_PSC_DIV1,TIMER_IC_PSC_DIV2,TIMER_IC_PSC_DIV4,TIMER_IC_PSC_DIV8
                 icfilter: 通道输入捕获滤波 0~15
    @参数[输出]  无
    @返回值      无
*/
void timer_input_capture_config(uint32_t timer_periph,uint16_t channel, timer_ic_parameter_struct* icpara)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        /* reset the CH0EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);

        /* reset the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH0P | TIMER_CHCTL2_CH0NP));
        TIMER_CHCTL2(timer_periph) |= (uint32_t)(icpara->icpolarity);
        /* reset the CH0MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0MS);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)(icpara->icselection);
        /* reset the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0CAPFLT);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpara->icfilter) << 4U);

        /* set the CH0EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH0EN;
        break;
    
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        /* reset the CH1EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);

        /* reset the CH1P and CH1NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH1P | TIMER_CHCTL2_CH1NP));
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(icpara->icpolarity) << 4U);
        /* reset the CH1MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1MS);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpara->icselection) << 8U);
        /* reset the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1CAPFLT);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpara->icfilter) << 12U);

        /* set the CH1EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH1EN;
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        /* reset the CH2EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH2EN);

        /* reset the CH2P and CH2NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH2P|TIMER_CHCTL2_CH2NP));
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(icpara->icpolarity) << 8U);

        /* reset the CH2MS bit */
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2MS);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(icpara->icselection));

        /* reset the CH2CAPFLT bit */
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2CAPFLT);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(icpara->icfilter) << 4U);

        /* set the CH2EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH2EN;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        /* reset the CH3EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH3EN);

        /* reset the CH3P bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH3P));
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(icpara->icpolarity) << 12U);

        /* reset the CH3MS bit */
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3MS);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(icpara->icselection) << 8U);

        /* reset the CH3CAPFLT bit */
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3CAPFLT);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)((uint32_t)(icpara->icfilter) << 12U);

        /* set the CH3EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH3EN;
        break;
    default:
        break;
    }
    /* configure TIMER channel input capture prescaler value */
    timer_channel_input_capture_prescaler_config(timer_periph, channel, (uint16_t)(icpara->icprescaler));
}

/*!
    @描述       配置定时器通道输入捕获预分频值
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
    @参数[输入]  prescaler: 通道输入捕获预分频器值
                只能选择一个参数，如下所示:
      @arg        TIMER_IC_PSC_DIV1: 不分频
      @arg        TIMER_IC_PSC_DIV2: 2分频
      @arg        TIMER_IC_PSC_DIV4: 4分频
      @arg        TIMER_IC_PSC_DIV8: 8分频
    @参数[输出] 无
    @返回值     无
*/
void timer_channel_input_capture_prescaler_config(uint32_t timer_periph, uint16_t channel, uint16_t prescaler)
{
    switch(channel){
    /* configure TIMER_CH_0 */
    case TIMER_CH_0:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0CAPPSC);
        TIMER_CHCTL0(timer_periph) |= (uint32_t)prescaler;
        break;
    /* configure TIMER_CH_1 */
    case TIMER_CH_1:
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1CAPPSC);
        TIMER_CHCTL0(timer_periph) |= ((uint32_t)prescaler << 8U);
        break;
    /* configure TIMER_CH_2 */
    case TIMER_CH_2:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH2CAPPSC);
        TIMER_CHCTL1(timer_periph) |= (uint32_t)prescaler;
        break;
    /* configure TIMER_CH_3 */
    case TIMER_CH_3:
        TIMER_CHCTL1(timer_periph) &= (~(uint32_t)TIMER_CHCTL1_CH3CAPPSC);
        TIMER_CHCTL1(timer_periph) |= ((uint32_t)prescaler << 8U);
        break;
    default:
        break;
    }
}

/*!
    @描述       读取通道捕获值
    @参数[输入]  timer_periph: 请参考以下参数
    @参数[输入]  channel:  待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0(TIMERx(x=0..4,7..13))
      @arg        TIMER_CH_1: 通道 1(TIMERx(x=0..4,7,8,11))
      @arg        TIMER_CH_2: 通道 2(TIMERx(x=0..4,7))
      @arg        TIMER_CH_3: 通道 3(TIMERx(x=0..4,7))
    @参数[输出] 无
    @返回值     通道捕获比较寄存器值
*/
uint32_t timer_channel_capture_value_register_read(uint32_t timer_periph, uint16_t channel)
{
    uint32_t count_value = 0U;

    switch(channel){
    /* read TIMER channel 0 capture compare register value */
    case TIMER_CH_0:
        count_value = TIMER_CH0CV(timer_periph);
        break;
    /* read TIMER channel 1 capture compare register value */
    case TIMER_CH_1:
        count_value = TIMER_CH1CV(timer_periph);
        break;
    /* read TIMER channel 2 capture compare register value */
    case TIMER_CH_2:
        count_value = TIMER_CH2CV(timer_periph);
        break;
    /* read TIMER channel 3 capture compare register value */
    case TIMER_CH_3:
        count_value = TIMER_CH3CV(timer_periph);
        break;
    default:
        break;
    }
    return (count_value);
}

/*!
    @描述       配置定时器捕获PWM输入参数
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输入]  channel: 待配置通道
                只能选择一个参数，如下所示:
      @arg        TIMER_CH_0: 通道 0
      @arg        TIMER_CH_1: 通道 1
     @参数[输入]  icpwm:定时器通道输入PWM参数结构， 详见 timer_ic_parameter_struct
                 icpolarity: TIMER_IC_POLARITY_RISING,TIMER_IC_POLARITY_FALLING
                 icselection: TIMER_IC_SELECTION_DIRECTTI,TIMER_IC_SELECTION_INDIRECTTI
                 icprescaler: TIMER_IC_PSC_DIV1,TIMER_IC_PSC_DIV2,TIMER_IC_PSC_DIV4,TIMER_IC_PSC_DIV8
                 icfilter: 0~15
    @参数[输出] 无
    @返回值     无
*/
void timer_input_pwm_capture_config(uint32_t timer_periph, uint16_t channel, timer_ic_parameter_struct* icpwm)
{
    uint16_t icpolarity  = 0x0U;
    uint16_t icselection = 0x0U;

    /* Set channel input polarity */
    if(TIMER_IC_POLARITY_RISING == icpwm->icpolarity){
        icpolarity = TIMER_IC_POLARITY_FALLING;
    }else{
        icpolarity = TIMER_IC_POLARITY_RISING;
    }

    /* Set channel input mode selection */
    if(TIMER_IC_SELECTION_DIRECTTI == icpwm->icselection){
        icselection = TIMER_IC_SELECTION_INDIRECTTI;
    }else{
        icselection = TIMER_IC_SELECTION_DIRECTTI;
    }

    if(TIMER_CH_0 == channel){
        /* reset the CH0EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);
        /* reset the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH0P|TIMER_CHCTL2_CH0NP));
        /* set the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)(icpwm->icpolarity);
        /* reset the CH0MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0MS);
        /* set the CH0MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)(icpwm->icselection);
        /* reset the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0CAPFLT);
        /* set the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= ((uint32_t)(icpwm->icfilter) << 4U);
        /* set the CH0EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH0EN;
        /* configure TIMER channel input capture prescaler value */
        timer_channel_input_capture_prescaler_config(timer_periph,TIMER_CH_0,(uint16_t)(icpwm->icprescaler));

        /* reset the CH1EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);
        /* reset the CH1P and CH1NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH1P|TIMER_CHCTL2_CH1NP));
        /* set the CH1P and CH1NP bits */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)icpolarity << 4U);
        /* reset the CH1MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1MS);
        /* set the CH1MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)icselection << 8U);
        /* reset the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1CAPFLT);
        /* set the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpwm->icfilter) << 12U);
        /* set the CH1EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH1EN;
        /* configure TIMER channel input capture prescaler value */
        timer_channel_input_capture_prescaler_config(timer_periph,TIMER_CH_1,(uint16_t)(icpwm->icprescaler));
    }else{
        /* reset the CH1EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);
        /* reset the CH1P and CH1NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH1P|TIMER_CHCTL2_CH1NP));
        /* set the CH1P and CH1NP bits */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)(icpwm->icpolarity) << 4U);
        /* reset the CH1MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1MS);
        /* set the CH1MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpwm->icselection) << 8U);
        /* reset the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1CAPFLT);
        /* set the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)(icpwm->icfilter) << 12U);
        /* set the CH1EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH1EN;
        /* configure TIMER channel input capture prescaler value */
        timer_channel_input_capture_prescaler_config(timer_periph, TIMER_CH_1, (uint16_t)(icpwm->icprescaler));

        /* reset the CH0EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);
        /* reset the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH0P|TIMER_CHCTL2_CH0NP));
        /* set the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)icpolarity;
        /* reset the CH0MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0MS);
        /* set the CH0MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)icselection;
        /* reset the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0CAPFLT);
        /* set the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= ((uint32_t)(icpwm->icfilter) << 4U);
        /* set the CH0EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH0EN;
        /* configure TIMER channel input capture prescaler value */
        timer_channel_input_capture_prescaler_config(timer_periph, TIMER_CH_0, (uint16_t)(icpwm->icprescaler));
    }
}

/*!
    @描述       配置定时器的HALL接口功能
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  hallmode: HALL接口功能状态 
                只能选择一个参数，如下所示:
      @arg        TIMER_HALLINTERFACE_ENABLE: HALL接口使能
      @arg        TIMER_HALLINTERFACE_DISABLE: HALL接口禁能
    @参数[输出] 无
    @返回值     无
*/
void timer_hall_mode_config(uint32_t timer_periph, uint32_t hallmode)
{
    if(TIMER_HALLINTERFACE_ENABLE == hallmode){
        TIMER_CTL1(timer_periph) |= (uint32_t)TIMER_CTL1_TI0S;
    }else if(TIMER_HALLINTERFACE_DISABLE == hallmode){
        TIMER_CTL1(timer_periph) &= ~(uint32_t)TIMER_CTL1_TI0S;
    }else{
        /* illegal parameters */
    }
}

/*!
    @描述       定时器的输入触发源选择 
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输入]  intrigger:待选择的触发源
                只能选择一个参数，如下所示:
      @arg        TIMER_SMCFG_TRGSEL_ITI0: 内部触发输入0(ITI0，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_ITI1: 内部触发输入1(ITI1，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_ITI2: 内部触发输入2(ITI2，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_ITI3: 内部触发输入3(ITI3，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_CI0F_ED: CI0的边沿标志位 (CI0F_ED，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_CI0FE0: 滤波后的通道0输入 (CI0FE0，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_CI1FE1: 滤波后的通道1输入(CI1FE1，TIMERx(x=0..4,7,8,11))
      @arg        TIMER_SMCFG_TRGSEL_ETIFP: 滤波后的外部触发输入(ETIFP，TIMERx(x=0..4,7))
    @参数[商城] 无
    @返回值     无
*/
void timer_input_trigger_source_select(uint32_t timer_periph, uint32_t intrigger)
{
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)TIMER_SMCFG_TRGS);
    TIMER_SMCFG(timer_periph) |= (uint32_t)intrigger;
}

/*!
    @描述       选择定时器主模式输出触发 
    @参数[输入]  timer_periph: TIMERx(x=0..7)
    @参数[输入]  outrigger:  主模式输出触发 
                只能选择一个参数，如下所示:
      @arg        TIMER_TRI_OUT_SRC_RESET: 复位
      @arg        TIMER_TRI_OUT_SRC_ENABLE: 使能
      @arg        TIMER_TRI_OUT_SRC_UPDATE: u更新
      @arg        TIMER_TRI_OUT_SRC_CH0: 通道0在发生一次捕获或一次比较
      @arg        TIMER_TRI_OUT_SRC_O0CPRE: 通道0比较
      @arg        TIMER_TRI_OUT_SRC_O1CPRE: 通道1比较
      @arg        TIMER_TRI_OUT_SRC_O2CPRE: 通道2比较
      @arg        TIMER_TRI_OUT_SRC_O3CPRE: 通道3比较
    @参数[输出] 无
    @返回值     无
*/
void timer_master_output_trigger_source_select(uint32_t timer_periph, uint32_t outrigger)
{
    TIMER_CTL1(timer_periph) &= (~(uint32_t)TIMER_CTL1_MMC);
    TIMER_CTL1(timer_periph) |= (uint32_t)outrigger;
}

/*!
    @描述       定时器从模式配置
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输入]  slavemode: 从模式
                只能选择一个参数，如下所示:
      @arg        TIMER_SLAVE_MODE_DISABLE: 从模式关闭
      @arg        TIMER_ENCODER_MODE0: 编码器模式0
      @arg        TIMER_ENCODER_MODE1: 编码器模式1
      @arg        TIMER_ENCODER_MODE2: 编码器模式2
      @arg        TIMER_SLAVE_MODE_RESTART: 复位模式
      @arg        TIMER_SLAVE_MODE_PAUSE: 暂停模式
      @arg        TIMER_SLAVE_MODE_EVENT: 事件模式
      @arg        TIMER_SLAVE_MODE_EXTERNAL0: 外部时钟模式0.
    @参数[输出] 无
    @返回值     无
*/

void timer_slave_mode_select(uint32_t timer_periph, uint32_t slavemode)
{
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)TIMER_SMCFG_SMC);

    TIMER_SMCFG(timer_periph) |= (uint32_t)slavemode;
}

/*!
    @描述       定时器主从模式配置
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输入]  masterslave: 主从模式使能状态
                只能选择一个参数，如下所示:
      @arg        TIMER_MASTER_SLAVE_MODE_ENABLE: 主从模式失能
      @arg        TIMER_MASTER_SLAVE_MODE_DISABLE: 主从模式禁能
    @参数[输出] 无
    @返回值     无
*/ 
void timer_master_slave_mode_config(uint32_t timer_periph, uint32_t masterslave)
{
    if(TIMER_MASTER_SLAVE_MODE_ENABLE == masterslave){
        TIMER_SMCFG(timer_periph) |= (uint32_t)TIMER_SMCFG_MSM;
    }else if(TIMER_MASTER_SLAVE_MODE_DISABLE == masterslave){
        TIMER_SMCFG(timer_periph) &= ~(uint32_t)TIMER_SMCFG_MSM;
    }else{
        /* illegal parameters */
    }
}

/*!
    @描述       配置定时器外部触发输入
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  extprescaler:外部触发预分频
                只能选择一个参数，如下所示:
      @arg        TIMER_EXT_TRI_PSC_OFF: 不分频
      @arg        TIMER_EXT_TRI_PSC_DIV2: 2分频
      @arg        TIMER_EXT_TRI_PSC_DIV4: 4分频
      @arg        TIMER_EXT_TRI_PSC_DIV8: 8分频
    @参数[输入]  extpolarity:外部触发输入极性
                只能选择一个参数，如下所示:
      @arg        TIMER_ETP_FALLING: 低电平或者下降沿有效
      @arg        TIMER_ETP_RISING: 高电平或者上升沿有效
    @参数[输入]  extfilter: 外部触发滤波控制（0~15）
    @参数[商城] 无
    @返回值     无
*/
void timer_external_trigger_config(uint32_t timer_periph, uint32_t extprescaler,
                                   uint32_t extpolarity, uint32_t extfilter)
{
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)(TIMER_SMCFG_ETP | TIMER_SMCFG_ETPSC | TIMER_SMCFG_ETFC));
    TIMER_SMCFG(timer_periph) |= (uint32_t)(extprescaler | extpolarity);
    TIMER_SMCFG(timer_periph) |= (uint32_t)(extfilter << 8U);
}

/*!
    \brief      configure TIMER quadrature decoder mode
    \param[in]  timer_periph: TIMERx(x=0..4,7)
    \param[in]  decomode: 
                only one parameter can be selected which is shown as below:
      \arg        TIMER_ENCODER_MODE0: counter counts on CI0FE0 edge depending on CI1FE1 level
      \arg        TIMER_ENCODER_MODE1: counter counts on CI1FE1 edge depending on CI0FE0 level
      \arg        TIMER_ENCODER_MODE2: counter counts on both CI0FE0 and CI1FE1 edges depending on the level of the other input
    \param[in]  ic0polarity: 
                only one parameter can be selected which is shown as below:
      \arg        TIMER_IC_POLARITY_RISING: capture rising edge
      \arg        TIMER_IC_POLARITY_FALLING: capture falling edge
    \param[in]  ic1polarity:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_IC_POLARITY_RISING: capture rising edge
      \arg        TIMER_IC_POLARITY_FALLING: capture falling edge
    \param[out] none
    \retval     none
*/
void timer_quadrature_decoder_mode_config(uint32_t timer_periph, uint32_t decomode,
                                   uint16_t ic0polarity, uint16_t ic1polarity)
{
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)TIMER_SMCFG_SMC);
    TIMER_SMCFG(timer_periph) |= (uint32_t)decomode;

    TIMER_CHCTL0(timer_periph) &= (uint32_t)(((~(uint32_t)TIMER_CHCTL0_CH0MS))&((~(uint32_t)TIMER_CHCTL0_CH1MS)));
    TIMER_CHCTL0(timer_periph) |= (uint32_t)(TIMER_IC_SELECTION_DIRECTTI|((uint32_t)TIMER_IC_SELECTION_DIRECTTI << 8U));

    TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH0P|TIMER_CHCTL2_CH0NP));
    TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH1P|TIMER_CHCTL2_CH1NP));
    TIMER_CHCTL2(timer_periph) |= ((uint32_t)ic0polarity|((uint32_t)ic1polarity << 4U));
}

/*!
    @描述       定时器配置为内部时钟模式
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输出] 无
    @返回值     无
*/
void timer_internal_clock_config(uint32_t timer_periph)
{
    TIMER_SMCFG(timer_periph) &= ~(uint32_t)TIMER_SMCFG_SMC;
}

/*!
    \brief      配置定时器内部触发器作为外部时钟输入
    \param[in]  timer_periph: TIMERx(x=0..4,7,8,11)
    \param[in]  intrigger:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_SMCFG_TRGSEL_ITI0: internal trigger 0
      \arg        TIMER_SMCFG_TRGSEL_ITI1: internal trigger 1
      \arg        TIMER_SMCFG_TRGSEL_ITI2: internal trigger 2
      \arg        TIMER_SMCFG_TRGSEL_ITI3: internal trigger 3
    \param[out] 无
    \retval     无
*/
void timer_internal_trigger_as_external_clock_config(uint32_t timer_periph, uint32_t intrigger)
{
    timer_input_trigger_source_select(timer_periph, intrigger);
    TIMER_SMCFG(timer_periph) &= ~(uint32_t)TIMER_SMCFG_SMC;
    TIMER_SMCFG(timer_periph) |= (uint32_t)TIMER_SLAVE_MODE_EXTERNAL0;
}

/*!
    \brief      configure TIMER the external trigger as external clock input
    \param[in]  timer_periph: TIMERx(x=0..4,7,8,11)
    \param[in]  extrigger:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_SMCFG_TRGSEL_CI0F_ED: TI0 edge detector
      \arg        TIMER_SMCFG_TRGSEL_CI0FE0: filtered TIMER input 0
      \arg        TIMER_SMCFG_TRGSEL_CI1FE1: filtered TIMER input 1
    \param[in]  extpolarity: 
                only one parameter can be selected which is shown as below:
      \arg        TIMER_IC_POLARITY_RISING: active high or rising edge active
      \arg        TIMER_IC_POLARITY_FALLING: active low or falling edge active
    \param[in]  extfilter: a value between 0 and 15
    \param[out] none
    \retval     none
*/
void timer_external_trigger_as_external_clock_config(uint32_t timer_periph, uint32_t extrigger,
                                       uint16_t extpolarity, uint32_t extfilter)
{
    if(TIMER_SMCFG_TRGSEL_CI1FE1 == extrigger){
        /* reset the CH1EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH1EN);
        /* reset the CH1NP bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH1P|TIMER_CHCTL2_CH1NP));
        /* set the CH1NP bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)((uint32_t)extpolarity << 4U);
        /* reset the CH1MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1MS);
        /* set the CH1MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)((uint32_t)TIMER_IC_SELECTION_DIRECTTI << 8U);
        /* reset the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH1CAPFLT);
        /* set the CH1CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)(extfilter << 12U);
        /* set the CH1EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH1EN;
    }else{
        /* reset the CH0EN bit */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)TIMER_CHCTL2_CH0EN);
        /* reset the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) &= (~(uint32_t)(TIMER_CHCTL2_CH0P|TIMER_CHCTL2_CH0NP));
        /* set the CH0P and CH0NP bits */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)extpolarity;
        /* reset the CH0MS bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0MS);
        /* set the CH0MS bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)TIMER_IC_SELECTION_DIRECTTI;
        /* reset the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) &= (~(uint32_t)TIMER_CHCTL0_CH0CAPFLT);
        /* reset the CH0CAPFLT bit */
        TIMER_CHCTL0(timer_periph) |= (uint32_t)(extfilter << 4U);
        /* set the CH0EN bit */
        TIMER_CHCTL2(timer_periph) |= (uint32_t)TIMER_CHCTL2_CH0EN;
    }
    /* select TIMER input trigger source */
    timer_input_trigger_source_select(timer_periph,extrigger);
    /* reset the SMC bit */
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)TIMER_SMCFG_SMC);
    /* set the SMC bit */
    TIMER_SMCFG(timer_periph) |= (uint32_t)TIMER_SLAVE_MODE_EXTERNAL0;
}

/*!
    @描述       配置定时器外部时钟模式0（ETI作为时钟源）
    @参数[输入]  timer_periph: TIMERx(x=0..4,7,8,11)
    @参数[输入]  extprescaler: ETI触发源预分频值
                只能选择一个参数，如下所示:
      @arg        TIMER_EXT_TRI_PSC_OFF:  不分频
      @arg        TIMER_EXT_TRI_PSC_DIV2: 2分频 
      @arg        TIMER_EXT_TRI_PSC_DIV4: 4分频
      @arg        TIMER_EXT_TRI_PSC_DIV8: 8分频
    @参数[输入]  extpolarity: ETI触发源极性
                只能选择一个参数，如下所示:
      @arg        TIMER_ETP_FALLING: 下降沿或者低电平有效
      @arg        TIMER_ETP_RISING: 上升沿或者高电平有效
    @参数[输入]  extfilter: ETI触发源滤波参数（0~15）
    @参数[输出] 无
    @返回值     无
*/
void timer_external_clock_mode0_config(uint32_t timer_periph, uint32_t extprescaler,
                                       uint32_t extpolarity, uint32_t extfilter)
{
    /* 配置定时器的外部触发输入 */
    timer_external_trigger_config(timer_periph, extprescaler, extpolarity, extfilter);

    /* 复位 SMC 位,TRGS 位 */
    TIMER_SMCFG(timer_periph) &= (~(uint32_t)(TIMER_SMCFG_SMC | TIMER_SMCFG_TRGS));
    /* 设置 SMC 位,TRGS 位 */
    TIMER_SMCFG(timer_periph) |= (uint32_t)(TIMER_SLAVE_MODE_EXTERNAL0 | TIMER_SMCFG_TRGSEL_ETIFP);
}

/*!
    @描述       配置定时器外部时钟模式1
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输入]  extprescaler: ETI触发源预分频值
                只能选择一个参数，如下所示:
      @arg        TIMER_EXT_TRI_PSC_OFF:  不分频
      @arg        TIMER_EXT_TRI_PSC_DIV2: 2分频 
      @arg        TIMER_EXT_TRI_PSC_DIV4: 4分频
      @arg        TIMER_EXT_TRI_PSC_DIV8: 8分频
    @参数[输入]  extpolarity: ETI触发源极性
                只能选择一个参数，如下所示:
      @arg        TIMER_ETP_FALLING: 下降沿或者低电平有效
      @arg        TIMER_ETP_RISING: 上升沿或者高电平有效
    @参数[输入]  extfilter: ETI触发源滤波参数（0~15）
    @参数[输出] 无
    @返回值     无
*/
void timer_external_clock_mode1_config(uint32_t timer_periph, uint32_t extprescaler,
                                       uint32_t extpolarity, uint32_t extfilter)
{
    /* configure TIMER external trigger input */
    timer_external_trigger_config(timer_periph, extprescaler, extpolarity, extfilter);

    TIMER_SMCFG(timer_periph) |= (uint32_t)TIMER_SMCFG_SMC1;
}

/*!
    @描述       定时器外部时钟模式1禁能
    @参数[输入]  timer_periph: TIMERx(x=0..4,7)
    @参数[输出] 无
    @返回值     无
*/
void timer_external_clock_mode1_disable(uint32_t timer_periph)
{
    TIMER_SMCFG(timer_periph) &= ~(uint32_t)TIMER_SMCFG_SMC1;
}

/*!
    \brief      configure TIMER write CHxVAL register selection
    \param[in]  timer_periph: TIMERx(x=0..4,7..13)
    \param[in]  ccsel:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_CHVSEL_DISABLE: no effect
      \arg        TIMER_CHVSEL_ENABLE: when write the CHxVAL register, if the write value is same as the CHxVAL value, the write access is ignored
    \param[out] none
    \retval     none
*/
void timer_write_chxval_register_config(uint32_t timer_periph, uint16_t ccsel)
{
    if(TIMER_CHVSEL_ENABLE == ccsel){
        TIMER_CFG(timer_periph) |= (uint32_t)TIMER_CFG_CHVSEL;
    }else if(TIMER_CHVSEL_DISABLE == ccsel){
        TIMER_CFG(timer_periph) &= ~(uint32_t)TIMER_CFG_CHVSEL;
    }else{
        /* illegal parameters */
    }
}

/*!
    \brief      configure TIMER output value selection
    \param[in]  timer_periph: TIMERx(x=0,7)
    \param[in]  outsel:
                only one parameter can be selected which is shown as below:
      \arg        TIMER_OUTSEL_DISABLE: no effect
      \arg        TIMER_OUTSEL_ENABLE: if POEN and IOS is 0, the output disabled
    \param[out] none
    \retval     none
*/
void timer_output_value_selection_config(uint32_t timer_periph, uint16_t outsel)
{
    if(TIMER_OUTSEL_ENABLE == outsel){
        TIMER_CFG(timer_periph) |= (uint32_t)TIMER_CFG_OUTSEL;
    }else if(TIMER_OUTSEL_DISABLE == outsel){
        TIMER_CFG(timer_periph) &= ~(uint32_t)TIMER_CFG_OUTSEL;
    }else{
        /* illegal parameters */
    }
}
