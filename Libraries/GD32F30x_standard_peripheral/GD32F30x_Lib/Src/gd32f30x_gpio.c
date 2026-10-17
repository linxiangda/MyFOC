/*!
    \file    gd32f30x_gpio.c
    \brief   GPIO driver

    \version 2017-02-10, V1.0.0, firmware for GD32F30x
    \version 2018-10-10, V1.1.0, firmware for GD32F30x
    \version 2018-12-25, V2.0.0, firmware for GD32F30x
    \version 2020-09-30, V2.1.0, firmware for GD32F30x
*/

/*
    版权所有:GigaDevice Semiconductor Inc.

    以源代码和二进制形式重新分发和使用，无论是否修改，在满足以下条件的情况下是允许的:

    1. 重新发布源代码必须保留上述版权声明条件列表和以下免责声明.
    2. 以二进制形式重新发布必须复制上述版权声明，此条件列表和文档中的以下免责声明和/或随分发提供的其他材料。
    3. 既没有版权所有者的名字，也没有贡献者的名字可用于认可或推广衍生自本软件的产品而无需具体的事先书面许可。


    本软件由版权所有者和贡献者“按原样”提供。
	以及任何明示或暗示的保证，包括但不限于，隐含的不保证适销性和适合某一特定目的。
	在任何情况下，版权持有人或贡献者都不对任何直接的、间接的、偶然的、特殊的、惩戒性的或后果性的损害(包括但不包括)
	不限于，替代商品或服务的采购;失去使用、数据或利润;或业务中断)，无论原因如何，根据任何责任理论，
	无论是合同、严格责任还是侵权行为(包括疏忽或其他)因使用本软件而产生的任何情况，即使被告知的可能性这种损害的。
*/

#include "gd32f30x_gpio.h"

#define AFIO_EXTI_SOURCE_MASK              ((uint8_t)0x03U)         /*!< AFIO exti source selection mask*/     
#define AFIO_EXTI_SOURCE_FIELDS            ((uint8_t)0x04U)         /*!< select AFIO exti source registers */
#define LSB_16BIT_MASK                     ((uint16_t)0xFFFFU)      /*!< LSB 16-bit mask */
#define PCF_POSITION_MASK                  ((uint32_t)0x000F0000U)  /*!< AFIO_PCF register position mask */
#define PCF_SWJCFG_MASK                    ((uint32_t)0xF8FFFFFFU)  /*!< AFIO_PCF register SWJCFG mask */
#define PCF_LOCATION1_MASK                 ((uint32_t)0x00200000U)  /*!< AFIO_PCF register location1 mask */
#define PCF_LOCATION2_MASK                 ((uint32_t)0x00100000U)  /*!< AFIO_PCF register location2 mask */
#define AFIO_PCF1_FIELDS                   ((uint32_t)0x80000000U)  /*!< select AFIO_PCF1 register */
#define GPIO_OUTPUT_PORT_OFFSET            ((uint32_t)4U)           /*!< GPIO event output port offset*/

/*!
    @简介        复位GPIO端口
    @参数[输入]  GPIO端口 gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    @参数[输出]  无
    @返回值      无
*/
void gpio_deinit(uint32_t gpio_periph)
{
    switch(gpio_periph){
    case GPIOA:
        /* 复位 GPIOA */
        rcu_periph_reset_enable(RCU_GPIOARST);
        rcu_periph_reset_disable(RCU_GPIOARST);
        break;
    case GPIOB:
        /* 复位 GPIOB */
        rcu_periph_reset_enable(RCU_GPIOBRST);
        rcu_periph_reset_disable(RCU_GPIOBRST);
        break;
    case GPIOC:
        /* 复位 GPIOC */
        rcu_periph_reset_enable(RCU_GPIOCRST);
        rcu_periph_reset_disable(RCU_GPIOCRST);
        break;
    case GPIOD:
        /* 复位 GPIOD */
        rcu_periph_reset_enable(RCU_GPIODRST);
        rcu_periph_reset_disable(RCU_GPIODRST);
        break;
    case GPIOE:
        /* 复位 GPIOE */
        rcu_periph_reset_enable(RCU_GPIOERST);
        rcu_periph_reset_disable(RCU_GPIOERST);
        break;
    case GPIOF:
        /* 复位 GPIOF */
        rcu_periph_reset_enable(RCU_GPIOFRST);
        rcu_periph_reset_disable(RCU_GPIOFRST);
        break;
    case GPIOG:
        /* 复位 GPIOG */
        rcu_periph_reset_enable(RCU_GPIOGRST);
        rcu_periph_reset_disable(RCU_GPIOGRST);
        break;
    default:
        break;
    }
}

