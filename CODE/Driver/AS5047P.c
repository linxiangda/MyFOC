#include "AS5047P.h"
#include <stdbool.h>
#include "SPI.h"
#include "filter.h"

#define _PI 3.14159265359f
#define _2PI 6.28318530718f

/**
 * @brief AS5047P初始化
 * @param none
 * @return none
 * @attention none
 */
void AS5047P_Init(void){
	SPI0_Init();
      /* 启动角度读取，丢弃首帧响应 */
    SPI0_Start();
    (void)SPI0_SwapByte(0xFFFFu);
    SPI0_Stop();
}

/**
 * @brief SPI获取编码器初始值
 * @param none
 * @return data
 * @attention 误检返回上次值
 */
uint16_t AS5047P_getRaw(void)
{
  uint16_t data;
  uint16_t parity;
  SPI0_Start();
  data = SPI0_SwapByte(0XFFFF);
  SPI0_Stop();

  // bit14错误标志
  if (data & 0x4000u) {
      return 0xFFFFu;
  }

  // 16位偶校验
  parity = data;
  parity ^= parity >> 8;
  parity ^= parity >> 4;
  parity ^= parity >> 2;
  parity ^= parity >> 1;

  if (parity & 1u) {
      return 0xFFFFu;
  }
  
  return data & 0x3FFFu;

}
/**
 * @brief 获取不记圈角度
 * @param AS5047P *encode
 * @return encode->angle
 * @attention none
 */
float getAngle(AS5047P *encode){
	uint16_t raw = AS5047P_getRaw();
	if(raw == 0xFFFF) return encode->angle;   // 坏读数沿用上一拍
	encode->angle = (float)raw/16384.0f * 360 * _PI / 180;
	return encode->angle;
}
/**
 * @brief 获取记圈角度
 * @param AS5047P *encode
 * @return encode->angle_revolution
 * @attention none
 */
float getAngle_revolution(AS5047P *encode){
    getAngle(encode);
    float angle_change = encode->angle - encode->angle_pre;

    if(angle_change > _PI)  angle_change -= _2PI;
    if(angle_change < -_PI) angle_change += _2PI;

    encode->angle_revolution += angle_change;
    encode->angle_pre = encode->angle;

    return encode->angle_revolution;
}

/**
 * @brief 编码器更新不记圈位置/记圈位置/速度
 * @param none
 * @return none
 * @attention 0.5ms算一次速度，速度滤波
 */
void AS5047P_updata(AS5047P *encode,float ts){
  static bool first = true;
  static uint16_t cnt = 0;

  if(first){ 
    if (first) {
      uint16_t raw = AS5047P_getRaw();
      if (raw == 0xFFFFu) {
          return;  // 不用旧值完成初始化，下次继续尝试
      }
      float angle = (float)raw * (_2PI / 16384.0f);
      encode->angle = angle;
      encode->angle_pre = angle;
      encode->angle_revolution = angle;
      encode->angle_revolution_pre = angle;
      encode->velocity = 0.0f;
      cnt = 0;
      first = false;
      return;
    }
  }

  getAngle_revolution(encode);
  if(++cnt >= 10){                    // 每 0.5ms 算一次速度
    cnt = 0;
    encode->velocity = (encode->angle_revolution - encode->angle_revolution_pre) / (10.0f * ts);
    encode->angle_revolution_pre = encode->angle_revolution;
    encode->velocity = LowPassFilter1(encode->velocity, 10.0f * ts, 0.9604f);
  }
}
