/*!
    \file    gd32f30x_dma.c
    \brief   DMA driver

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

#include "gd32f30x_dma.h"
#include <stdlib.h>

#define DMA_WRONG_HANDLE        while(1){}

/* check whether peripheral matches channels or not */
static ErrStatus dma_periph_and_channel_check(uint32_t dma_periph, dma_channel_enum channelx);

/*!
    @描述       复位DMA通道寄存器 
    @传输[输入]  dma_periph: DMA选择
      @arg        DMAx(x=0,1)
    @传输[输入]  channelx: DMA通道选择
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     无
*/
void dma_deinit(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    /* 失能DMA通道 */
    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
    /* 复位DMA通道寄存器 */
    DMA_CHCTL(dma_periph, channelx) = DMA_CHCTL_RESET_VALUE;
    DMA_CHCNT(dma_periph, channelx) = DMA_CHCNT_RESET_VALUE;
    DMA_CHPADDR(dma_periph, channelx) = DMA_CHPADDR_RESET_VALUE;
    DMA_CHMADDR(dma_periph, channelx) = DMA_CHMADDR_RESET_VALUE;
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(DMA_CHINTF_RESET_VALUE, channelx);
}

/*!
    @描述       将DMA结构体中所有参数初始化为默认值
    @参数[输入]  init_struct: 一个已经定义的 dma_parameter_struct 结构体变量地址
    @参数[输出] 无
    @返回值     无
*/
void dma_struct_para_init(dma_parameter_struct* init_struct)
{
    if(NULL == init_struct){
        DMA_WRONG_HANDLE
    }

    /* 用默认值设置DMA结构体 */
    init_struct->periph_addr  = 0U;
    init_struct->periph_width = 0U; 
    init_struct->periph_inc   = DMA_PERIPH_INCREASE_DISABLE;
    init_struct->memory_addr  = 0U;
    init_struct->memory_width = 0U;
    init_struct->memory_inc   = DMA_MEMORY_INCREASE_DISABLE;
    init_struct->number       = 0U;
    init_struct->direction    = DMA_PERIPHERAL_TO_MEMORY;
    init_struct->priority     = DMA_PRIORITY_LOW;
}

/*!
    @描述      初始化DMA通道
    @参数[输入]  dma_periph: DMA选择
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道选择
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  init_struct: 初始化结构体，详见 dma_parameter_struct
      @arg        periph_addr: 外设基地址
      @arg        periph_width: 外设数据传输宽度 
					DMA_PERIPHERAL_WIDTH_8BIT, DMA_PERIPHERAL_WIDTH_16BIT, DMA_PERIPHERAL_WIDTH_32BIT
      @arg        periph_inc: 外设地址生成算法模式
					DMA_PERIPH_INCREASE_ENABLE, DMA_PERIPH_INCREASE_DISABLE 
      @arg        memory_addr: 存储器基地址
      @arg        memory_width: 存储器数据传输宽度
					DMA_MEMORY_WIDTH_8BIT, DMA_MEMORY_WIDTH_16BIT, DMA_MEMORY_WIDTH_32BIT
      @arg        memory_inc: 存储器地址生成算法模式
					DMA_MEMORY_INCREASE_ENABLE, DMA_MEMORY_INCREASE_DISABLE
      @arg        direction: DMA通道数据传输方向
					DMA_PERIPHERAL_TO_MEMORY, DMA_MEMORY_TO_PERIPHERAL
      @arg        number: DMA通道数据传输数量
      @arg        priority: DMA通道传输软件优先级
					DMA_PRIORITY_LOW, DMA_PRIORITY_MEDIUM, DMA_PRIORITY_HIGH, DMA_PRIORITY_ULTRA_HIGH
    @参数[输出] 无
    @返回值     无
*/
void dma_init(uint32_t dma_periph, dma_channel_enum channelx, dma_parameter_struct* init_struct)
{
    uint32_t ctl;

    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    /* 配置外设基地址 */
    DMA_CHPADDR(dma_periph, channelx) = init_struct->periph_addr;

    /* 配置存储器基地址 */
    DMA_CHMADDR(dma_periph, channelx) = init_struct->memory_addr;
    
    /* 配置DMA通道数据传输数量 */
    DMA_CHCNT(dma_periph, channelx) = (init_struct->number & DMA_CHANNEL_CNT_MASK);

    /* 配置外设、存储器数据传输宽度和DMA通道传输软件优先级 */
    ctl = DMA_CHCTL(dma_periph, channelx);
    ctl &= ~(DMA_CHXCTL_PWIDTH | DMA_CHXCTL_MWIDTH | DMA_CHXCTL_PRIO);
    ctl |= (init_struct->periph_width | init_struct->memory_width | init_struct->priority);
    DMA_CHCTL(dma_periph, channelx) = ctl;

    /* 配置外设地址生成算法模式 */
    if(DMA_PERIPH_INCREASE_ENABLE == init_struct->periph_inc){
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
    }else{
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
    }

    /* 配置存储器地址生成算法模式 */
    if(DMA_MEMORY_INCREASE_ENABLE == init_struct->memory_inc){
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
    }else{
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
    }

    /* 配置DMA通道数据传输方向 */
    if(DMA_PERIPHERAL_TO_MEMORY == init_struct->direction){
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    }else{
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    }
}