/*!
    @简介       复位 I/O(AFIO)
    @参数[输入] 无
    @参数[输出] 无
    @返回值     无
*/
void gpio_afio_deinit(void)
{
    rcu_periph_reset_enable(RCU_AFRST);
    rcu_periph_reset_disable(RCU_AFRST);
}

/*!
    @简介      GPIO参数初始化
    @参数1[输入]  gpio_periph: GPIO端口 GPIOx(x = A,B,C,D,E,F,G) 
    @参数2[输入]  mode: GPIO引脚模式
                只能选择一个参数，如下所示:
      \arg        GPIO_MODE_AIN: 模拟输入模式 
      \arg        GPIO_MODE_IN_FLOATING: 浮空输入模式 
      \arg        GPIO_MODE_IPD: 下拉输入模式
      \arg        GPIO_MODE_IPU: 上拉输入模式
      \arg        GPIO_MODE_OUT_OD: 开漏输出模式 
      \arg        GPIO_MODE_OUT_PP: 推挽输出模式 
      \arg        GPIO_MODE_AF_OD: AFIO开漏输出模式
      \arg        GPIO_MODE_AF_PP: AFIO推挽输出模式 

    @参数3[输入]  speed: GPIO输出最大速度
                只能选择一个参数，如下所示:
      \arg        GPIO_OSPEED_10MHZ: 最大输出速度 10MHz
      \arg        GPIO_OSPEED_2MHZ: 最大输出速度 2MHz
      \arg        GPIO_OSPEED_50MHZ: 最大输出速度 50MHz
      \arg        GPIO_OSPEED_MAX: 最大输出速度高于 50MHz
    @参数4[输入]  pin: GPIO引脚
                  可以选择一个或多个参数，如下所示:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数[输出]   无
    @返回值       无       
*/
void gpio_init(uint32_t gpio_periph, uint32_t mode, uint32_t speed, uint32_t pin)
{
    uint16_t i;
    uint32_t temp_mode = 0U;
    uint32_t reg = 0U;

    /* GPIO模式配置 */
    temp_mode = (uint32_t)(mode & ((uint32_t)0x0FU));
    
    /* GPIO速度配置 */
    if(((uint32_t)0x00U) != ((uint32_t)mode & ((uint32_t)0x10U))){
        /* 最大输出速度 */
        if(GPIO_OSPEED_MAX == (uint32_t)speed){
            temp_mode |= (uint32_t)0x03U;
            /* 设置相应的SPD(端口速度寄存器)位 */
            GPIOx_SPD(gpio_periph) |= (uint32_t)pin ;
        }else{
            /* 最大输出速度:10MHz,2MHz,50MHz */
            temp_mode |= (uint32_t)speed;
        }
    }

    /* 用GPIO_CTL0(端口控制寄存器0)配置8个低端口引脚 */
    for(i = 0U;i < 8U;i++){
        if((1U << i) & pin){
            reg = GPIO_CTL0(gpio_periph);
            
            /* 清除指定引脚模式位 */
            reg &= ~GPIO_MODE_MASK(i);
            /* 设置指定引脚模式位 */
            reg |= GPIO_MODE_SET(i, temp_mode);
            
            /* 设置IPD(下拉输入模式)或IPU(上拉输入模式)  */
            if(GPIO_MODE_IPD == mode){
                /* 复位相应的OCTL(端口输出控制寄存器)位 */
                GPIO_BC(gpio_periph) = (uint32_t)((1U << i) & pin);
            }else{
                /* 设置相应的OCTL(端口输出控制寄存器)位 */
                if(GPIO_MODE_IPU == mode){
                    GPIO_BOP(gpio_periph) = (uint32_t)((1U << i) & pin);
                }
            }
            /* 设置GPIO_CTL0端口控制寄存器 */
            GPIO_CTL0(gpio_periph) = reg;
        }
    }
    /* 用GPIO_CTL1(端口控制寄存器1)配置8个高端口引脚 */
    for(i = 8U;i < 16U;i++){
        if((1U << i) & pin){
            reg = GPIO_CTL1(gpio_periph);
            
            /* 清除指定引脚模式位 */
            reg &= ~GPIO_MODE_MASK(i - 8U);
            /* 设置指定引脚模式位 */
            reg |= GPIO_MODE_SET(i - 8U, temp_mode);
            
            /* 设置IPD(下拉输入模式)或IPU(上拉输入模式) */
            if(GPIO_MODE_IPD == mode){
                /* 复位相应的OCTL(端口输出控制寄存器)位 */
                GPIO_BC(gpio_periph) = (uint32_t)((1U << i) & pin);
            }else{
                /* 设置相应的OCTL(端口输出控制寄存器)位 */
                if(GPIO_MODE_IPU == mode){
                    GPIO_BOP(gpio_periph) = (uint32_t)((1U << i) & pin);
                }
            }
            /* 设置GPIO_CTL1端口控制寄存器 */
            GPIO_CTL1(gpio_periph) = reg;
        }
    }
}

