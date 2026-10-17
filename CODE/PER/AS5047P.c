#include "AS5047P.h"
#include <math.h> 
#include <stdbool.h>
AS5047P as5047p = {
	.angle_pre = 0,
	.full_rotations = 0,
};
/**
 * @brief AS5047P初始化
 * @param none
 * @return none
 * @attention none
 */
void AS5047P_Init(void){
	SPI0_Init();
}

/**
 * @brief SPI获取编码器初始值
 * @param none
 * @return data
 * @attention 误检返回上次值
 */
uint16_t AS5047P_getRaw(void)
{
    uint16_t data = 0x0000;
    
    SPI0_Start();
    data = SPI0_SwapByte(0XFFFF);
    SPI0_Stop();
	if(data & 0xC000) return 0xFFFF;   // bit15错误 / bit14校验失败
	data &= 0x3FFF;    //取14位

	return	data;

}
/**
 * @brief 获取不记圈角度
 * @param none
 * @return as5047p.angle
 * @attention none
 */
float getAngle(void){
	uint16_t raw = AS5047P_getRaw();
	if(raw == 0xFFFF) return as5047p.angle;   // 坏读数沿用上一拍
	as5047p.angle = (float)raw/16384.0f * 360 * _PI / 180;
	return as5047p.angle;
}
/**
 * @brief 获取记圈角度
 * @param none
 * @return as5047p.angle
 * @attention none
 */
float getAngle_revolution(void){
    getAngle();
    float angle_change = as5047p.angle - as5047p.angle_pre;

    if(angle_change > _PI)  angle_change -= _2PI;
    if(angle_change < -_PI) angle_change += _2PI;

    as5047p.angle_revolution += angle_change;
    as5047p.angle_pre = as5047p.angle;

    return as5047p.angle_revolution;
}
/**
 * @brief 获取速度
 * @param none
 * @return as5047p.velocity
 * @attention none
 */
float getVelocity(void){
  getAngle_revolution();

  as5047p.velocity = (as5047p.angle_revolution - as5047p.angle_revolution_pre)/Ts_T0;
  as5047p.angle_revolution_pre = as5047p.angle_revolution;
  return as5047p.velocity;
}
/**
 * @brief 首次初始化部分参数
 * @param none
 * @return none
 * @attention none
 */
void AS5047P_first(void){
  as5047p.angle = getAngle();
  as5047p.angle_pre = as5047p.angle;

  as5047p.angle_revolution = as5047p.angle; 
  as5047p.angle_revolution_pre = as5047p.angle_revolution;
}
/**
 * @brief 编码器更新不记圈位置/记圈位置/速度
 * @param none
 * @return none
 * @attention 0.5ms算一次速度，速度滤波
 */
void AS5047P_updata(void){
  static bool first = true;
  static uint16_t cnt = 0;
  static float angle_pre = 0;

  if(first){ AS5047P_first(); first = false; }
  getAngle_revolution();

  if(++cnt >= 10){                    // 每 0.5ms 算一次
    cnt = 0;
    as5047p.velocity = (as5047p.angle_revolution - angle_pre) / (10.0f * Ts_T0);
    angle_pre = as5047p.angle_revolution;
    as5047p.velocity = LowPassFilter1(as5047p.velocity, Ts_T0, 0.98f); 
  }
}
