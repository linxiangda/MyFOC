/*!
    \file    gd32f30x_adc.c
    \brief   ADC driver

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

#include "gd32f30x_adc.h"

/*!
    @描述       复位ADC
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输出] 无
    @返回值     无
*/
void adc_deinit(uint32_t adc_periph)
{
    switch(adc_periph){
    case ADC0:
        rcu_periph_reset_enable(RCU_ADC0RST);
        rcu_periph_reset_disable(RCU_ADC0RST);
        break;
    case ADC1:
        rcu_periph_reset_enable(RCU_ADC1RST);
        rcu_periph_reset_disable(RCU_ADC1RST);
        break;
#if (defined(GD32F30X_HD) || defined(GD32F30X_XD))    
    case ADC2:
        rcu_periph_reset_enable(RCU_ADC2RST);
        rcu_periph_reset_disable(RCU_ADC2RST);
        break;
#endif    
    default:
        break;      
    }
}

/*!
    @描述       使能ADC
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输出] 无
    @返回值     无
*/
void adc_enable(uint32_t adc_periph)
{
    if(RESET == (ADC_CTL1(adc_periph) & ADC_CTL1_ADCON)){
        ADC_CTL1(adc_periph) |= (uint32_t)ADC_CTL1_ADCON;
    }       
}

/*!
    @描述       失能ADC
    @参数[输入]  adc_periph: ADCx,x=0,1,2
               这些参数中只能选择一个
    @参数[输出] 无
    @返回值     无
*/
void adc_disable(uint32_t adc_periph)
{
    ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ADCON);
}

/*!
    @描述       ADC校准复位
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    \参数[输出] 无
    \返回值     无
*/
void adc_calibration_enable(uint32_t adc_periph)
{
    /* 重置所选ADC校准寄存器 */
    ADC_CTL1(adc_periph) |= (uint32_t) ADC_CTL1_RSTCLB;
    /* 检查RSTCLB位状态 */
    while((ADC_CTL1(adc_periph) & ADC_CTL1_RSTCLB)){
    }
    /* 开启ADC校准 */
    ADC_CTL1(adc_periph) |= ADC_CTL1_CLB;
    /* 检查CLB位状态 */
    while((ADC_CTL1(adc_periph) & ADC_CTL1_CLB)){
    }
}

/*!
    @描述       DMA请求使能
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输出] 无
    @返回值     无
*/
void adc_dma_mode_enable(uint32_t adc_periph)
{
    ADC_CTL1(adc_periph) |= (uint32_t)(ADC_CTL1_DMA);
}

/*!
    @描述       DMA请求失能
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输出] 无
    @返回值     无
*/
void adc_dma_mode_disable(uint32_t adc_periph)
{
    ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_DMA);
}

/*!
    @描述       温度传感器和Vrefint通道使能
    @参数[输入] 无
    @参数[输出] 无
    @返回值     无
*/
void adc_tempsensor_vrefint_enable(void)
{
    /* 使能温度传感器和Vrefint通道 */
    ADC_CTL1(ADC0) |= ADC_CTL1_TSVREN;
}

/*!
    @描述       温度传感器和Vrefint通道失能
    @参数[输入] 无
    @参数[输出] 无
    @返回值     无
*/
void adc_tempsensor_vrefint_disable(void)
{
    /* 失能温度传感器和Vrefint通道 */
    ADC_CTL1(ADC0) &= ~ADC_CTL1_TSVREN;
}

/*!
    @描述       配置ADC分辨率 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  resolution: ADC分辨率
                这些参数中只能选择一个
      @arg        ADC_RESOLUTION_12B: 12位ADC分辨率
      @arg        ADC_RESOLUTION_10B: 10位ADC分辨率
      @arg        ADC_RESOLUTION_8B: 8位ADC分辨率
      @arg        ADC_RESOLUTION_6B: 6位ADC分辨率
    @参数[输出] 无
    @返回值     无
*/
void adc_resolution_config(uint32_t adc_periph , uint32_t resolution)
{
    ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_DRES);
    ADC_OVSAMPCTL(adc_periph) |= (uint32_t)resolution;
}

