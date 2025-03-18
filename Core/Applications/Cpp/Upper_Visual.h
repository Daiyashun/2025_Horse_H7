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
    motor_t Visual_motor_receive[12];
    void Vdata_get(void);
    void Vdata_transfer(void);
};

class Upper_data_send
{
public:
    IMU_N300WP Visual_imu;
    motor_t Visual_motor_send;
    void Low_Data_get(void);
    void Low_Data_send(void);
};

#endif //UPPER_VISUAL_H
