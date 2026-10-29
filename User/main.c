#include "main.h"

#include "Mydef.h"
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "GPIO.h"
#include "Serial.h"
#include "CAN.h"
#include "Timer.h"
#include "PWM.h"
#include "ADC.h"
#include "SPI.h"
#include "Flash.h"

#include "PID.h"
#include "FOC.h"
#include "VOFA.h"
#include "Control.h"

#include "AS5047P.h"
#include "DRV8323.h"
#include "WS2812B.h"
uint16_t time_main = 0;
extern FOCStruct foc;
extern AS5047P as5047p;
extern volatile MOTOR_Mode motor_mode;
extern Error_type error_type;

extern PID position_pid;
extern PID velocity_pid;
extern PID Iq_pid;
extern PID Id_pid;
int main(void){
	systick_config();
//	CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;  // 使能 DWT
//	DWT->CYCCNT = 0;                                  // 清零周期计数
//	DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;              // 使能周期计数器
	rcu_config();
	nvic_priority_group_set(NVIC_PRIGROUP_PRE2_SUB2);//分组2：抢占2位/响应2位
	/*  注入组ADC完成中断 0-0
		定时器0中断 1-0
		串口空闲中断 2-0
		定时器5中断 2-1
		systick 3-3
	*/
	delay_1ms(500);//等1s，启动电压稳定
    GPIO_Init();//GPIO初始化
	USART2_Init();//串口初始化
	CAN0_Init();//CAN初始化
	TIMER5_Init();//定时器5初始化，1ms，先不使能
	TIMER0_PWM_Init();//定时器1/PWM初始化，50us/20khz，先不使能
	
	WS2812B_Init();//RGB灯初始化，使用定时器3
	AS5047P_Init();//编码器初始化，使用SPI0
	DRV8323_Init();//驱动芯片初始化，使用SPI1
	ADC0_Init();//ADC0初始化
	
	//PID初始化,iq,id还要对合成矢量限幅
	#if MOTOR_LEG_WHEEL == 0
		/* GIM6010-8：腿电机 */
		PID_Init(&position_pid, 10.0f, 0.0f, 0.1f, 80.0f);
		PID_Init(&velocity_pid, 0.1f, 0.3f, 0.0f, 8.0f);
		PID_Init(&Iq_pid,       2.0f, 10.0f, 0.0f, 13.16f);
		PID_Init(&Id_pid,       2.0f, 10.0f, 0.0f, 13.16f);//单电机最大功率在3/2*12*8=144W左右
	#else
		/* M0603A_111：轮电机 */
		PID_Init(&position_pid, 10.0f, 0.0f, 0.1f, 80.0f);
		PID_Init(&velocity_pid, 0.05f, 1.5f, 0.0f, 1.0f);
		PID_Init(&Iq_pid,       10.0f, 60.0f, 0.0f, 13.16f);
		PID_Init(&Id_pid,       10.0f, 60.0f, 0.0f, 13.16f);
	#endif
	
	
	//定时器启用
	timer_enable(TIMER0);
	timer_enable(TIMER5);
	//PWM输出启动
	timer_primary_output_config(PWM_TIMER, ENABLE);
	//让注入组先跑一会儿，开启ADC校准
	delay_1ms(100);          
	ADC_Calibrate();	
	Serial_SendString("Init Over\n");
	
	//Flash读入，若已校准过，可直接运行
	if (Flash_Cal_Read(&foc.zero_electric_angle)) {
        foc.calibration_flag = 1;  
    }
	while(1){
		Start_calibration();
		if(time_main >= 20){//20ms执行
			time_main = 0;
			if(motor_mode != Motor_MODE_NONE && motor_mode != Motor_MODE_FAULT){
				// sendJustFloatPacket();//vofa发送程序，在闭环情况下发送
				sendFireWater();
			}
			Error_Check();//错误检测，不费时
		}
	}
}

/**
 * @brief 调参服务函数
 * @param none
 * @return none
 * @attention none
 */
typedef struct {
    const char *name;
    float *value;
} ParamMap;

static const ParamMap param_map[] = {
    {"kpp", &position_pid.Kp},
    {"kip", &position_pid.Ki},
    {"kdp", &position_pid.Kd},

    {"kpv", &velocity_pid.Kp},
    {"kiv", &velocity_pid.Ki},
    {"kdv", &velocity_pid.Kd},

    {"kpq", &Iq_pid.Kp},
    {"kiq", &Iq_pid.Ki},
    {"kdq", &Iq_pid.Kd},
	
    {"kpd", &Id_pid.Kp},
    {"kid", &Id_pid.Ki},
    {"kdd", &Id_pid.Kd},

    {"p", &foc.target_position},
    {"v", &foc.target_velocity},
    {"c", &foc.target_current},
};

