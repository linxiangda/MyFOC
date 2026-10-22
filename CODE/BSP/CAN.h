#ifndef _CAN_H
#define _CAN_H
#include "main.h"

//CAN
#define CAN_TX_PIN   GPIO_PIN_12
#define CAN_RX_PIN   GPIO_PIN_11
#define CAN_PORT     GPIOA

void CAN0_Init(void);
uint8_t CAN0_TransmitData(uint32_t ID,uint8_t *data,uint8_t len);
uint8_t CAN0_ReceiveData_Flag(uint8_t fifo_num);
void CAN0_ReceiveData(can_receive_message_struct* receive_message);
void USBD_LP_CAN0_RX0_IRQHandler(void);

int32_t Encode_Q9_19(float value);
float Decode_Q9_19(int32_t value);

#endif
