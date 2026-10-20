#include "VOFA.h"
#include "string.h"
#include "stdio.h"
/**
 * @brief 转换参数为VOFAjustFloat格式发送
 * @param float val
 * @return none
 * @attention none
 */
void sendFloat(float val)
{
    uint8_t *p = (uint8_t *)&val;
    Serial_SendByte(p[0]);
    Serial_SendByte(p[1]);
    Serial_SendByte(p[2]);
    Serial_SendByte(p[3]);
}
/**
 * @brief VOFA发送
 * @param none
 * @return none
 * @attention 发送结尾添加尾巴0x00, 0x00, 0x80, 0x7f
 */
void sendJustFloatPacket(void)
{
    sendFloat(foc.target_position);
	sendFloat(as5047p.angle_revolution * DIR);
	
	sendFloat(foc.target_velocity);
	sendFloat(as5047p.velocity * DIR);
	
	sendFloat(foc.target_current);
	sendFloat(foc.Iq);
	
	sendFloat(0.0f);
	sendFloat(foc.Id);
    
    uint8_t tail[4] = {0x00, 0x00, 0x80, 0x7f};
    Serial_SendArray(tail, 4);
}

void sendFireWater(void){
    int len;
    char packet[128];

    len = snprintf(
            packet, sizeof(packet),
            "%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",
            foc.target_position,
            (as5047p.angle_revolution / G * DIR),
            foc.target_velocity,
            (as5047p.velocity / G * DIR),
            foc.target_current,
            foc.Iq,
            0.0f,
            foc.Id
        );

    if (len > 0 && (size_t)len < sizeof(packet)){
        Serial_SendString(packet);
    }   

}