void Serial_Parse(char *str){
    char cmd[4] = {0};
    float val = 0.0f;
	
    if (sscanf(str, "%3[^0123456789+- ]%f", cmd, &val) != 2) {//期望匹配两项
        return;
    }
    for (int i = 0; cmd[i]; i++) {//指令不区分大小写
        cmd[i] = (char)tolower((unsigned char)cmd[i]);
    }
	
	if (strcmp(cmd, "m") == 0) {
		uint8_t mode = (uint8_t)val;
		if(mode > 4) {
			error_type = Mode_Set_error;
			return;
		}
		/* 关中断设置模式 */
		uint32_t primask = __get_PRIMASK();
    	__disable_irq();
		if (motor_mode != (MOTOR_Mode)mode){
			motor_mode = (MOTOR_Mode)mode;
			foc.change_once_set=1;//改一次模式，Control执行一次FOC_CloseOut()/FOC_OpenOut()
		}
		__DMB();
    	__set_PRIMASK(primask);
		return;
	}
    for (uint16_t i = 0; i < sizeof(param_map) / sizeof(param_map[0]); i++) {
        if (strcmp(cmd, param_map[i].name) == 0) {
			*param_map[i].value = val;
			return;
        }
    }
}

void CAN_Parse(uint8_t *data){
	uint32_t mode;
	float controldata;
    memcpy(&mode, &data[0], sizeof(mode));
	memcpy(&controldata, &data[4], sizeof(controldata));

    if (mode > 4) {
        error_type = Mode_Set_error;
        return;
    }
   
	/* 关中断设置模式、数值 */
	uint32_t primask = __get_PRIMASK();
	__disable_irq();

    if (motor_mode != (MOTOR_Mode)mode) {
		motor_mode = (MOTOR_Mode)mode;
        foc.change_once_set = 1;//改一次模式，执行一次FOC_CloseOut()/FOC_OpenOut()
    }
	    if(motor_mode == Motor_MODE_C){
        foc.target_current=controldata;
        foc.target_velocity=0;
        foc.target_position=0;  
    }
    if(motor_mode == Motor_MODE_V){
        foc.target_current=0;
        foc.target_velocity=controldata;
        foc.target_position=0;  
    }
    if(motor_mode == Motor_MODE_P){
        foc.target_current=0;
        foc.target_velocity=0;
        foc.target_position=controldata;  
    }

	__DMB();
	__set_PRIMASK(primask);
    
}

void CAN_SendFeedback(void)
{
	/* 对要发送数据进行缓存，避免被打断 */
    uint8_t data1[8] = {0};
    uint8_t data2[8] = {0};
    float iq;
    float velocity;
    float position;
    uint8_t mode;
    uint8_t error;
	uint8_t calibrated;
	uint8_t calibrating;

    uint32_t primask = __get_PRIMASK();
    __disable_irq();

    iq       = foc.Iq;
    velocity = as5047p.velocity;
    position = as5047p.angle_revolution;
    mode  = (uint8_t)motor_mode;
    error = (uint8_t)error_type;
    calibrated = (uint8_t)foc.calibration_flag;
	calibrating = (uint8_t)foc.calibration_running;

    __DMB();
    __set_PRIMASK(primask);

    velocity = -(velocity / G * DIR);
    position = position / G * DIR;
	#if MOTOR_LEG_WHEEL == 0
		position = LEG_EXTEND_SIGN * (position - LEG_RETRACT_POS_RAD);
	#endif

    memcpy(&data1[0], &iq,       4);
    memcpy(&data1[4], &velocity, 4);

    memcpy(&data2[0], &position, 4);
    data2[4] = mode;
    data2[5] = error;
    data2[6] = (calibrated << 1) | (calibrating);
    data2[7] = 1;

    if (CAN0_TransmitData(0x201 + MOTOR_LEFT_RIGHT * 2 + MOTOR_LEG_WHEEL, data1, 8) == CAN_NOMAILBOX) {
        return;
    }

    if (CAN0_TransmitData(0x201 + MOTOR_LEFT_RIGHT * 2 + MOTOR_LEG_WHEEL + 0x10u, data2, 8) == CAN_NOMAILBOX) {
        return;
    }
}
