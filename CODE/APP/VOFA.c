#include "VOFA.h"
#include "string.h"
#include "stdio.h"

#include "Serial.h"
#include "FOC.h"
#include "AS5047P.h"
#include "Mydef.h"
extern FOCStruct foc;
extern AS5047P as5047p;
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

    float position_show = (as5047p.angle_revolution / G * DIR);
    #if MOTOR_LEG_WHEEL == 0
        position_show = LEG_EXTEND_SIGN * (as5047p.angle_revolution / G * DIR - LEG_RETRACT_POS_RAD);
    #endif
    len = snprintf(
            packet, sizeof(packet),
            "%.6f,%.6f,%.4f,%.4f,%.4f,%.4f,%.4f,%.4f\n",
            foc.target_position,
            position_show,
            foc.target_velocity,
            -(as5047p.velocity / G * DIR),
            foc.target_current,
            foc.Iq,
            0.0f,
            foc.Id
        );

    if (len > 0 && (size_t)len < sizeof(packet)){
        Serial_SendString(packet);
    }   

}
