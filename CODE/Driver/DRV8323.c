#include "DRV8323.h"
#include "GPIO.h"
#include "SPI.h"
#include "Serial.h"
/**
 * @brief DRV8323初始化
 * @param none
 * @return none
 * @attention 若DRV复位失败，报错，卡死程序
 */
void DRV8323_Init(void){
	DRV_EN_reset();
	SPI1_Init();
	if (0 != DRV8323_reset())
	{
		Serial_SendString("\n\rDRV8323 init fail!\n\r");
		while (1);
	}
}
/**
 * @brief 读DRV8323寄存器
 * @param const DRV8323_Reg reg
 * @return 11位数据
 * @attention 读命令，最高位置1；3位地址；11位数据
 */
uint16_t DRV8323_read_reg(const DRV8323_Reg reg){
    uint16_t controlword = 0x8000 | reg;
    uint16_t recbuff = 0xbeef;

    SPI1_Start();
    recbuff = SPI1_SwapByte(controlword);
    SPI1_Stop();
    return (0x7ff & recbuff); //11位数据
}
/**
 * @brief 写DRV8323寄存器
 * @param reg 寄存器地址
 * @param regVal 要写入的11位数据
 * @return none
 * @attention 读命令，最高位置0；
 */
void DRV8323_write_reg(const DRV8323_Reg reg, uint16_t regVal){
    uint16_t controlword = (reg | (regVal & 0x7ff));

    SPI1_Start();
    SPI1_SwapByte(controlword);
    SPI1_Stop();
}
/**
 * @brief 读故障状态寄存器1
 * @param none
 * @return FSR1 原始值
 */
uint16_t DRV8323_read_FSR1(void){
    return DRV8323_read_reg(DRV8323_FAULT_STATUS_1);
}
/**
 * @brief 读故障状态寄存器2
 * @param none
 * @return FSR2 原始值
 */
uint16_t DRV8323_read_FSR2(void){
    return DRV8323_read_reg(DRV8323_FAULT_STATUS_2);
}
/**
 * @brief 读取完整故障状态
 * @param none
 * @return 高11位=FSR1，低11位=FSR2
 * @attention 两次读之间留10us，保证SPI时序稳定
 */
uint32_t DRV8323_getFault(void){
    uint16_t val1 = DRV8323_read_FSR1();
    delay_1us(10);
    uint16_t val2 = DRV8323_read_FSR2();
    delay_1us(10);

    val1 &= 0x07FF;
    val2 &= 0x07FF;
	
    /* 合并成32位：FSR1在高位，FSR2在低位 */
    return (uint32_t)(val1 << 11) | val2;
}
/**
 * @brief 写驱动控制寄存器 DRIVER_CONTROL
 * @param DIS_CPUV 电荷泵欠压故障使能
 * @param DIS_GDF 栅极驱动故障使能
 * @param OTW_REP 过温上报
 * @param PWM_MODE PWM输入模式
 * @param PWM_COM 1x模式同步/异步整流
 * @param PWM_DIR 1x模式方向
 * @param COAST 滑行（关断所有MOSFET）
 * @param BRAKE 刹车（低边导通）
 * @param CLR_FLT 清除故障锁存
 * @return none
 * @attention 各位按bit位拼装后写入
 */
void DRV8323_write_DCR(int DIS_CPUV, int DIS_GDF, int OTW_REP, int PWM_MODE, int PWM_COM, int PWM_DIR, int COAST, int BRAKE, int CLR_FLT){
    uint16_t val = (DIS_CPUV << 9) | (DIS_GDF << 8) | (OTW_REP << 7) | (PWM_MODE << 5) | (PWM_COM << 4) | (PWM_DIR << 3) | (COAST << 2) | (BRAKE << 1) | CLR_FLT;
    DRV8323_write_reg(DRV8323_DRIVER_CONTROL, val);
}
/**
 * @brief 写高边栅极驱动寄存器 GATE_DRIVE_HS
 * @param LOCK 锁存配置
 * @param IDRIVEP_HS 高边开通电流
 * @param IDRIVEN_HS 高边关断电流
 * @return none
 */
void DRV8323_write_HSR(int LOCK, int IDRIVEP_HS, int IDRIVEN_HS){
    uint16_t val = (LOCK << 8) | (IDRIVEP_HS << 4) | IDRIVEN_HS;
    DRV8323_write_reg(DRV8323_GATE_DRIVE_HS, val);
}
/**
 * @brief 写低边栅极驱动寄存器 GATE_DRIVE_LS
 * @param CBC 逐周期限流使能
 * @param TDRIVE 峰值栅极电流持续时间
 * @param IDRIVEP_LS 低边开通电流
 * @param IDRIVEN_LS 低边关断电流
 * @return none
 */