/*!
    @简介      	置位GPIO引脚值
    @参数1[输入]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G) 
    @参数2[输入]  pin: GPIO引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数[输出] 无
    @返回值     无
*/
void gpio_bit_set(uint32_t gpio_periph,uint32_t pin)
{
    GPIO_BOP(gpio_periph) = (uint32_t)pin;
}

/*!
    @简介      复位GPIO引脚值 
    @参数1[输入]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    @参数2[输入]  pin: GPIO引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数[输出] 无
    @返回值     无
*/
void gpio_bit_reset(uint32_t gpio_periph,uint32_t pin)
{
    GPIO_BC(gpio_periph) = (uint32_t)pin;
}

/*!
    @简介      将数据写入指定的GPIO引脚
    @参数1[输入]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    @参数2[输入]  pin: GPIO引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数3[输入]  bit_value: SET or RESET
      @arg        RESET: 清除端口引脚
      @arg        SET: 设置端口引脚
    @参数1[输出] 无
    @返回值      无
*/
void gpio_bit_write(uint32_t gpio_periph,uint32_t pin,bit_status bit_value)
{
    if(RESET != bit_value){
        GPIO_BOP(gpio_periph) = (uint32_t)pin;
    }else{
        GPIO_BC(gpio_periph) = (uint32_t)pin;
    }
}

/*!
    @简介      将数据写入指定的GPIO端口
    @参数1[输出]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G) 
    @参数2[输入]  data: 将要写入的具体值 
    @参数[输出] 无
    @返回值     无
*/
void gpio_port_write(uint32_t gpio_periph,uint16_t data)
{
    GPIO_OCTL(gpio_periph) = (uint32_t)data;
}

/*!
    @简介      获取GPIO引脚输入状态
    @参数[输入]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    @参数[输入]  pin: GPIO引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数[输出] 无
    @返回值     gpio引脚输入状态: SET or RESET
*/
FlagStatus gpio_input_bit_get(uint32_t gpio_periph,uint32_t pin)
{
    if((uint32_t)RESET != (GPIO_ISTAT(gpio_periph)&(pin))){
        return SET; 
    }else{
        return RESET;
    }
}

/*!
    @简介      获取GPIO端口输入状态
    @参数[in]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G) 
    @参数[out] 无
    @返回值    该gpio所有引脚的输入状态 0x0000-0xFFFF
*/
uint16_t gpio_input_port_get(uint32_t gpio_periph)
{
    return (uint16_t)(GPIO_ISTAT(gpio_periph));
}

/*!
    @简介      获取GPIO引脚输出状态
    @参数[输入]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    @参数[输入]  pin: GPIO引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    @参数[输出] 无
    @返回值     gpio引脚输出状态: SET or RESET
*/
FlagStatus gpio_output_bit_get(uint32_t gpio_periph,uint32_t pin)
{
    if((uint32_t)RESET !=(GPIO_OCTL(gpio_periph)&(pin))){
        return SET;
    }else{
        return RESET;
    }
}

/*!
    @简介      获取GPIO端口输出状态
    @参数[in]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G) 
    @参数[out] 无
    @返回值     gpio所有引脚的输出状态 0x0000-0xFFFF
*/
uint16_t gpio_output_port_get(uint32_t gpio_periph)
{
    return ((uint16_t)GPIO_OCTL(gpio_periph));
}