/*!
    @描述       DMA循环模式使能
    @参数[输入]  dma_periph:DMA外设 
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
               只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无 
*/
void dma_circulation_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CMEN;
}

/*!
    @描述       DMA循环模式失能
    @参数[输入]  dma_periph:DMA外设 
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
               只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无 
*/
void dma_circulation_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CMEN;
}

/*!
    @描述      存储器到存储器传输模式使能
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
               只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无
*/
void dma_memory_to_memory_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_M2M;
}

/*!
    @描述      存储器到存储器传输模式失能
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
               只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无
*/
void dma_memory_to_memory_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_M2M;
}

/*!
    @描述       DMA通道使能 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无 
*/
void dma_channel_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_CHEN;
}

/*!
    @描述       DMA通道失能 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[生成] 无
    @返回值     无 
*/
void dma_channel_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_CHEN;
}

/*!
    @描述      配置DMA通道传输的外设基地址  
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  address: 外设基地址
    @参数[输出] 无
    @返回值     无
*/
void dma_periph_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHPADDR(dma_periph, channelx) = address;
}

/*!
    @描述       配置DMA通道传输的存储器基地址  
    @参数[输入] dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入] channelx:DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入] address: 外设基地址
    @参数[输出] 无
    @返回值     无
*/
void dma_memory_address_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t address)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHMADDR(dma_periph, channelx) = address;
}

/*!
    @描述       配置DMA通道数据传输数量
    @参数[输入] dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入] channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入] number: 数据传输数量(0-65536)
    @参数[输出] 无
    @返回值     无
*/
void dma_transfer_number_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t number)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCNT(dma_periph, channelx) = (number & DMA_CHANNEL_CNT_MASK);
}

/*!
    @描述       获取DMA通道还有多少数据要传输 
    @参数[输入]  dma_periph: DMA通道
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA外设 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     uint32_t: DMA剩余传输数据的数量
*/
uint32_t dma_transfer_number_get(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    return (uint32_t)DMA_CHCNT(dma_periph, channelx);
}