/*!
    @描述       配置ADC间断模式
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_channel_group: 通道组选择
                这些参数中只能选择一个
      @arg        ADC_REGULAR_CHANNEL: 规则组
      @arg        ADC_INSERTED_CHANNEL: 注入组
      @arg        ADC_CHANNEL_DISCON_DISABLE: 规则通道组和注入通道组间断模式禁能
    @参数[输入]  length: 间断模式下的转换数目,
				  for regular channel,the number can be 1..8
                  for inserted channel,the number has no effect
    @参数[输出] 无
    @返回值     无
*/
void adc_discontinuous_mode_config(uint32_t adc_periph, uint8_t adc_channel_group, uint8_t length)
{
    ADC_CTL0(adc_periph) &= ~((uint32_t)( ADC_CTL0_DISRC | ADC_CTL0_DISIC ));
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        /* 配置间断模式下的转换次数  */
        ADC_CTL0(adc_periph) &= ~((uint32_t)ADC_CTL0_DISNUM);
        ADC_CTL0(adc_periph) |= CTL0_DISNUM(((uint32_t)length - 1U));
    
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_DISRC;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t)ADC_CTL0_DISIC;
        break;
    case ADC_CHANNEL_DISCON_DISABLE:
    default:
        break;
    }
}

/*!
    \brief      configure the ADC sync mode
    \param[in]  mode: ADC mode
                only one among these parameters can be selected
      \arg        ADC_MODE_FREE: all the ADCs work independently
      \arg        ADC_DAUL_REGULAL_PARALLEL_INSERTED_PARALLEL: ADC0 and ADC1 work in combined regular parallel + inserted parallel mode
      \arg        ADC_DAUL_REGULAL_PARALLEL_INSERTED_ROTATION: ADC0 and ADC1 work in combined regular parallel + trigger rotation mode
      \arg        ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_FAST: ADC0 and ADC1 work in combined inserted parallel + follow-up fast mode
      \arg        ADC_DAUL_INSERTED_PARALLEL_REGULAL_FOLLOWUP_SLOW: ADC0 and ADC1 work in combined inserted parallel + follow-up slow mode
      \arg        ADC_DAUL_INSERTED_PARALLEL: ADC0 and ADC1 work in inserted parallel mode only
      \arg        ADC_DAUL_REGULAL_PARALLEL: ADC0 and ADC1 work in regular parallel mode only
      \arg        ADC_DAUL_REGULAL_FOLLOWUP_FAST: ADC0 and ADC1 work in follow-up fast mode only
      \arg        ADC_DAUL_REGULAL_FOLLOWUP_SLOW: ADC0 and ADC1 work in follow-up slow mode only
      \arg        ADC_DAUL_INSERTED_TRRIGGER_ROTATION: ADC0 and ADC1 work in trigger rotation mode only
    \param[out] none
    \retval     none
*/
void adc_mode_config(uint32_t mode)
{
    ADC_CTL0(ADC0) &= ~(ADC_CTL0_SYNCM);
    ADC_CTL0(ADC0) |= mode;
}

/*!
    @描述       配置ADC特殊功能
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  function: 功能配置
                这些参数中只能选择一个
      @arg        ADC_SCAN_MODE: 扫描模式选择
      @arg        ADC_INSERTED_CHANNEL_AUTO: 注入组自动转换
      @arg        ADC_CONTINUOUS_MODE: 连续模式选择
    @参数[输入]  newvalue: 功能使能禁能 ENABLE or DISABLE
    @参数[输出] 无
    @返回值     无
*/
void adc_special_function_config(uint32_t adc_periph , uint32_t function , ControlStatus newvalue)
{
    if(newvalue){
        if(0U != (function & ADC_SCAN_MODE)){
            ADC_CTL0(adc_periph) |= ADC_SCAN_MODE;
        }
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)){
            ADC_CTL0(adc_periph) |= ADC_INSERTED_CHANNEL_AUTO;
        } 
        if(0U != (function & ADC_CONTINUOUS_MODE)){
            ADC_CTL1(adc_periph) |= ADC_CONTINUOUS_MODE;
        }        
    }else{
        if(0U != (function & ADC_SCAN_MODE)){
            ADC_CTL0(adc_periph) &= ~ADC_SCAN_MODE;
        }
        if(0U != (function & ADC_INSERTED_CHANNEL_AUTO)){
            ADC_CTL0(adc_periph) &= ~ADC_INSERTED_CHANNEL_AUTO;
        } 
        if(0U != (function & ADC_CONTINUOUS_MODE)){
            ADC_CTL1(adc_periph) &= ~ADC_CONTINUOUS_MODE;
        }       
    }
}