void DRV8323_write_LSR(int CBC, int TDRIVE, int IDRIVEP_LS, int IDRIVEN_LS){
    uint16_t val = (CBC << 10) | (TDRIVE << 8) | (IDRIVEP_LS << 4) | IDRIVEN_LS;
    DRV8323_write_reg(DRV8323_GATE_DRIVE_LS, val);
}
/**
 * @brief 写过流保护控制寄存器 OCP_CONTROL
 * @param TRETRY 过流重试时间
 * @param DEAD_TIME 死区时间
 * @param OCP_MODE 过流模式（锁存/重试/上报/关闭）
 * @param OCP_DEG 过流去毛刺时间
 * @param VDS_LVL VDS过流阈值
 * @return none
 */
void DRV8323_write_OCPCR(int TRETRY, int DEAD_TIME, int OCP_MODE, int OCP_DEG, int VDS_LVL){
    uint16_t val = (TRETRY << 10) | (DEAD_TIME << 8) | (OCP_MODE << 6) | (OCP_DEG << 4) | VDS_LVL;
    DRV8323_write_reg(DRV8323_OCP_CONTROL, val);
}
/**
 * @brief 写电流采样控制寄存器 CSA_CONTROL
 * @param CSA_FET 采样正输入选择
 * @param VREF_DIV 参考电压分压
 * @param LS_REF 低边参考选择
 * @param CSA_GAIN 采样增益
 * @param DIS_SEN 采样过流保护
 * @param CSA_CAL_A/B/C 三相采样校准使能
 * @param SEN_LVL 采样过流阈值
 * @return none
 */
void DRV8323_write_CSACR(int CSA_FET, int VREF_DIV, int LS_REF, int CSA_GAIN, int DIS_SEN, int CSA_CAL_A, int CSA_CAL_B, int CSA_CAL_C, int SEN_LVL){
    uint16_t val = (CSA_FET << 10) | (VREF_DIV << 9) | (LS_REF << 8) | (CSA_GAIN << 6) | (DIS_SEN << 5) | (CSA_CAL_A << 4) | (CSA_CAL_B << 3) | (CSA_CAL_C << 2) | SEN_LVL;
    DRV8323_write_reg(DRV8323_CSA_CONTROL, val);
}
/**
 * @brief 使能栅极驱动输出
 * @param none
 * @return none
 * @attention none
 */
void DRV8323_enable_gd(void){
    uint16_t val = (DRV8323_read_reg(DRV8323_DRIVER_CONTROL)) & (~(0x1 << 2));
    DRV8323_write_reg(DRV8323_DRIVER_CONTROL, val);
}
/**
 * @brief 失能栅极驱动输出
 * @param none
 * @return none
 * @attention none
 */
void DRV8323_disable_gd(void){
    uint16_t val = (DRV8323_read_reg(DRV8323_DRIVER_CONTROL)) | (0x1 << 2);
    DRV8323_write_reg(DRV8323_DRIVER_CONTROL, val);
}
/**
 * @brief 电流采样放大器校准
 * @param none
 * @return none
 * @attention 三相 CSA_CAL 全置1，芯片进入校准，校准完成会自动清除
 */
void DRV8323_calibrate(void){
    uint16_t val = 0x1 << 4 | 0x1 << 3 | 0x1 << 2;
    DRV8323_write_reg(DRV8323_CSA_CONTROL, val);
}
/**
 * @brief 复位并配置DRV8323
 * @param none
 * @return 0=成功，-1=回读校验失败
 * @attention 用EN引脚复位芯片后依次写配置，每步留100us，
 *            最后逐寄存器回读校验，防止SPI通信错误导致配置没写进去
 */
int DRV8323_reset(void){
    //DRV8323 config
    DRV_EN_reset();
    delay_1us(20);
    DRV_EN_set();
    delay_1us(20);
	
    DRV8323_calibrate();
    delay_1us(100);
    DRV8323_write_DCR(0x0, 0x0, 0x0, PWM_MODE_3X, 0x0, 0x0, 0x1, 0x0, 0x1);
    delay_1us(100);
    DRV8323_write_HSR(LOCK_OFF, IDRIVEP_HS_370MA, IDRIVEN_HS_1360MA);
    delay_1us(100);
    DRV8323_write_LSR(1, TDRIVE_1000NS, IDRIVEP_HS_370MA, IDRIVEN_HS_1360MA);
    delay_1us(100);
    DRV8323_write_CSACR(0x0, VREF_DIV_2, 0x0, CSA_GAIN_40, 0x0, 0x0, 0x0, 0x0, SEN_LVL_0_25);
    delay_1us(100);
    DRV8323_write_OCPCR(TRETRY_50US, DEADTIME_50NS, OCP_RETRY, OCP_DEG_4US, VDS_LVL_0_45);
    delay_1us(100);

    if (0x0024 != DRV8323_read_reg(DRV8323_DRIVER_CONTROL)){
        return -1;
    }
    if (0x03AD != DRV8323_read_reg(DRV8323_GATE_DRIVE_HS)){
        return -1;
    }
    if (0x05AD != DRV8323_read_reg(DRV8323_GATE_DRIVE_LS)){
        return -1;
    }
    if (0x02C0 != DRV8323_read_reg(DRV8323_CSA_CONTROL)){
        return -1;
    }
    if (0x0455 != DRV8323_read_reg(DRV8323_OCP_CONTROL)){
        return -1;
    }
    return 0;
}


