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

    float Sbus_Data_Velocity_ForwardBackward;
    float Sbus_Data_Velocity_ForwardBackward_Max;
    float Sbus_Data_Velocity_ForwardBackward_Min;
    float Sbus_Data_Velocity_RightLeft;
    float Sbus_Data_Velocity_RightLeft_Max;
    float Sbus_Data_Velocity_RightLeft_Min;
    float Sbus_Data_Turn_Yaw;
    float Sbus_Data_Turn_Yaw_Max;
    float Sbus_Data_Turn_Yaw_Min;

    int Sbus_Data_WorkMode;
    float Sbus_Data_WalkMode;
    bool Sbus_JumpMode_flag;
    bool Sbus_JumpStart_flag;

#define SBUS_CHANNEL_MAX 1810
#define SBUS_CHANNEL_MIN 172

#define SBUS_WorkMoode_Paralysis 0
#define SBUS_WorkMoode_Stand 100
#define SBUS_WorkMoode_Walk 1000
#define SBUS_WalkMoode_Walk 0
#define SBUS_WalkMoode_Grovel 100
#define SBUS_Jump_Enable_Flag 1
#define SBUS_Jump_Disable_Flag 0
#define SBUS_Jump_Start_Flag 1

#define SBUS_FORWARDandBACK_RightHand_Y Sbus_Channel[2 - 1]
#define SBUS_LEFTandRIGHT_RightHand_X Sbus_Channel[1 - 1]
#define SBUS_TURNYAW__LeftHand_Y Sbus_Channel[4 - 1]
#define SBUS_WalkMoodeChoose_SA Sbus_Channel[5 - 1]
#define SBUS_WorkModeChoose_SB Sbus_Channel[6 - 1]
#define SBUS_Jump_ENABLE_FLAG_SE Sbus_Channel[9 - 1]
#define SBUS_Jump_START_FLAG_SF Sbus_Channel[10 - 1]

    void Sbus_Uart_Receive_init(UART_HandleTypeDef *huart);
    void Sbus_Uart_Receive_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25]);
    void Sbus_OriginData_Handler(UART_HandleTypeDef* huart,uint8_t buffer[25]);
    uint8_t Sbus_OriginData_Check(void);
    void Sbus_OriginData_Trans(uint8_t buffer[23]);

    void Sbus_Data_Init(void);
    float Sbus_Data_Transform(uint16_t SBUS_channel, float min, float max);
    float Sbus_Data_WorkModeChoose(float Sbus_Channel);
    float Sbus_Data_WalkModeChoose(float Sbus_Channel);
    bool Sbus_Data_JumpModeChoose(float Sbus_Channel);
    bool Sbus_Data_Jump(float Sbus_Channel);

    void Sbus_Data_Update(void);
};



#endif //SBUS_HANDLER_H