/*!
    @描述       配置ADC数据对齐方式
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  data_alignment: 数据对齐方式选择
                这些参数中只能选择一个
      @arg        ADC_DATAALIGN_RIGHT: LSB对齐(右对齐)
      @arg        ADC_DATAALIGN_LEFT: MSB对齐(左对齐)
    @参数[输出] 无
    @返回值     无
*/
void adc_data_alignment_config(uint32_t adc_periph , uint32_t data_alignment)
{
    if(ADC_DATAALIGN_RIGHT != data_alignment){
        ADC_CTL1(adc_periph) |= ADC_CTL1_DAL;
    }else{
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_DAL);
    }
}

/*!
    @描述       配置规则通道组或注入通道组的长度
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_channel_group: 通道组选择
                只能选择一个参数
      @arg        ADC_REGULAR_CHANNEL: 规则通道组
      @arg        ADC_INSERTED_CHANNEL: 注入通道组
    @参数[输入]  length: 通道长度
                        规则通道组长度为 1-16
                        注入通道组长度为 1-4
    @参数[输出] 无
    @返回值     无
*/
void adc_channel_length_config(uint32_t adc_periph, uint8_t adc_channel_group, uint32_t length)
{
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_RSQ0(adc_periph) &= ~((uint32_t)ADC_RSQ0_RL);
        ADC_RSQ0(adc_periph) |= RSQ0_RL((uint32_t)(length-1U));

        break;
    case ADC_INSERTED_CHANNEL:
        ADC_ISQ(adc_periph) &= ~((uint32_t)ADC_ISQ_IL);
        ADC_ISQ(adc_periph) |= ISQ_IL((uint32_t)(length-1U));
    
        break;
    default:
        break;
    }
}

/*!
    @描述       配置ADC规则通道组 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
               这些参数中只能选择一个
    @参数[输入]  rank: 规则组通道序列,取值范围 0 to 15
    @参数[输入]  adc_channel: ADC通道选择
                这些参数中只能选择一个
      @arg        ADC_CHANNEL_x(x=0..17)(x=16 和 x=17 只适用于ADC0)
    @参数[输入]  sample_time: 采样时间
                只能选择一个参数
      @arg        ADC_SAMPLETIME_1POINT5: 1.5 周期
      @arg        ADC_SAMPLETIME_7POINT5: 7.5 周期
      @arg        ADC_SAMPLETIME_13POINT5: 13.5 周期
      @arg        ADC_SAMPLETIME_28POINT5: 28.5 周期
      @arg        ADC_SAMPLETIME_41POINT5: 41.5 周期
      @arg        ADC_SAMPLETIME_55POINT5: 55.5 周期
      @arg        ADC_SAMPLETIME_71POINT5: 71.5 周期
      @arg        ADC_SAMPLETIME_239POINT5: 239.5 周期
    @参数[输出] 无
    @返回值     无
*/
void adc_regular_channel_config(uint32_t adc_periph , uint8_t rank , uint8_t adc_channel , uint32_t sample_time)
{
    uint32_t rsq,sampt;
    
    /* ADC常规序列配置 */
    if(rank < 6U){
        rsq = ADC_RSQ2(adc_periph);
        rsq &=  ~((uint32_t)(ADC_RSQX_RSQN << (5U*rank)));
        rsq |= ((uint32_t)adc_channel << (5U*rank));
        ADC_RSQ2(adc_periph) = rsq;
    }else if(rank < 12U){
        rsq = ADC_RSQ1(adc_periph);
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-6U))));
        rsq |= ((uint32_t)adc_channel << (5U*(rank-6U)));
        ADC_RSQ1(adc_periph) = rsq;
    }else if(rank < 16U){
        rsq = ADC_RSQ0(adc_periph);
        rsq &= ~((uint32_t)(ADC_RSQX_RSQN << (5U*(rank-12U))));
        rsq |= ((uint32_t)adc_channel << (5U*(rank-12U)));
        ADC_RSQ0(adc_periph) = rsq;
    }else{
    }
    
    /* ADC采样时间配置 */
    if(adc_channel < 10U){
        sampt = ADC_SAMPT1(adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*adc_channel)));
        sampt |= (uint32_t)(sample_time << (3U*adc_channel));
        ADC_SAMPT1(adc_periph) = sampt;
    }else if(adc_channel < 18U){
        sampt = ADC_SAMPT0(adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*(adc_channel-10U))));
        sampt |= (uint32_t)(sample_time << (3U*(adc_channel-10U)));
        ADC_SAMPT0(adc_periph) = sampt;
    }else{
    }
}

