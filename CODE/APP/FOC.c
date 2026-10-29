#include "FOC.h"
#include <math.h>
#include "Mydef.h"
#include "math_util.h"
#include "filter.h"
#include "PWM.h"
/**
 * @brief 电角度计算
 * @param float angle 不计圈角度
 * @return foc.electric_angle 电角度
 * @attention 角度归到0-2PI，实际电角度=测得电角度-编码器与电机的偏移量
 */
float electricalAngle(FOCStruct *FOC, float angle){
	FOC->electric_angle = normalizeAngle((float)(PP * DIR) * angle - FOC->zero_electric_angle);
	return FOC->electric_angle;
}
/**
 * @brief Iq,Id计算
 * @param float angle_el 电角度
 * @return FOC->electric_angle 电角度
 * @attention 计算两项旋转电流并滤波
 */
void Get_I(FOCStruct *FOC, float angle_el){
	float Ialpha = FOC->Ia;
	float Ibeta = _1_SQRT3 * FOC->Ia + _2_SQRT3 * FOC->Ib;
	float c = cosf(angle_el);
	float s = sinf(angle_el);  
	FOC->Id = Ialpha * c + Ibeta * s;
	FOC->Iq = Ibeta * c - Ialpha * s;

	FOC->Iq = LowPassFilter2(FOC->Iq, Ts_T0, 0.9025f);
	FOC->Id = LowPassFilter3(FOC->Id, Ts_T0, 0.9025f);
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
void setPhaseVoltage(FOCStruct *FOC, float *Uq, float *Ud, float angle_el)
{
    /* 合成电压矢量限幅 */
    float max_voltage =FOC->voltage_power_supply * 0.57735026919f * 0.95f;

    if (!(max_voltage > 0.0f)) {
        Pwm_DataReset();
        return;
    }
    float voltage_sq = *Uq * *Uq + *Ud * *Ud;

    if (voltage_sq > max_voltage * max_voltage) {
        float scale = max_voltage / sqrtf(voltage_sq);
        *Uq *= scale;
        *Ud *= scale;
    }

	/* 逆 Park：dq -> alpha/beta */
	float Ualpha = *Ud * cosf(angle_el) - *Uq * sinf(angle_el);
	float Ubeta  = *Ud * sinf(angle_el) + *Uq * cosf(angle_el);

	/* 电压矢量幅值和角度 */
	float magnitude = sqrtf(Ualpha * Ualpha + Ubeta * Ubeta);
	float vector_angle = normalizeAngle(atan2f(Ubeta, Ualpha));

	/* 扇区选择 */
	int sector = floorf(vector_angle / _PI_3) + 1;

	/* 计算 T1、T2、T0 */
	float k = magnitude / FOC->voltage_power_supply;
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

	float Ua = Ta * FOC->voltage_power_supply;
	float Ub = Tb * FOC->voltage_power_supply;
	float Uc = Tc * FOC->voltage_power_supply;

	Pwm_Set(FOC->voltage_power_supply,Ua, Ub, Uc);
}