/*!
    @描述      配置DMA通道软件优先级
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  priority: 通道软件优先级
                只能选择一个参数，如下所示:
      @arg        DMA_PRIORITY_LOW: 低优先级
      @arg        DMA_PRIORITY_MEDIUM: 中优先级
      @arg        DMA_PRIORITY_HIGH: 高优先级
      @arg        DMA_PRIORITY_ULTRA_HIGH: 超高优先级
    @参数[输出] 无
    @返回值     无
*/
void dma_priority_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t priority)
{
    uint32_t ctl;

    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    /*获取DMA_CHxCTL寄存器 */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* 分配寄存器 */
    ctl &= ~DMA_CHXCTL_PRIO;
    ctl |= priority;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    @描述       配置DMA通道存储器数据传输宽度
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  mwidth: 存储器数据传输的宽度
                只能选择一个参数，如下所示:
      @arg        DMA_MEMORY_WIDTH_8BIT: 8位数据传输宽度
      @arg        DMA_MEMORY_WIDTH_16BIT: 16位数据传输宽度
      @arg        DMA_MEMORY_WIDTH_32BIT: 32位数据传输宽度
    @参数[输出] 无
    @返回值     无
*/
void dma_memory_width_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t mwidth)
{
    uint32_t ctl;

    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_MWIDTH;
    ctl |= mwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    @描述       配置DMA通道外设数据传输宽度 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: 	DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  pwidth: 外设数据传输的宽度
                只能选择一个参数，如下所示:
      @arg        DMA_PERIPHERAL_WIDTH_8BIT: 8位数据传输宽度
      @arg        DMA_PERIPHERAL_WIDTH_16BIT: 16位数据传输宽度
      @arg        DMA_PERIPHERAL_WIDTH_32BIT: 32位数据传输宽度
    @参数[输出] 无
    @返回值     无
*/
void dma_periph_width_config (uint32_t dma_periph, dma_channel_enum channelx, uint32_t pwidth)
{
    uint32_t ctl;

    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    /* acquire DMA_CHxCTL register */
    ctl = DMA_CHCTL(dma_periph, channelx);
    /* assign regiser */
    ctl &= ~DMA_CHXCTL_PWIDTH;
    ctl |= pwidth;
    DMA_CHCTL(dma_periph, channelx) = ctl;
}

/*!
    @描述      DMA通道传输的存储器地址生成算法增量模式使能 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     无
*/
void dma_memory_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_MNAGA;
}

/*!
    @描述      DMA通道传输的存储器地址生成算法增量模式失能 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
               只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     无
*/
void dma_memory_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_MNAGA;
}

/*!
    @描述       DMA通道传输的外设地址生成算法增量模式使能
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     无
*/
void dma_periph_increase_enable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_PNAGA;
}

/*!
    @描述       DMA通道传输的外设地址生成算法增量模式失能 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输出] 无
    @返回值     无
*/
void dma_periph_increase_disable(uint32_t dma_periph, dma_channel_enum channelx)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_PNAGA;
}

/*!
    @描述       配置DMA通道的数据传输方向  
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  direction: 数据传输方向
                只能选择一个参数，如下所示:
      @arg        DMA_PERIPHERAL_TO_MEMORY: 读取外设中数据，写入存储器
      @arg        DMA_MEMORY_TO_PERIPHERAL: 读取存储器中数据，写入外设
    @参数[输出] 无
    @返回值     无
*/
void dma_transfer_direction_config(uint32_t dma_periph, dma_channel_enum channelx, uint32_t direction)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    if(DMA_PERIPHERAL_TO_MEMORY == direction){
        DMA_CHCTL(dma_periph, channelx) &= ~DMA_CHXCTL_DIR;
    } else {
        DMA_CHCTL(dma_periph, channelx) |= DMA_CHXCTL_DIR;
    }
}

/*!
    @描述       获取DMA通道状态标志位 
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  flag: DMA标志
                只能选择一个参数，如下所示:
      @arg        DMA_FLAG_G: 通道全局标志
      @arg        DMA_FLAG_FTF: 通道传输完成标志
      @arg        DMA_FLAG_HTF: 通道半传输完成标志
      @arg        DMA_FLAG_ERR: 通道错误标志
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus dma_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    FlagStatus reval;

    if(RESET != (DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx))){
        reval = SET;
    }else{
        reval = RESET;
    }

    return reval;
}

/*!
    @模式       清除DMA通道状态标志位
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  flag: 标志位
                只能选择一个参数，如下所示:
      @arg        DMA_FLAG_G: 通道全局标志
      @arg        DMA_FLAG_FTF: 通道传输完成标志
      @arg        DMA_FLAG_HTF: 通道半传输完成标志
      @arg        DMA_FLAG_ERR: 通道错误标志
    @参数[输出] 无
    @返回值     无
*/
void dma_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*!
    @描述       获取DMA通道中断状态标志位
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  flag: 标志位
                只能选择一个参数，如下所示:
      @arg        DMA_INT_FLAG_FTF: 通道传输完成中断标志
      @arg        DMA_INT_FLAG_HTF: 通道半传输完成中断标志
      @arg        DMA_INT_FLAG_ERR: 通道错误中断标志
    @参数[输出] 无
    @返回值     FlagStatus: SET or RESET