/*!
    @描述      配置ADC注入通道组 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
               这些参数中只能选择一个
    @参数[输入]  rank: 规则组通道序列,取值范围 0 to 3
    @参数[输入]  adc_channel: ADC通道选择
                这些参数中只能选择一个
      @arg        ADC_CHANNEL_x(x=0..17)(x=16 和 x=17 只适用于ADC0)
    @参数[输入]  sample_time: 采样时间
                只能选择一个参数
      @arg        ADC_SAMPLETIME_1POINT5: 1.5 周期
      @arg        ADC_SAMPLETIME_7POINT5: 7.5 周期
      @arg        ADC_SAMPLETIME_13POINT5: 13.5 周期
      @arg        ADC_SAMPLETIME_28POINT5: 28.5 周期
      @arg        ADC_SAMPLETIME_41POINT5: 41.5 周期
      @arg        ADC_SAMPLETIME_55POINT5: 55.5 周期
      @arg        ADC_SAMPLETIME_71POINT5: 71.5 周期
      @arg        ADC_SAMPLETIME_239POINT5: 239.5 周期
    @参数[输出] 无
    @返回值     无
*/
void adc_inserted_channel_config(uint32_t adc_periph , uint8_t rank , uint8_t adc_channel , uint32_t sample_time)
{
    uint8_t inserted_length;
    uint32_t isq,sampt;
    
    inserted_length = (uint8_t)GET_BITS(ADC_ISQ(adc_periph) , 20U , 21U);
    
    isq = ADC_ISQ(adc_periph);
    isq &= ~((uint32_t)(ADC_ISQ_ISQN << (5U * ((3 + rank) - inserted_length))));
    isq |= ((uint32_t)adc_channel << (5U * ((3 + rank) - inserted_length)));
    ADC_ISQ(adc_periph) = isq;

    /* ADC采样时间配置 */  
    if(adc_channel < 10U){
        sampt = ADC_SAMPT1(adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*adc_channel)));
        sampt |= (uint32_t) sample_time << (3U*adc_channel);
        ADC_SAMPT1(adc_periph) = sampt;
    }else if(adc_channel < 18U){
        sampt = ADC_SAMPT0(adc_periph);
        sampt &= ~((uint32_t)(ADC_SAMPTX_SPTN << (3U*(adc_channel-10U))));
        sampt |= ((uint32_t)sample_time << (3U*(adc_channel-10U)));
        ADC_SAMPT0(adc_periph) = sampt;
    }else{
    }
}

/*!
    @描述       配置ADC注入通道组数据偏移值
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  inserted_channel : 注入通道选择
                只能选择一个参数
      @arg        ADC_INSERTED_CHANNEL_0: 注入通道0
      @arg        ADC_INSERTED_CHANNEL_1: 注入通道1
      @arg        ADC_INSERTED_CHANNEL_2: 注入通道2
      @arg        ADC_INSERTED_CHANNEL_3: 注入通道3
    @参数[输入]  offset : 数据偏移值，取值范围为0~4095
    @参数[输出] 无
    @返回值     无
*/
void adc_inserted_channel_offset_config(uint32_t adc_periph , uint8_t inserted_channel , uint16_t offset)
{
    uint8_t inserted_length;
    uint32_t num = 0U;

    inserted_length = (uint8_t)GET_BITS(ADC_ISQ(adc_periph) , 20U , 21U);
    num = 3U - (inserted_length - inserted_channel);
    
    if(num <= 3U){
        /* 计算寄存器的偏移量 */
        num = num * 4U;
        /* 配置所选通道的偏移量 */
        REG32((adc_periph) + 0x14U + num) = IOFFX_IOFF((uint32_t)offset);
    }  
}

/*!
    @描述      配置ADC外部触发
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_channel_group: 通道组选择
                可以选择一个或多个参数
      @arg        ADC_REGULAR_CHANNEL: 规则组
      @arg        ADC_INSERTED_CHANNEL: 注入组
    @参数[输入]  newvalue: 通道使能禁能 ENABLE or DISABLE
    @参数[输出] 无
    @返回值     无
*/
void adc_external_trigger_config(uint32_t adc_periph, uint8_t adc_channel_group, ControlStatus newvalue)
{
    if(newvalue){
        if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
            ADC_CTL1(adc_periph) |= ADC_CTL1_ETERC;
        }
        if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
            ADC_CTL1(adc_periph) |= ADC_CTL1_ETEIC;
        }        
    }else{
        if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
            ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETERC;
        }
        if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
            ADC_CTL1(adc_periph) &= ~ADC_CTL1_ETEIC;
        }      
    }
}

