//
// Created by 27713 on 25-2-21.
//

#ifndef IMU_N300WP_H
#define IMU_N300WP_H
#include <sys/_stdint.h>
#include "UART_DMA.h"



#define TYPE_IMU 0x40   //经过校准的IMU数据
#define TYPE_AHRS 0x41  //AHRS数据
#define IMU_LEN 64      //IMU数据长度
#define AHRS_LEN 56     //AHRS数据长度

class IMU_N300WP
{
public:
    uint8_t RX_IMU[IMU_LEN];       //MSG_IMU数据存储数组
    uint8_t RX_AHRS[AHRS_LEN];      //MSG_AHRS数据存储数组
    uint8_t Fd_data[80];    //接收数组
    uint8_t last_num;       //上一帧数据的帧尾
    float RollSpeed;        //横滚角速度
    float PitchSpeed;       //俯仰角速度
    float YawSpeed;         //偏航角速度
    float Roll;             //横滚角
    float Pitch;            //俯仰角
    float Yaw;              //偏航角
    float Qw;            //四元数
    float Qx;            //四元数
    float Qy;            //四元数
    float Qz;            //四元数
    bool imu_flag;          //IMU数据接收标志位
    bool ahrs_flag;         //AHRS数据接收标志位
    bool data_check(uint8_t *data);      //接收数组有效性判断函数
    float data_trans(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
    void data_get(uint8_t *data);       //数据接收与转化
};



#endif //IMU_N300WP_H
