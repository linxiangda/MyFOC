#include "CAN.h"
#include <string.h>
#define Q19_SCALE 524288.0f
can_receive_message_struct CAN0_receive_message;

/**
 * @brief CAN0初始化
 */
void CAN0_Init(void){
    can_deinit(CAN0);
    can_parameter_struct CAN_StructInit;
    CAN_StructInit.working_mode = CAN_NORMAL_MODE;
    CAN_StructInit.prescaler = 12;
    CAN_StructInit.time_segment_1 = CAN_BT_BS1_2TQ;
    CAN_StructInit.time_segment_2 = CAN_BT_BS2_2TQ;
    CAN_StructInit.resync_jump_width = CAN_BT_SJW_1TQ;
    CAN_StructInit.auto_retrans = ENABLE;
    CAN_StructInit.auto_bus_off_recovery = ENABLE;
    CAN_StructInit.auto_wake_up = DISABLE;
    CAN_StructInit.rec_fifo_overwrite = DISABLE;
    CAN_StructInit.time_triggered = DISABLE;
    CAN_StructInit.trans_fifo_order = DISABLE;
    can_init(CAN0, &CAN_StructInit);

    can_filter_parameter_struct CAN_Filter_StructInit;
    CAN_Filter_StructInit.filter_number = 0;
    CAN_Filter_StructInit.filter_fifo_number = CAN_FIFO0;
    CAN_Filter_StructInit.filter_mode = CAN_FILTERMODE_MASK;
    CAN_Filter_StructInit.filter_bits = CAN_FILTERBITS_32BIT;
    CAN_Filter_StructInit.filter_list_high = (0x100) << 5;
    CAN_Filter_StructInit.filter_list_low = 0x0000;
    CAN_Filter_StructInit.filter_mask_high = (0x7fc) << 5;
    CAN_Filter_StructInit.filter_mask_low = 0x0006;
    CAN_Filter_StructInit.filter_enable = ENABLE;
    can_filter_init(&CAN_Filter_StructInit);


    nvic_irq_enable(USBD_LP_CAN0_RX0_IRQn, 1, 0);
    can_interrupt_enable(CAN0, CAN_INT_RFNE0);
}

/**
 * @brief CAN0发送数据
 * @param uint32_t ID ID号
 * @param uint8_t *data 数据地址
 * @param uint8_t len 数据长度
 * @return none
 */
void CAN0_TransmitData(uint32_t ID, uint8_t *data, uint8_t len){
    can_trasnmit_message_struct CAN_Transmit_StructInit;
    CAN_Transmit_StructInit.tx_ff = CAN_FF_STANDARD;
    CAN_Transmit_StructInit.tx_ft = CAN_FT_DATA;
    CAN_Transmit_StructInit.tx_sfid = ID;
    CAN_Transmit_StructInit.tx_efid = ID;
    for(uint8_t i=0;i<len;i++){
        CAN_Transmit_StructInit.tx_data[i] = data[i];    
    }
    CAN_Transmit_StructInit.tx_dlen = len;
    
    can_message_transmit(CAN0, &CAN_Transmit_StructInit);

}


/**
 * @brief CAN0接收标志位检测
 * @param uint8_t fifo_num FIFO号，可为CAN_FIFO0/CAN_FIFO1
 * @return 1 or 0
 */
uint8_t CAN0_ReceiveData_Flag(uint8_t fifo_num){
    if(can_receive_message_length_get(CAN0, fifo_num) > 0){
        return 1;
    }
    return 0;
}

/**
 * @brief CAN0接收数据
 * @param can_receive_message_struct* receive_message 接收数据结构体
 * @return none
 */
void CAN0_ReceiveData(can_receive_message_struct* receive_message){
    can_message_receive(CAN0, CAN_FIFO0, receive_message);
}

/**
 * @brief CAN0接收中断服务函数
 */
void USBD_LP_CAN0_RX0_IRQHandler(void){
    if (can_interrupt_flag_get(CAN0, CAN_INT_FLAG_RFL0) == SET){
        can_message_receive(CAN0, CAN_FIFO0, &CAN0_receive_message);
        if(CAN0_receive_message.rx_ff == CAN_FF_STANDARD && CAN0_receive_message.rx_ft == CAN_FT_DATA &&
            CAN0_receive_message.rx_sfid == ControlID && CAN0_receive_message.rx_dlen == 8 && CAN0_receive_message.rx_fi == 0
        ){
            uint8_t data[8]={0};
            memcpy(data,CAN0_receive_message.rx_data,CAN0_receive_message.rx_dlen);
            CAN_Parse(data);
        }
        
    }
}

void CAN_Parse(uint8_t *data){
    const uint8_t *order = &data[MOTOR_LEG_WHEEL * 4];
    uint8_t mode = (order[0] >> 5);
    if(mode > 4) {
        error_type = CAN_Mode_Set_error;
        return;
    }
    if (motor_mode != (MOTOR_Mode)mode) {
        Change_once = 1;//改一次模式，执行一次FOC_CloseOut()/FOC_OpenOut()
        PID_Reset(&position_pid);//改一次模式 PID复位
        PID_Reset(&velocity_pid);
        PID_Reset(&Iq_pid);
        PID_Reset(&Id_pid);
    }

    uint32_t raw = ((uint32_t)(order[0] & 0x1F) << 24)
            | ((uint32_t)order[1] << 16)
            | ((uint32_t)order[2] << 8)
            | ((uint32_t)order[3]);

    int32_t q = (int32_t)raw;
    if (raw & 0x10000000u)
        q -= 0x20000000;

    float controldata = Decode_Q9_19(q);

    motor_mode = (MOTOR_Mode)mode;
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
    
}

int32_t Encode_Q9_19(float value)
{
    if (value >= 512.0f) return 268435455;      // 29位补码最大正数
    if (value <= -512.0f) return -268435456;     // 29位补码最小负数
    return (int32_t)(value * Q19_SCALE);
}

float Decode_Q9_19(int32_t value){
    return ((float)value / Q19_SCALE);
}