/*!
    @描述      配置ADC外部触发源 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[in]  adc_channel_group: 通道组选择
                只能选择一个参数
      @arg        ADC_REGULAR_CHANNEL: 规则组通道
      @arg        ADC_INSERTED_CHANNEL: 注入组通道
    @参数[输入]  external_trigger_source: 规则通道组或注入通道组触发源
                只能选择一个参数
                规则组:
      @arg        ADC0_1_EXTTRIG_REGULAR_T0_CH0: TIMER0 CH0事件 
      @arg        ADC0_1_EXTTRIG_REGULAR_T0_CH1: TIMER0 CH1事件 
      @arg        ADC0_1_EXTTRIG_REGULAR_T0_CH2: TIMER0 CH2事件
      @arg        ADC0_1_EXTTRIG_REGULAR_T1_CH1: TIMER1 CH1事件
      @arg        ADC0_1_EXTTRIG_REGULAR_T2_TRGO: TIMER2 TRGO事件
      @arg        ADC0_1_EXTTRIG_REGULAR_T3_CH3: TIMER3 CH3事件
      @arg        ADC0_1_EXTTRIG_REGULAR_T7_TRGO: TIMER7 TRGO事件
      @arg        ADC0_1_EXTTRIG_REGULAR_EXTI_11 : 外部中断线11
      @arg        ADC2_EXTTRIG_REGULAR_T2_CH0: TIMER2 CC0事件
      @arg        ADC2_EXTTRIG_REGULAR_T1_CH2: TIMER1 CC2事件
      @arg        ADC2_EXTTRIG_REGULAR_T0_CH2: TIMER0 CC2事件 
      @arg        ADC2_EXTTRIG_REGULAR_T7_CH0: TIMER7 CC0事件 
      @arg        ADC2_EXTTRIG_REGULAR_T7_TRGO: TIMER 7 TRGO事件 
      @arg        ADC2_EXTTRIG_REGULAR_T4_CH0: TIMER4 CC0事件 
      @arg        ADC2_EXTTRIG_REGULAR_T4_CH2: TIMER4 CC2事件
      @arg        ADC0_1_2_EXTTRIG_REGULAR_NONE: 软件    
                注入组:
      @arg        ADC0_1_EXTTRIG_INSERTED_T0_TRGO: TIMER0 TRGO事件
      @arg        ADC0_1_EXTTRIG_INSERTED_T0_CH3: TIMER0 CC3事件 
      @arg        ADC0_1_EXTTRIG_INSERTED_T1_TRGO: TIMER1 TRGO事件 
      @arg        ADC0_1_EXTTRIG_INSERTED_T1_CH0: TIMER1 CC0事件 
      @arg        ADC0_1_EXTTRIG_INSERTED_T2_CH3: TIMER2 CC3事件 
      @arg        ADC0_1_EXTTRIG_INSERTED_T3_TRGO: TIMER3 TRGO事件 
      @arg        ADC0_1_EXTTRIG_INSERTED_EXTI_15: 外部中断线15
      @arg        ADC0_1_EXTTRIG_INSERTED_T7_CH3: TIMER7 CC3事件 
      @arg        ADC2_EXTTRIG_INSERTED_T0_TRGO: TIMER0 TRGO事件 
      @arg        ADC2_EXTTRIG_INSERTED_T0_CH3: TIMER0 CC3事件 
      @arg        ADC2_EXTTRIG_INSERTED_T3_CH2: TIMER3 CC2事件 
      @arg        ADC2_EXTTRIG_INSERTED_T7_CH1: TIMER7 CC1事件 
      @arg        ADC2_EXTTRIG_INSERTED_T7_CH3: TIMER7 CC3事件 
      @arg        ADC2_EXTTRIG_INSERTED_T4_TRGO: TIMER4 TRGO事件
      @arg        ADC2_EXTTRIG_INSERTED_T4_CH3: TIMER4 CC3事件
      @arg        ADC0_1_2_EXTTRIG_INSERTED_NONE: 软件触发 
    @参数[输出] 无
    @返回值     无
*/
void adc_external_trigger_source_config(uint32_t adc_periph, uint8_t adc_channel_group, uint32_t external_trigger_source)
{   
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ETSRC);
        ADC_CTL1(adc_periph) |= (uint32_t)external_trigger_source;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL1(adc_periph) &= ~((uint32_t)ADC_CTL1_ETSIC);
        ADC_CTL1(adc_periph) |= (uint32_t)external_trigger_source;
        break;
    default:
        break;
    }
}