*/
FlagStatus dma_interrupt_flag_get(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    uint32_t interrupt_enable = 0U, interrupt_flag = 0U;

    switch(flag){
        case DMA_INT_FLAG_FTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_FTFIE;
            break;
        case DMA_INT_FLAG_HTF:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_HTFIE;
            break;
        case DMA_INT_FLAG_ERR:
            interrupt_flag = DMA_INTF(dma_periph) & DMA_FLAG_ADD(flag, channelx);
            interrupt_enable = DMA_CHCTL(dma_periph, channelx) & DMA_CHXCTL_ERRIE;
            break;
        default:
            DMA_WRONG_HANDLE
        }

    if(interrupt_flag && interrupt_enable){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @描述       清除DMA通道中断状态标志位
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  flag: 标志位
                只能选择一个参数，如下所示:
      @arg        DMA_INT_FLAG_G: 通道全局中断标志
      @arg        DMA_INT_FLAG_FTF: 通道传输完成中断标志
      @arg        DMA_INT_FLAG_HTF: 通道半传输完成中断标志
      @arg        DMA_INT_FLAG_ERR: 通道错误中断标志
    @参数[输出] 无
    @返回值     无
*/
void dma_interrupt_flag_clear(uint32_t dma_periph, dma_channel_enum channelx, uint32_t flag)
{
    DMA_INTC(dma_periph) |= DMA_FLAG_ADD(flag, channelx);
}

/*!
    @描述       DMA通道中断使能
    @描述[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  source: DMA中断源
               可以选择一个或多个参数，如下所示
      @arg        DMA_INT_FTF: 通道传输完成中断
      @arg        DMA_INT_HTF: 通道半传输完成中断
      @arg        DMA_INT_ERR: 通道错误中断
    @参数[输出] 无
    @返回值     无
*/
void dma_interrupt_enable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }

    DMA_CHCTL(dma_periph, channelx) |= source;
}

/*!
    @描述      DMA通道中断失能
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA0: DMA_CHx(x=0..6), DMA1: DMA_CHx(x=0..4)
    @参数[输入]  source: DMA中断源
                可以选择一个或多个参数，如下所示
      @arg        DMA_INT_FTF: 通道传输完成中断
      @arg        DMA_INT_HTF: 通道半传输完成中断
      @arg        DMA_INT_ERR: 通道错误中断
    @参数[输出] 无
    @返回值     无
*/
void dma_interrupt_disable(uint32_t dma_periph, dma_channel_enum channelx, uint32_t source)
{
    if(ERROR == dma_periph_and_channel_check(dma_periph, channelx)){
        DMA_WRONG_HANDLE
    }
    
    DMA_CHCTL(dma_periph, channelx) &= ~source;
}

/*!
    @描述       检查外设和通道是否匹配
    @参数[输入]  dma_periph: DMA外设
      @arg        DMAx(x=0,1)
    @参数[输入]  channelx: DMA通道 
                只能选择一个参数，如下所示:
      @arg        DMA_CHx(x=0..6)
    @参数[输出] 无
    @返回值     无
*/
static ErrStatus dma_periph_and_channel_check(uint32_t dma_periph, dma_channel_enum channelx)
{
    ErrStatus val = SUCCESS;
    
    if(DMA1 == dma_periph){
        /* 对于DMA1, 通道从 DMA_CH0 到 DMA_CH4 */
        if(channelx > DMA_CH4){
            val = ERROR;
        }
    }

    return val;
}
