#include "gd32f30x.h"
#include "systick.h"

volatile static uint32_t delay;

void delay_400_ns(void)//非常粗略 
{
    uint16_t i;
    for(i=0; i<15; i++);
}

void rcu_config(void)
{
	rcu_periph_clock_enable(RCU_GPIOA);     // 开启GPIOA时钟
	rcu_periph_clock_enable(RCU_GPIOB);     // 开启GPIOB时钟
	rcu_periph_clock_enable(RCU_GPIOC);     // 开启GPIOC时钟
	
	rcu_periph_clock_enable(RCU_TIMER0);    // 开启TIMER0时钟 pwm（APB1，定时器时钟=120MHz）
	rcu_periph_clock_enable(RCU_TIMER3);    // 开启TIMER3时钟 rgb（APB2，定时器时钟=120MHz）
	rcu_periph_clock_enable(RCU_TIMER5);    // 定时器中断
	
	rcu_periph_clock_enable(RCU_SPI0);
	rcu_periph_clock_enable(RCU_SPI1);
//	rcu_periph_clock_enable(RCU_SPI2);
	
	rcu_periph_clock_enable(RCU_ADC0);									//开启ADC0时钟

	rcu_periph_clock_enable(RCU_USART2);    //开启USART2时钟
	rcu_periph_clock_enable(RCU_DMA0);      // 开启DMA0时钟
	rcu_periph_clock_enable(RCU_AF);        // 开启复用功能时钟	
}
void systick_config(void)
{
    /* setup systick timer for 1000Hz interrupts */
    if (SysTick_Config(SystemCoreClock / 1000000U)){
        /* capture error */
        while (1){
        }
    }
    /* configure the systick handler priority */
//    NVIC_SetPriority(SysTick_IRQn, 0x00U);
	NVIC_SetPriority(SysTick_IRQn, 0x0FU);
}


void delay_1ms(uint32_t count)
{
    delay = count*1000;

    while(0U != delay){
    }
}

void delay_1us(uint32_t count)
{
    delay = count;

    while(0U != delay){
    }
}


void delay_decrement(void)
{
    if (0U != delay){
        delay--;
    }
}



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

////-------------------------------------------------------------------------------------------------------------------
//// @brief        微秒级延时
//// @param        count            延时时间
//// @return       void
//// Sample usage:        delay_1us(50);
////                      //延时50微秒
////-------------------------------------------------------------------------------------------------------------------
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

////-------------------------------------------------------------------------------------------------------------------
//// @brief        毫秒级延时
//// @param        count            延时时间
//// @return       void
//// Sample usage:        delay_1ms(50);
////                      //延时50毫秒
////-------------------------------------------------------------------------------------------------------------------
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

