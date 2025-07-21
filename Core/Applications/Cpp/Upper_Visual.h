//
// Created by 27713 on 25-3-15.
//
#ifndef UPPER_VISUAL_H
#define UPPER_VISUAL_H
#include "FDCAN_receive_dm.h"
#include "IMU_N300WP.h"

class Upper_data_receive
{
public:
    float Visual_motor_receive_tor[12];
    float Last_data[12];
    float Visual_motor_receive_pos[12];
    uint8_t Real_data[54];
    bool Visual_Enable_Flag;
    bool Visual_Receive_Flag;
    void Vdata_get(uint8_t* data, uint8_t length);
    float Vdata_transfer(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
    void Vdata_send(void);
    void Angle_transfer(void);
    void Tor_transfer(void);
};

class Upper_data_send
{
public:
    float Visual_imu[10];
    float Visual_motor_send[3][4][3];     //3个FDCAN，每个FDCAN带四个电机，每个电机3个参数,共计36个参数
    float Reactive_Force[4];
    float All_data[50];
    float Reactive_Force_Cal(float data1,float data2,float data3,float data4);
    void All_Data_get(void);
    void All_Data_send(UART_HandleTypeDef *huart);
};

#endif //UPPER_VISUAL_H