/*!
    @简介      配置GPIO引脚映射
    @参数[输入]  gpio_remap: 选择要重新映射的引脚
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_SPI0_REMAP: SPI0 重映射
      @arg        GPIO_I2C0_REMAP: I2C0 重映射
      @arg        GPIO_USART0_REMAP: USART0 重映射
      @arg        GPIO_USART1_REMAP: USART1 重映射
      @arg        GPIO_USART2_PARTIAL_REMAP: USART2 部分重映射
      @arg        GPIO_USART2_FULL_REMAP: USART2 全部重映射
      @arg        GPIO_TIMER0_PARTIAL_REMAP: TIMER0 部分重映射 
      @arg        GPIO_TIMER0_FULL_REMAP: TIMER0 全部重映射
      @arg        GPIO_TIMER1_PARTIAL_REMAP0: TIMER1 部分重映射
      @arg        GPIO_TIMER1_PARTIAL_REMAP1: TIMER1 部分重映射
      @arg        GPIO_TIMER1_FULL_REMAP: TIMER1 全部重映射
      @arg        GPIO_TIMER2_PARTIAL_REMAP: TIMER2 部分重映射
      @arg        GPIO_TIMER2_FULL_REMAP: TIMER2 全部重映射
      @arg        GPIO_TIMER3_REMAP: TIMER3 重映射
      @arg        GPIO_CAN_PARTIAL_REMAP: CAN 部分重映射(only for GD32F30X_HD devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_CAN_FULL_REMAP: CAN 全部重映射(only for GD32F30X_HD devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_CAN0_PARTIAL_REMAP: CAN0部分重映射(only for GD32F30X_CL（互联型） devices)
      @arg        GPIO_CAN0_FULL_REMAP: CAN0全部重映射(only for GD32F30X_CL（互联型） devices)
      @arg        GPIO_PD01_REMAP: PD01重映射
      @arg        GPIO_TIMER4CH3_IREMAP: TIMER4通道3内部重映射
      @arg        GPIO_ADC0_ETRGINS_REMAP: ADC0注入转换外部触发重映射(only for GD32F30X_HD（高密度型） devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_ADC0_ETRGREG_REMAP: ADC0规则转换外部触发重映射(only for GD32F30X_HD（高密度型） devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_ADC1_ETRGINS_REMAP: ADC1注入转换外部触发重映射(only for GD32F30X_HD（高密度型） devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_ADC1_ETRGREG_REMAP: ADC1规则转换外部触发重映射(only for GD32F30X_HD（高密度型） devices and GD32F30X_XD（超高密度型） devices)
      @arg        GPIO_ENET_REMAP: ENET 重映射(only for GD32F30X_CL（互联型） devices) 
      @arg        GPIO_CAN1_REMAP: CAN1 重映射(only for GD32F30X_CL（互联型）devices)
      @arg        GPIO_SWJ_NONJTRST_REMAP: 全部的SWJ(JTAG-DP + SW-DP)，但是不包括NJTRST
      @arg        GPIO_SWJ_SWDPENABLE_REMAP: JTAG-DP除能，SW-DP使能 
      @arg        GPIO_SWJ_DISABLE_REMAP: JTAG-DP除能，SW-DP除能
      @arg        GPIO_SPI2_REMAP: SPI2 重映射 
      @arg        GPIO_TIMER1ITR0_REMAP: TIMER1 内部触发0重映射(only for GD32F30X_CL（互联型） devices)
      @arg        GPIO_PTP_PPS_REMAP: 以太网PTP PPS重映射(only for GD32F30X_CL（互联型） devices) 
      @arg        GPIO_TIMER8_REMAP: TIMER8 重映射
      @arg        GPIO_TIMER9_REMAP: TIMER9 重映射
      @arg        GPIO_TIMER10_REMAP: TIMER10 重映射
      @arg        GPIO_TIMER12_REMAP: TIMER12 重映射
      @arg        GPIO_TIMER13_REMAP: TIMER13 重映射
      @arg        GPIO_EXMC_NADV_REMAP: EXMC_NADV 连接/断开 
      @arg        GPIO_CTC_REMAP0: CTC 是否使能(PD15)
      @arg        GPIO_CTC_REMAP1: CTC 是否使能(PF0)
    @参数[输入]  newvalue是否使能: ENABLE：使能 or DISABLE：失能
    @参数[输出] 无
    @返回值     无
*/
void gpio_pin_remap_config(uint32_t remap, ControlStatus newvalue)
{
    uint32_t remap1 = 0U, remap2 = 0U, temp_reg = 0U, temp_mask = 0U;

    if(((uint32_t)0x80000000U) == (remap & 0x80000000U)){
        /* 获取AFIO_PCF1寄存器值 */
        temp_reg = AFIO_PCF1;
    }else{
        /* 获取AFIO_PCF0寄存器值 */
        temp_reg = AFIO_PCF0;
    }

    temp_mask = (remap & PCF_POSITION_MASK) >> 0x10U;
    remap1 = remap & LSB_16BIT_MASK;

    /* 判断引脚映射类型 */
    if((PCF_LOCATION1_MASK | PCF_LOCATION2_MASK) == (remap & (PCF_LOCATION1_MASK | PCF_LOCATION2_MASK))){
        temp_reg &= PCF_SWJCFG_MASK;
        AFIO_PCF0 &= PCF_SWJCFG_MASK;
    }else if(PCF_LOCATION2_MASK == (remap & PCF_LOCATION2_MASK)){
        remap2 = ((uint32_t)0x03U) << temp_mask;
        temp_reg &= ~remap2;
        temp_reg |= ~PCF_SWJCFG_MASK;
    }else{
        temp_reg &= ~(remap1 << ((remap >> 0x15U)*0x10U));
        temp_reg |= ~PCF_SWJCFG_MASK;
    }
    
    /* 设置引脚重映射值 */
    if(DISABLE != newvalue){
        temp_reg |= (remap1 << ((remap >> 0x15U)*0x10U));
    }
    
    if(AFIO_PCF1_FIELDS == (remap & AFIO_PCF1_FIELDS)){
        /* 设置AFIO_PCF1寄存器值 */
        AFIO_PCF1 = temp_reg;
    }else{
        /* 设置AFIO_PCF0寄存器值 */
        AFIO_PCF0 = temp_reg;
    }
}

