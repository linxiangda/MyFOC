#include "GPIO.h"

/**
 * @brief GPIO初始化
 * @param none
 * @return none
 * @attention none
 */
void GPIO_Init(void){	
	gpio_init(DRV_EN_PORT,GPIO_MODE_OUT_PP,GPIO_OSPEED_50MHZ,DRV_EN_PIN);//DRV_EN配置为推挽输出
	gpio_init(DRV_nFAULT_PORT,GPIO_MODE_IN_FLOATING, GPIO_OSPEED_50MHZ, DRV_nFAULT_PIN);//DRV_nFAULT设置为悬空输入
}
void DRV_EN_reset(void){
	gpio_bit_reset(DRV_EN_PORT, DRV_EN_PIN);
}

void DRV_EN_set(void){
	gpio_bit_set(DRV_EN_PORT, DRV_EN_PIN);
}

uint8_t DRV_nFAULT_get(void){
	return gpio_input_bit_get(DRV_nFAULT_PORT,DRV_nFAULT_PIN);
}