/*!
    @描述      ADC软件触发使能
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[in]  adc_channel_group: 通道组选择
                可以选择一个或多个参数
      @arg        ADC_REGULAR_CHANNEL: 规则组
      @arg        ADC_INSERTED_CHANNEL: 注入组
    @参数[输出] 无
    @返回值     无
*/
void adc_software_trigger_enable(uint32_t adc_periph , uint8_t adc_channel_group)
{
    if(0U != (adc_channel_group & ADC_REGULAR_CHANNEL)){
        ADC_CTL1(adc_periph) |= ADC_CTL1_SWRCST;
    }
    if(0U != (adc_channel_group & ADC_INSERTED_CHANNEL)){
        ADC_CTL1(adc_periph) |= ADC_CTL1_SWICST;
    }
}

/*!
    @描述       读ADC规则组数据寄存器 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入] 无
    @参数[输出] 无
    @返回值     ADC转换值
*/
uint16_t adc_regular_data_read(uint32_t adc_periph)
{
    return (uint16_t)(ADC_RDATA(adc_periph));
}

/*!
    @描述      读ADC注入组数据寄存器
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  inserted_channel : 注入通道选择
                只能选择一个参数
      @arg        ADC_INSERTED_CHANNEL_0: 注入组通道0
      @arg        ADC_INSERTED_CHANNEL_1: 注入组通道1
      @arg        ADC_INSERTED_CHANNEL_2: 注入组通道2
      @arg        ADC_INSERTED_CHANNEL_3: 注入组通道3
    @参数[输出] 无
    @返回值     ADC转换值
*/
uint16_t adc_inserted_data_read(uint32_t adc_periph , uint8_t inserted_channel)
{
    uint32_t idata;
    /* 读取所选通道的数据 */
    switch(inserted_channel){
    case ADC_INSERTED_CHANNEL_0:
        idata = ADC_IDATA0(adc_periph);
        break;
    case ADC_INSERTED_CHANNEL_1:
        idata = ADC_IDATA1(adc_periph);
        break;
    case ADC_INSERTED_CHANNEL_2:
        idata = ADC_IDATA2(adc_periph);
        break;
    case ADC_INSERTED_CHANNEL_3:
        idata = ADC_IDATA3(adc_periph);
        break;
    default:
        idata = 0U;
        break;
    }
    return (uint16_t)idata;
}

/*!
    \brief      read the last ADC0 and ADC1 conversion result data in sync mode
    \param[in]  none
    \param[out] none
    \retval     the conversion value
*/
uint32_t adc_sync_mode_convert_value_read(void)
{
    /* return conversion value */
    return ADC_RDATA(ADC0);
}

/*!
    @描述       获取ADC标志位
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_flag: the adc flag bits
                只能选择一个参数
      @arg        ADC_FLAG_WDE: 模拟看门狗事件标志位
      @arg        ADC_FLAG_EOC: 转换结束标志位
      @arg        ADC_FLAG_EOIC: 注入通道组转换结束标志位
      @arg        ADC_FLAG_STIC: 注入通道组转换开始标志位
      @arg        ADC_FLAG_STRC: 规则通道组转换开始标志位
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus adc_flag_get(uint32_t adc_periph , uint32_t adc_flag)
{
    FlagStatus reval = RESET;
    if(ADC_STAT(adc_periph) & adc_flag){
        reval = SET;
    }
    return reval;
}

/*!
    @描述      清除ADC标志位
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_flag: the adc flag bits
                只能选择一个参数
      @arg        ADC_FLAG_WDE: 模拟看门狗事件标志位
      @arg        ADC_FLAG_EOC: 转换结束标志位
      @arg        ADC_FLAG_EOIC: 注入通道组转换结束标志位
      @arg        ADC_FLAG_STIC: 注入通道组转换开始标志位
      @arg        ADC_FLAG_STRC: 规则通道组转换开始标志位
    @参数[输出] 无
    @返回值     无
*/
void adc_flag_clear(uint32_t adc_periph , uint32_t adc_flag)
{
    ADC_STAT(adc_periph) &= ~((uint32_t)adc_flag);
}

