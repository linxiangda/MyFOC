#include "gd32f30x.h"                   // Device header


//volatile static float count_1us = 0;
//volatile static float count_1ms = 0;

////-------------------------------------------------------------------------------------------------------------------
//// @brief        配置时钟
//// @param        void
//// @return       void
////-------------------------------------------------------------------------------------------------------------------
//void systick_config(void)
//{
//    /* systick时钟源配置为系统时钟的8分频 */
//    systick_clksource_set(SYSTICK_CLKSOURCE_HCLK_DIV8);
//    count_1us = (float)SystemCoreClock/8000000;
//    count_1ms = (float)count_1us * 1000;
//}

//void delay_1us(uint32_t count)
//{
//    uint32_t ctl;
//    
//    /* 设置重装载值 */
//    SysTick->LOAD = (uint32_t)(count * count_1us);
//	
//    /* 清除当前计数值 */
//    SysTick->VAL = 0x0000U;
//	
//    /* 使能systick timer */
//    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
//	
//    /* 等待 COUNTFLAG flag 置位 */
//    do
//    {
//        ctl = SysTick->CTRL;
//    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
//		
//    /* 禁用systick timer */
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//		
//    /* 清除当前计数值 */
//    SysTick->VAL = 0x0000U;
//}
//void delay_1ms(uint32_t count)
//{
//    uint32_t ctl;
//    
//    /* 设置重装载值 */
//    SysTick->LOAD = (uint32_t)(count * count_1ms);
//	
//    /* 清除当前计数值 */
//    SysTick->VAL = 0x0000U;
//	
//    /* enable the systick timer */
//    SysTick->CTRL = SysTick_CTRL_ENABLE_Msk;
//	
//    /* 等待 COUNTFLAG flag 置位 */
//    do
//    {
//        ctl = SysTick->CTRL;
//    }while((ctl&SysTick_CTRL_ENABLE_Msk)&&!(ctl & SysTick_CTRL_COUNTFLAG_Msk));
//		
//    /* 禁用 systick timer */
//    SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
//		
//    /* 清除当前计数值 */
//    SysTick->VAL = 0x0000U;
//		
//}