#ifdef GD32F30X_CL
/*!
    \brief      select ethernet MII or RMII PHY
    \param[in]  enet_sel: ethernet MII or RMII PHY selection
      \arg        GPIO_ENET_PHY_MII: configure ethernet MAC for connection with an MII PHY
      \arg        GPIO_ENET_PHY_RMII: configure ethernet MAC for connection with an RMII PHY
    \param[out] none
    \retval     none
*/
void gpio_ethernet_phy_select(uint32_t enet_sel)
{
    /* clear AFIO_PCF0_ENET_PHY_SEL bit */
    AFIO_PCF0 &= (uint32_t)(~AFIO_PCF0_ENET_PHY_SEL);

    /* select MII or RMII PHY */
    AFIO_PCF0 |= (uint32_t)enet_sel;
}
#endif /* GD32F30X_CL */

/*!
    @简介      选择GPIO引脚外接源
    @参数1[输入]  output_port: GPIO 事件输出端口
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PORT_SOURCE_GPIOA: 输出端口源 A
      @arg        GPIO_PORT_SOURCE_GPIOB: 输出端口源 B
      @arg        GPIO_PORT_SOURCE_GPIOC: 输出端口源 C
      @arg        GPIO_PORT_SOURCE_GPIOD: 输出端口源 D
      @arg        GPIO_PORT_SOURCE_GPIOE: 输出端口源 E 
      @arg        GPIO_PORT_SOURCE_GPIOF: 输出端口源 F
      @arg        GPIO_PORT_SOURCE_GPIOG: 输出端口源 G
    @参数2[输入]  output_pin: GPIO 输出引脚源
                可以选择一个或多个参数，如下所示:
      @arg        GPIO_PIN_SOURCE_x(x=0..15)
    @参数[输出] 无
    @返回值     无
*/
void gpio_exti_source_select(uint8_t output_port, uint8_t output_pin)
{
    uint32_t source = 0U;
    source = ((uint32_t)0x0FU) << (AFIO_EXTI_SOURCE_FIELDS * (output_pin & AFIO_EXTI_SOURCE_MASK));

    /* 选择EXTI源 */
    if(GPIO_PIN_SOURCE_4 > output_pin){
        /* 选择 EXTI0/EXTI1/EXTI2/EXTI3 */
        AFIO_EXTISS0 &= ~source;
        AFIO_EXTISS0 |= (((uint32_t)output_port) << (AFIO_EXTI_SOURCE_FIELDS * (output_pin & AFIO_EXTI_SOURCE_MASK)));
    }else if(GPIO_PIN_SOURCE_8 > output_pin){
        /* 选择 EXTI4/EXTI5/EXTI6/EXTI7 */
        AFIO_EXTISS1 &= ~source;
        AFIO_EXTISS1 |= (((uint32_t)output_port) << (AFIO_EXTI_SOURCE_FIELDS * (output_pin & AFIO_EXTI_SOURCE_MASK)));
    }else if(GPIO_PIN_SOURCE_12 > output_pin){
        /* 选择 EXTI8/EXTI9/EXTI10/EXTI11 */
        AFIO_EXTISS2 &= ~source;
        AFIO_EXTISS2 |= (((uint32_t)output_port) << (AFIO_EXTI_SOURCE_FIELDS * (output_pin & AFIO_EXTI_SOURCE_MASK)));
    }else{
        /* 选择 EXTI12/EXTI13/EXTI14/EXTI15 */
        AFIO_EXTISS3 &= ~source;
        AFIO_EXTISS3 |= (((uint32_t)output_port) << (AFIO_EXTI_SOURCE_FIELDS * (output_pin & AFIO_EXTI_SOURCE_MASK)));
    }
}

