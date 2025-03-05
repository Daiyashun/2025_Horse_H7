//
// Created by 27713 on 25-2-21.
//

#include "IMU_N300WP.h"
#include <math.h>




bool IMU_N300WP::data_check(uint8_t* data)
{
    if(data[0] == 0xFC && last_num == 0xFD)  //帧头帧尾校验
    {
        //根据指令类型和数据长度判断数据类型
        if((data[1] == TYPE_IMU) && (data[2] == IMU_LEN)) imu_flag = 1;
        if((data[1] == TYPE_AHRS) && (data[2] == AHRS_LEN)) ahrs_flag = 1;
        return true;
    }
    last_num = data[sizeof(data) - 1]; //记录上一组数据最后一位数，判断帧尾
    return false;
}

void IMU_N300WP::data_get(uint8_t* data)
{

    //将本帧中的数据存储到uint8_t的存储数组中
    if(imu_flag == 1)
    {
        for(int i = 0; i < sizeof(data); i++)
        {
            RX_IMU[i] = data[i];
        }

        imu_flag = 0;   //重置标志位
    }
    if(ahrs_flag == 1)
    {
        for(int i = 0; i < sizeof(data); i++)
        {
            RX_AHRS[i] = data[i];
        }
        RollSpeed = data_trans(RX_AHRS[7],RX_AHRS[8],RX_AHRS[9],RX_AHRS[10]);
        PitchSpeed = data_trans(RX_AHRS[11],RX_AHRS[12],RX_AHRS[13],RX_AHRS[14]);
        YawSpeed = data_trans(RX_AHRS[15],RX_AHRS[16],RX_AHRS[17],RX_AHRS[18]);
        Roll = data_trans(RX_AHRS[19],RX_AHRS[20],RX_AHRS[21],RX_AHRS[22]);
        Pitch = data_trans(RX_AHRS[23],RX_AHRS[24],RX_AHRS[25],RX_AHRS[26]);
        Yaw = data_trans(RX_AHRS[27],RX_AHRS[28],RX_AHRS[29],RX_AHRS[30]);
        Qw = data_trans(RX_AHRS[31],RX_AHRS[32],RX_AHRS[33],RX_AHRS[34]);
        Qx = data_trans(RX_AHRS[35],RX_AHRS[36],RX_AHRS[37],RX_AHRS[38]);
        Qy = data_trans(RX_AHRS[39],RX_AHRS[40],RX_AHRS[41],RX_AHRS[42]);
        Qz = data_trans(RX_AHRS[43],RX_AHRS[44],RX_AHRS[45],RX_AHRS[46]);
        ahrs_flag = 0;  //重置标志位
    }
}

float IMU_N300WP::data_trans(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
{
    long long transition_32;
    float tmp=0;
    float last_tmp=0;
    int sign=0;
    int exponent=0;
    float mantissa=0;
    transition_32 = 0;
    transition_32 |= d4<<24; //得到数据的底 8 位
    transition_32 |= d3<<16;
    transition_32 |= d2<<8;
    transition_32 |= d1; //得到数据的高 8 位
    sign = (transition_32 & 0x80000000) ? -1 : 1;//符号位
    //先右移操作，再按位与计算，出来结果是 30 到 23 位对应的 e
    exponent = ((transition_32 >> 23) & 0xff) - 127;
    //将 22~0 转化为 10 进制，得到对应的 x 系数
    mantissa = 1 + ((float)(transition_32 & 0x7fffff) / 0x7fffff);
    tmp = sign * mantissa;
    tmp = tmp * pow(2, exponent);
    last_tmp = tmp;
    return last_tmp;
}

