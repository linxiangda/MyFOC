#include "FOC.h"
#include <math.h>

FOC foc = {
	.voltage_power_supply=24.0,
    .target_position=0,
	.target_velocity=0,
	.target_current=0,
};
/**
 * @brief 电角度计算
 * @param float angle 不计圈角度
 * @return foc.electric_angle 电角度
 * @attention 角度归到0-2PI，实际电角度=测得电角度-编码器与电机的偏移量
 */
float electricalAngle(float angle){
	foc.electric_angle = normalizeAngle((float)(PP * DIR) * angle - foc.zero_electric_angle);
	return foc.electric_angle;
}
/**
 * @brief Iq,Id计算
 * @param float angle_el 电角度
 * @return foc.electric_angle 电角度
 * @attention 计算两项旋转电流并滤波
 */
void Get_I(float angle_el){
	float Ialpha = foc.Ia;
	float Ibeta = _1_SQRT3 * foc.Ia + _2_SQRT3 * foc.Ib;
	float c = cosf(angle_el);
	float s = sinf(angle_el);  
	foc.Id = Ialpha * c + Ibeta * s;
	foc.Iq = Ibeta * c - Ialpha * s;

	foc.Iq = LowPassFilter2(foc.Iq,Ts_T0,0.95f);
	foc.Id = LowPassFilter3(foc.Id,Ts_T0,0.95f);
}
/**
 * @brief SVPWM计算
 * @param float Uq 输出Uq
 * @param float Ud 输出Ud
 * @param float angle_el 电角度
 * @return none
 * @attention 根据Uq,Ud。判断扇区，计算T0、T1、T2，根据扇区计算Ta、Tb、Tc。
			  从而计算Ua,Ub,Uc
 */
void setPhaseVoltage(float Uq, float Ud, float angle_el)
{
	/* 逆 Park：dq -> alpha/beta */
	float Ualpha = Ud * cosf(angle_el) - Uq * sinf(angle_el);
	float Ubeta  = Ud * sinf(angle_el) + Uq * cosf(angle_el);

	/* 电压矢量幅值和角度 */
	float magnitude = sqrtf(Ualpha * Ualpha + Ubeta * Ubeta);
	float vector_angle = normalizeAngle(atan2f(Ubeta, Ualpha));

	/* 扇区选择 */
	int sector = floorf(vector_angle / _PI_3) + 1;

	/* 计算 T1、T2、T0 */
	float k = magnitude / foc.voltage_power_supply;
	float T1 = _SQRT3 * sinf((float)sector * _PI_3 - vector_angle) * k;
	float T2 = _SQRT3 * sinf(vector_angle - ((float)sector - 1.0f) * _PI_3) * k;
	float T0 = 1 - T1 - T2;

	/* 根据扇区计算Ta、Tb、Tc */
	float Ta, Tb, Tc;
	switch (sector)
	{
	case 1:
		Ta = T1 + T2 + T0 / 2;
		Tb = T2 + T0 / 2;
		Tc = T0 / 2;
		break;
	case 2:
		Ta = T1 + T0 / 2;
		Tb = T1 + T2 + T0 / 2;
		Tc = T0 / 2;
		break;
	case 3:
		Ta = T0 / 2;
		Tb = T1 + T2 + T0 / 2;
		Tc = T2 + T0 / 2;
		break;
	case 4:
		Ta = T0 / 2;
		Tb = T1 + T0 / 2;
		Tc = T1 + T2 + T0 / 2;
		break;
	case 5:
		Ta = T2 + T0 / 2;
		Tb = T0 / 2;
		Tc = T1 + T2 + T0 / 2;
		break;
	case 6:
		Ta = T1 + T2 + T0 / 2;
		Tb = T0 / 2;
		Tc = T1 + T0 / 2;
		break;
	default:
		Ta = 0;
		Tb = 0;
		Tc = 0;
		break;
	}

	float Ua = Ta * foc.voltage_power_supply;
	float Ub = Tb * foc.voltage_power_supply;
	float Uc = Tc * foc.voltage_power_supply;

	Pwm_Set(Ua, Ub, Uc);
}
/**
 * @brief 编码器与电机的偏移量计算
 * @param none
 * @return none
 * @attention 开环把电机打到电气0位（α轴/a轴），测量此时编码器值，所得即为偏移量
 */
void calibration(void){
	setPhaseVoltage(1.5,0,_3PI_2);
	delay_1ms(1000);
	for(int i = 0; i < 10; i++){
		AS5047P_updata();
	    delay_1ms(1);
	}
	foc.zero_electric_angle = 0;
	foc.zero_electric_angle = electricalAngle(as5047p.angle);
	Flash_Cal_Write(foc.zero_electric_angle); 
	setPhaseVoltage(0,0,_3PI_2);

	Serial_SendString("angle_el0:");
	Serial_SendFloatNumber(foc.zero_electric_angle);
	Serial_SendString("\n");
}


