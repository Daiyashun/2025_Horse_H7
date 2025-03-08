//
// Created by 27713 on 25-3-6.
//

#ifndef CAN_BSP_H
#define CAN_BSP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
extern uint8_t rx_data1[8];
extern uint8_t rx_data2[8];
extern uint8_t rx_data3[8];

void can_bsp_init(void);
void can_filter_init(void);
uint8_t fdcanx_send_data(FDCAN_HandleTypeDef *hfdcan, uint16_t id, uint8_t *data, uint32_t len);
uint8_t fdcanx_receive(FDCAN_HandleTypeDef *hfdcan, uint16_t *rec_id, uint8_t *buf);
void fdcan1_rx_callback(void);
void fdcan2_rx_callback(void);
void fdcan3_rx_callback(void);

#ifdef __cplusplus
}
#endif
#endif //CAN_BSP_H