/*!
    @描述      获取ADC中断标志位
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_interrupt: ADC中断标志位
                只能选择一个参数
      @arg        ADC_INT_FLAG_WDE: 模拟看门狗中断标志位
      @arg        ADC_INT_FLAG_EOC: 转换结束中断标志位
      @arg        ADC_INT_FLAG_EOIC: 注入通道组转换结束中断标志位
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus adc_interrupt_flag_get(uint32_t adc_periph , uint32_t adc_interrupt)
{
    FlagStatus interrupt_flag = RESET;
    uint32_t state;
    /* 检查中断位 */
    switch(adc_interrupt){
    case ADC_INT_FLAG_WDE:
        state = ADC_STAT(adc_periph) & ADC_STAT_WDE;
        if((ADC_CTL0(adc_periph) & ADC_CTL0_WDEIE) && state){
          interrupt_flag = SET;
        }
        break;
    case ADC_INT_FLAG_EOC:
        state = ADC_STAT(adc_periph) & ADC_STAT_EOC;
          if((ADC_CTL0(adc_periph) & ADC_CTL0_EOCIE) && state){
            interrupt_flag = SET;
          }
        break;
    case ADC_INT_FLAG_EOIC:
        state = ADC_STAT(adc_periph) & ADC_STAT_EOIC;
        if((ADC_CTL0(adc_periph) & ADC_CTL0_EOICIE) && state){
            interrupt_flag = SET;
        }
        break;
    default:
        break;
    }
    return interrupt_flag;
}

/*!
    @描述      清除ADC中断标志位
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_interrupt: ADC中断标志位
                只能选择一个参数
      @arg        ADC_INT_FLAG_WDE: 模拟看门狗中断标志位
      @arg        ADC_INT_FLAG_EOC: 转换结束中断标志位
      @arg        ADC_INT_FLAG_EOIC: 注入通道组转换结束中断标志位
    @参数[输出] 无
	@返回值     无
*/
void adc_interrupt_flag_clear(uint32_t adc_periph , uint32_t adc_interrupt)
{
    ADC_STAT(adc_periph) &= ~((uint32_t)adc_interrupt);
}

/*!
    @描述      ADC中断使能 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_interrupt: ADC中断
                可以选择一个或多个参数
      @arg        ADC_INT_WDE: 模拟看门狗中断标志位
      @arg        ADC_INT_EOC: 转换结束中断标志位
      @arg        ADC_INT_EOIC: 注入通道组转换结束中断标志位
    @参数[输出] 无
    @返回值     无
*/
void adc_interrupt_enable(uint32_t adc_periph , uint32_t adc_interrupt)
{
    if(0U != (adc_interrupt & ADC_INT_WDE)){  
        ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_WDEIE;
    }  

    if(0U != (adc_interrupt & ADC_INT_EOC)){      
        ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_EOCIE;
    }  

    if(0U != (adc_interrupt & ADC_INT_EOIC)){      
        ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_EOICIE;
    }
}

/*!
    @描述      ADC中断失能 
    @参数[输入]  adc_periph: ADCx,x=0,1,2
                这些参数中只能选择一个
    @参数[输入]  adc_interrupt: ADC中断
                可以选择一个或多个参数
      @arg        ADC_INT_WDE: 模拟看门狗中断标志位
      @arg        ADC_INT_EOC: 转换结束中断标志位
      @arg        ADC_INT_EOIC: 注入通道组转换结束中断标志位
    @参数[输出] 无
    @返回值     无
*/
void adc_interrupt_disable(uint32_t adc_periph, uint32_t adc_interrupt)
{  
    if(0U != (adc_interrupt & ADC_INT_WDE)){  
        ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_WDEIE;
    }  

    if(0U != (adc_interrupt & ADC_INT_EOC)){      
        ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_EOCIE;
    }  

    if(0U != (adc_interrupt & ADC_INT_EOIC)){      
        ADC_CTL0(adc_periph) &= ~(uint32_t) ADC_CTL0_EOICIE;
    }
}

/*!
    \brief      configure ADC analog watchdog single channel 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[in]  adc_channel: the selected ADC channel
                only one among these parameters can be selected
      \arg        ADC_CHANNEL_x: ADC Channelx(x=0..17)(x=16 and x=17 are only for ADC0)
    \param[out] none
    \retval     none
*/
void adc_watchdog_single_channel_enable(uint32_t adc_periph, uint8_t adc_channel)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);

    ADC_CTL0(adc_periph) |= (uint32_t)adc_channel;
    ADC_CTL0(adc_periph) |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
}

