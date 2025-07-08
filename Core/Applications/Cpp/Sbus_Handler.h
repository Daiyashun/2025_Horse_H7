#ifndef SBUS_HANDLER_H
#define SBUS_HANDLER_H
#include <cstdint>
#include "UART_DMA.h"


class SBUS
{
public:
    uint8_t Sbus_Receive_Buffer[25];
    uint8_t Sbus_Check_Buffer[25];
    uint16_t Sbus_Channel[16];
    uint8_t Sbus_Check_False_Num;
    uint8_t Sbus_Check_False_Time;

    void Sbus_Uart_Receive_init(UART_HandleTypeDef *huart);
    void Sbus_Uart_Receive_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25]);
    void Sbus_Data_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25]);
    uint8_t Sbus_Data_Check();
    void Sbus_Data_Trans(uint8_t buffer[23]);
};

// #define SBUS_FORWARDandBACK_CHANNEL1
// #define SBUS_LEFTandRIGHT_CHANNEL2
// #define SBUS_FORWARDandBACK_CHANNEL3

#endif //SBUS_HANDLER_H
