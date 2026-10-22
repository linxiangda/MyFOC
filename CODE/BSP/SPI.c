#include "SPI.h"
/**
 * @brief SPI0初始化
 * @param none
 * @return none
 * @attention 16位数据，下降沿采样
 */
void SPI0_Init(void){
	/* 引脚初始化 */
	gpio_init(ENC_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, ENC_CS_PIN);
	gpio_init(ENC_SCK_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, ENC_SCK_PIN);
	gpio_init(ENC_MISO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, ENC_MISO_PIN);
	gpio_init(ENC_MOSI_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, ENC_MOSI_PIN);

    /* 硬件重映射 */
	gpio_pin_remap_config(GPIO_SWJ_SWDPENABLE_REMAP, ENABLE);
	gpio_pin_remap_config(GPIO_SPI0_REMAP, ENABLE);
    /* SPI配置 */
	spi_parameter_struct spi_initpara;
	spi_struct_para_init(&spi_initpara);
	spi_initpara.device_mode = SPI_MASTER;             //主机
    spi_initpara.trans_mode = SPI_TRANSMODE_FULLDUPLEX;//双线全双工
    spi_initpara.frame_size = SPI_FRAMESIZE_16BIT;     //16位数据帧
    spi_initpara.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;//CPOL=0, CPHA=1(空闲为0，第二个边缘采样)
    spi_initpara.nss = SPI_NSS_SOFT;     //软件cs
    spi_initpara.prescale = SPI_PSC_16;  // 最高时钟10MHz
    spi_initpara.endian = SPI_ENDIAN_MSB;//高位先行
    spi_init(ENC_SPI, &spi_initpara);
		   
    spi_enable(ENC_SPI);
}
/**
 * @brief SPI1初始化
 * @param none
 * @return none
 * @attention 16位数据，下降沿采样
 */
void SPI1_Init(void){
	/* 引脚初始化 */
	gpio_init(DRV_CS_PORT, GPIO_MODE_OUT_PP, GPIO_OSPEED_50MHZ, DRV_CS_PIN);
	gpio_init(DRV_SCK_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DRV_SCK_PIN);
	gpio_init(DRV_MISO_PORT, GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, DRV_MISO_PIN);
	gpio_init(DRV_MOSI_PORT, GPIO_MODE_AF_PP, GPIO_OSPEED_50MHZ, DRV_MOSI_PIN);

    /* SPI配置 */
	spi_parameter_struct spi_initpara;
	spi_struct_para_init(&spi_initpara);
	spi_initpara.device_mode = SPI_MASTER;             //主机
    spi_initpara.trans_mode = SPI_TRANSMODE_FULLDUPLEX;//双线全双工
    spi_initpara.frame_size = SPI_FRAMESIZE_16BIT;     //16位数据帧
    spi_initpara.clock_polarity_phase = SPI_CK_PL_LOW_PH_2EDGE;//CPOL=0, CPHA=1(空闲为0，第二个边缘采样)
    spi_initpara.nss = SPI_NSS_SOFT;     //软件cs
    spi_initpara.prescale = SPI_PSC_32;  // 最高时钟10MHz
    spi_initpara.endian = SPI_ENDIAN_MSB;//高位先行
    spi_init(DRV_SPI, &spi_initpara);
		   
    spi_enable(DRV_SPI);
}
/**
 * @brief SPI0起始帧
 * @param none
 * @return none
 * @attention none
 */
void SPI0_Start(void)
{
	gpio_bit_reset(ENC_CS_PORT,ENC_CS_PIN); 
	delay_400_ns();
}
/**
 * @brief SPI0结束帧
 * @param none
 * @return none
 * @attention none
 */
void SPI0_Stop(void)
{ 
	delay_400_ns();
	gpio_bit_set(ENC_CS_PORT,ENC_CS_PIN);
}
/**
 * @brief SPI0交换
 * @param none
 * @return none
 * @attention none
 */
uint16_t SPI0_SwapByte(uint16_t ByteSend)
{
	while(spi_i2s_flag_get(ENC_SPI,SPI_FLAG_TBE) == RESET);
	spi_i2s_data_transmit(ENC_SPI,ByteSend);
	while(spi_i2s_flag_get(ENC_SPI,SPI_FLAG_RBNE) == RESET);
	return spi_i2s_data_receive(ENC_SPI);
}

/**
 * @brief SPI1起始帧
 * @param none
 * @return none
 * @attention none
 */
void SPI1_Start(void)
{
	gpio_bit_reset(DRV_CS_PORT,DRV_CS_PIN); 
	delay_400_ns();
}
/**
 * @brief SPI1结束帧
 * @param none
 * @return none
 * @attention none
 */
void SPI1_Stop(void)
{ 
	delay_400_ns();
	gpio_bit_set(DRV_CS_PORT,DRV_CS_PIN);
}
/**
 * @brief SPI1交换
 * @param none
 * @return none
 * @attention none
 */
uint16_t SPI1_SwapByte(uint16_t ByteSend)
{
	while(spi_i2s_flag_get(DRV_SPI,SPI_FLAG_TBE) == RESET);
	spi_i2s_data_transmit(DRV_SPI,ByteSend);
	while(spi_i2s_flag_get(DRV_SPI,SPI_FLAG_RBNE) == RESET);
	return spi_i2s_data_receive(DRV_SPI);
}