/*!
    \brief      configure ADC analog watchdog group channel 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[in]  adc_channel_group: the channel group use analog watchdog
                only one parameter can be selected 
      \arg        ADC_REGULAR_CHANNEL: regular channel group
      \arg        ADC_INSERTED_CHANNEL: inserted channel group
      \arg        ADC_REGULAR_INSERTED_CHANNEL: both regular and inserted group
    \param[out] none
    \retval     none
*/
void adc_watchdog_group_channel_enable(uint32_t adc_periph, uint8_t adc_channel_group)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC);
    /* select the group */
    switch(adc_channel_group){
    case ADC_REGULAR_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_RWDEN;
        break;
    case ADC_INSERTED_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t) ADC_CTL0_IWDEN;
        break;
    case ADC_REGULAR_INSERTED_CHANNEL:
        ADC_CTL0(adc_periph) |= (uint32_t)(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN);
        break;
    default:
        break;
    }
}

/*!
    \brief      disable ADC analog watchdog 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[out] none
    \retval     none
*/
void adc_watchdog_disable(uint32_t adc_periph)
{
    ADC_CTL0(adc_periph) &= (uint32_t)~(ADC_CTL0_RWDEN | ADC_CTL0_IWDEN | ADC_CTL0_WDSC | ADC_CTL0_WDCHSEL);
}

/*!
    \brief      configure ADC analog watchdog threshold 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[in]  low_threshold: analog watchdog low threshold,0..4095
    \param[in]  high_threshold: analog watchdog high threshold,0..4095
    \param[out] none
    \retval     none
*/
void adc_watchdog_threshold_config(uint32_t adc_periph , uint16_t low_threshold , uint16_t high_threshold)
{
    ADC_WDLT(adc_periph) = (uint32_t)WDLT_WDLT(low_threshold);
    ADC_WDHT(adc_periph) = (uint32_t)WDHT_WDHT(high_threshold);
}

/*!
    \brief      configure ADC oversample mode 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[in]  mode: ADC oversampling mode
                only oneparameter can be selected
      \arg        ADC_OVERSAMPLING_ALL_CONVERT: all oversampled conversions for a channel are done consecutively after a trigger
      \arg        ADC_OVERSAMPLING_ONE_CONVERT: each oversampled conversion for a channel needs a trigger
    \param[in]  shift: ADC oversampling shift
                only oneparameter can be selected
      \arg        ADC_OVERSAMPLING_SHIFT_NONE: no oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_1B: 1-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_2B: 2-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_3B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_4B: 3-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_5B: 5-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_6B: 6-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_7B: 7-bit oversampling shift
      \arg        ADC_OVERSAMPLING_SHIFT_8B: 8-bit oversampling shift
    \param[in]  ratio: ADC oversampling ratio
                only oneparameter can be selected
      \arg        ADC_OVERSAMPLING_RATIO_MUL2: oversampling ratio multiple 2
      \arg        ADC_OVERSAMPLING_RATIO_MUL4: oversampling ratio multiple 4
      \arg        ADC_OVERSAMPLING_RATIO_MUL8: oversampling ratio multiple 8
      \arg        ADC_OVERSAMPLING_RATIO_MUL16: oversampling ratio multiple 16
      \arg        ADC_OVERSAMPLING_RATIO_MUL32: oversampling ratio multiple 32
      \arg        ADC_OVERSAMPLING_RATIO_MUL64: oversampling ratio multiple 64
      \arg        ADC_OVERSAMPLING_RATIO_MUL128: oversampling ratio multiple 128
      \arg        ADC_OVERSAMPLING_RATIO_MUL256: oversampling ratio multiple 256
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_config(uint32_t adc_periph, uint32_t mode, uint16_t shift, uint8_t ratio)
{
    if(ADC_OVERSAMPLING_ONE_CONVERT == mode){
        ADC_OVSAMPCTL(adc_periph) |= (uint32_t)ADC_OVSAMPCTL_TOVS;
    }else{
        ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_TOVS);
    }
    /* config the shift and ratio */
    ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)(ADC_OVSAMPCTL_OVSR | ADC_OVSAMPCTL_OVSS));
    ADC_OVSAMPCTL(adc_periph) |= ((uint32_t)shift | (uint32_t)ratio);
}

/*!
    \brief      enable ADC oversample mode 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_enable(uint32_t adc_periph)
{
    ADC_OVSAMPCTL(adc_periph) |= ADC_OVSAMPCTL_OVSEN;
}

/*!
    \brief      disable ADC oversample mode 
    \param[in]  adc_periph: ADCx,x=0,1,2
                only one among these parameters can be selected
    \param[out] none
    \retval     none
*/
void adc_oversample_mode_disable(uint32_t adc_periph)
{
    ADC_OVSAMPCTL(adc_periph) &= ~((uint32_t)ADC_OVSAMPCTL_OVSEN);
}