/*!
    \brief      configure GPIO pin event output
    \param[in]  output_port: gpio event output port
                only one parameter can be selected which are shown as below:
      \arg        GPIO_EVENT_PORT_GPIOA: event output port A
      \arg        GPIO_EVENT_PORT_GPIOB: event output port B
      \arg        GPIO_EVENT_PORT_GPIOC: event output port C
      \arg        GPIO_EVENT_PORT_GPIOD: event output port D
      \arg        GPIO_EVENT_PORT_GPIOE: event output port E
    \param[in]  output_pin: GPIO event output pin
                only one parameter can be selected which are shown as below:
      \arg        GPIO_EVENT_PIN_x(x=0..15)
    \param[out] none
    \retval     none
*/
void gpio_event_output_config(uint8_t output_port, uint8_t output_pin)
{
    uint32_t reg = 0U;
    reg = AFIO_EC;
    
    /* clear AFIO_EC_PORT and AFIO_EC_PIN bits */
    reg &= (uint32_t)(~(AFIO_EC_PORT|AFIO_EC_PIN));
    
    reg |= (uint32_t)((uint32_t)output_port << GPIO_OUTPUT_PORT_OFFSET);
    reg |= (uint32_t)output_pin;
    
    AFIO_EC = reg;
}

/*!
    \brief      enable GPIO pin event output
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_event_output_enable(void)
{
    AFIO_EC |= AFIO_EC_EOE;
}

/*!
    \brief      disable GPIO pin event output
    \param[in]  none
    \param[out] none
    \retval     none
*/
void gpio_event_output_disable(void)
{
    AFIO_EC &= (uint32_t)(~AFIO_EC_EOE);
}

/*!
    \brief      lock GPIO pin
    \param[in]  gpio_periph: GPIOx(x = A,B,C,D,E,F,G)
    \param[in]  pin: GPIO pin
                one or more parameters can be selected which are shown as below:
      \arg        GPIO_PIN_x(x=0..15), GPIO_PIN_ALL
    \param[out] none
    \retval     none
*/
void gpio_pin_lock(uint32_t gpio_periph,uint32_t pin)
{
    uint32_t lock = 0x00010000U;
    lock |= pin;

    /* lock key writing sequence: write 1 -> write 0 -> write 1 -> read 0 -> read 1 */
    GPIO_LOCK(gpio_periph) = (uint32_t)lock;
    GPIO_LOCK(gpio_periph) = (uint32_t)pin;
    GPIO_LOCK(gpio_periph) = (uint32_t)lock;
    lock = GPIO_LOCK(gpio_periph);
    lock = GPIO_LOCK(gpio_periph);
}

/*!
    \brief      configure the I/O compensation cell
    \param[in]  compensation: specifies the I/O compensation cell mode
                only one parameter can be selected which are shown as below:
      \arg        GPIO_COMPENSATION_ENABLE: I/O compensation cell is enabled
      \arg        GPIO_COMPENSATION_DISABLE: I/O compensation cell is disabled
    \param[out] none
    \retval     none
*/
void gpio_compensation_config(uint32_t compensation)
{
    uint32_t reg;
    reg = AFIO_CPSCTL;

    /* reset the AFIO_CPSCTL_CPS_EN bit and set according to gpio_compensation */
    reg &= ~AFIO_CPSCTL_CPS_EN;
    AFIO_CPSCTL = (reg | compensation);
}

/*!
    \brief      check the I/O compensation cell is ready or not
    \param[in]  none
    \param[out] none
    \retval     FlagStatus: SET or RESET
  */
FlagStatus gpio_compensation_flag_get(void)
{
    if(((uint32_t)RESET) != (AFIO_CPSCTL & AFIO_CPSCTL_CPS_RDY)){
        return SET;
    }else{
        return RESET;
    }
}
