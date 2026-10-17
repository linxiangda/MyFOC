/*!
    \file    gd32f30x_misc.c
    \brief   MISC driver

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

#include "gd32f30x_misc.h"

/*!
    @简介      设置优先级组
    @参数[输入]  nvic_prigroup: NVIC优先组
      @rg        NVIC_PRIGROUP_PRE0_SUB4:0位用于抢占优先级，4位用于响应优先级
      @rg        NVIC_PRIGROUP_PRE1_SUB3:1位用于抢占优先级，3位用于响应优先级
      @rg        NVIC_PRIGROUP_PRE2_SUB2:2位用于抢占优先级，2位用于响应优先级
      @rg        NVIC_PRIGROUP_PRE3_SUB1:3位用于抢占优先级，1位用于响应优先级
      @rg        NVIC_PRIGROUP_PRE4_SUB0:4位用于抢占优先级，0位用于响应优先级
    @参数[输出] 无
    @返回值     无
*/
void nvic_priority_group_set(uint32_t nvic_prigroup)
{
    /* 配置优先级组值 */
    SCB->AIRCR = NVIC_AIRCR_VECTKEY_MASK | nvic_prigroup;
}

/*!
    @简介       启用NVIC请求
    @参数[输入]  nvic_irq: NVIC中断请求, 详细信息见 IRQn_Type
    @参数[输入]  nvic_irq_pre_priority: 需要设置的抢占优先级
    @参数[输入]  nvic_irq_sub_priority: 需要设置的子优先级
    @参数[输出] 无
    @返回值     无
*/
void nvic_irq_enable(uint8_t nvic_irq, uint8_t nvic_irq_pre_priority, 
                     uint8_t nvic_irq_sub_priority)
{
    uint32_t temp_priority = 0x00U, temp_pre = 0x00U, temp_sub = 0x00U;
    /* 使用优先级组值获取temp_pre和temp_sub */
    if(((SCB->AIRCR) & (uint32_t)0x700U)==NVIC_PRIGROUP_PRE0_SUB4){
        temp_pre=0U;
        temp_sub=0x4U;
    }else if(((SCB->AIRCR) & (uint32_t)0x700U)==NVIC_PRIGROUP_PRE1_SUB3){
        temp_pre=1U;
        temp_sub=0x3U;
    }else if(((SCB->AIRCR) & (uint32_t)0x700U)==NVIC_PRIGROUP_PRE2_SUB2){
        temp_pre=2U;
        temp_sub=0x2U;
    }else if(((SCB->AIRCR) & (uint32_t)0x700U)==NVIC_PRIGROUP_PRE3_SUB1){
        temp_pre=3U;
        temp_sub=0x1U;
    }else if(((SCB->AIRCR) & (uint32_t)0x700U)==NVIC_PRIGROUP_PRE4_SUB0){
        temp_pre=4U;
        temp_sub=0x0U;
    }else{
        nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);
        temp_pre=2U;
        temp_sub=0x2U;
    }
    /* 获取temp_priority来填充NVIC->IP寄存器 */
    temp_priority = (uint32_t)nvic_irq_pre_priority << (0x4U - temp_pre);
    temp_priority |= nvic_irq_sub_priority &(0x0FU >> (0x4U - temp_sub));
    temp_priority = temp_priority << 0x04U;
    NVIC->IP[nvic_irq] = (uint8_t)temp_priority;
    /* 启用所选的IRQ */
    NVIC->ISER[nvic_irq >> 0x05U] = (uint32_t)0x01U << (nvic_irq & (uint8_t)0x1FU);
}

/*!
    @简介       禁用NVIC请求
    @参数[输入]  nvic_irq: NVIC中断请求, 详细信息见
    @参数[输出] 无
    @返回值     无
*/
void nvic_irq_disable(uint8_t nvic_irq)
{
    /* 禁用所选IRQ */
    NVIC->ICER[nvic_irq >> 0x05] = (uint32_t)0x01 << (nvic_irq & (uint8_t)0x1F);
}

/*!
    \brief      set the NVIC vector table base address
    \param[in]  nvic_vict_tab: the RAM or FLASH base address
      \arg        NVIC_VECTTAB_RAM: RAM base address
      \are        NVIC_VECTTAB_FLASH: Flash base address
    \param[in]  offset: Vector Table offset
    \param[out] none
    \retval     none
*/
void nvic_vector_table_set(uint32_t nvic_vict_tab, uint32_t offset)
{
    SCB->VTOR = nvic_vict_tab | (offset & NVIC_VECTTAB_OFFSET_MASK);
    __DSB();
}

/*!
    \brief      set the state of the low power mode
    \param[in]  lowpower_mode: the low power mode state
      \arg        SCB_LPM_SLEEP_EXIT_ISR: if chose this para, the system always enter low power 
                    mode by exiting from ISR
      \arg        SCB_LPM_DEEPSLEEP: if chose this para, the system will enter the DEEPSLEEP mode
      \arg        SCB_LPM_WAKE_BY_ALL_INT: if chose this para, the lowpower mode can be woke up 
                    by all the enable and disable interrupts
    \param[out] none
    \retval     none
*/
void system_lowpower_set(uint8_t lowpower_mode)
{
    SCB->SCR |= (uint32_t)lowpower_mode;
}

/*!
    \brief      reset the state of the low power mode
    \param[in]  lowpower_mode: the low power mode state
      \arg        SCB_LPM_SLEEP_EXIT_ISR: if chose this para, the system will exit low power 
                    mode by exiting from ISR
      \arg        SCB_LPM_DEEPSLEEP: if chose this para, the system will enter the SLEEP mode
      \arg        SCB_LPM_WAKE_BY_ALL_INT: if chose this para, the lowpower mode only can be 
                    woke up by the enable interrupts
    \param[out] none
    \retval     none
*/
void system_lowpower_reset(uint8_t lowpower_mode)
{
    SCB->SCR &= (~(uint32_t)lowpower_mode);
}

/*!
    \brief      set the systick clock source
    \param[in]  systick_clksource: the systick clock source needed to choose
      \arg        SYSTICK_CLKSOURCE_HCLK: systick clock source is from HCLK
      \arg        SYSTICK_CLKSOURCE_HCLK_DIV8: systick clock source is from HCLK/8
    \param[out] none
    \retval     none
*/

void systick_clksource_set(uint32_t systick_clksource)
{
    if(SYSTICK_CLKSOURCE_HCLK == systick_clksource ){
        /* set the systick clock source from HCLK */
        SysTick->CTRL |= SYSTICK_CLKSOURCE_HCLK;
    }else{
        /* set the systick clock source from HCLK/8 */
        SysTick->CTRL &= SYSTICK_CLKSOURCE_HCLK_DIV8;
    }
}
