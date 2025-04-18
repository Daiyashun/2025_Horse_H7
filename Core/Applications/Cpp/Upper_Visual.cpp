//
// Created by 27713 on 25-3-15.
//

#include "Upper_Visual.h"

#include "vofa_setting.h"

extern motor_t motor[12];

float Upper_data_receive::Vdata_transfer(uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4)
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

void Upper_data_receive::Vdata_get(uint8_t* data, uint8_t length)
{
    for(int i = 0; i < length; i++)
    {
        if((data[i]==0x7f) && (data[i-1] == 0x80) && (data[i-2] == 0x00) && (data[i-3] == 0x00))
        {
            Visual_Receive_Flag = 0;
            for(int i = 0; i < ((12 * 4) + 4) ; i++)
            {
                Real_data[i] = data[i];
            }
            int j = 0;
            int k = 0;
            while(j < 12)
            {
#if USE_DYF
                Visual_motor_receive_pos[j] = Vdata_transfer(Real_data[k],Real_data[k + 1],Real_data[k + 2],Real_data[k + 3]);
#else
                Visual_motor_receive_tor[j] = Vdata_transfer(Real_data[k],Real_data[k + 1],Real_data[k + 2],Real_data[k + 3]);
#endif

                j ++;
                k += 4;
            }
            Visual_Receive_Flag = 1;
        }
    }
}

void Upper_data_receive::Vdata_send()
{

    if (Visual_Receive_Flag)
    {
#if USE_DYF


#else
        for(int i = 0; i < 12; i++)
        {
            motor[i].send.tor = Visual_motor_receive_tor[i];
            Tor_transfer();
#endif
        }
    }

}

void Upper_data_receive::Angle_transfer()
{
    for (int i = 1; i < 13; i++)
    {
        switch (i)
        {
            case CAN_DM_M1_ID:

                break;

            case CAN_DM_M2_ID:
                break;

            case CAN_DM_M3_ID:
                break;

            case CAN_DM_M4_ID:

                break;

            case CAN_DM_M5_ID:

                break;

            case CAN_DM_M6_ID:

                break;

            case CAN_DM_M7_ID:

                break;

            case CAN_DM_M8_ID:

                break;

            case CAN_DM_M9_ID:

                break;

            case CAN_DM_M10_ID:

                break;

            case CAN_DM_M11_ID:

                break;

            case CAN_DM_M12_ID:

                break;

            default:break;
        }
    }
}

void Upper_data_receive::Tor_transfer()
{
    for (int i = 1; i < 13; i++)
    {
        switch (i)
        {
            case CAN_DM_M1_ID:

                break;

            case CAN_DM_M2_ID:

                break;

            case CAN_DM_M3_ID:

                break;

            case CAN_DM_M4_ID:

                break;

            case CAN_DM_M5_ID:

                break;

            case CAN_DM_M6_ID:

                break;

            case CAN_DM_M7_ID:

                break;

            case CAN_DM_M8_ID:

                break;

            case CAN_DM_M9_ID:

                break;

            case CAN_DM_M10_ID:

                break;

            case CAN_DM_M11_ID:

                break;

            case CAN_DM_M12_ID:

                break;

            default:break;
        }
    }
}


float Upper_data_send::Reactive_Force_Cal(float data1, float data2, float data3, float data4)
{
    return (3.89281 * data1)/(sinf(data2 + data3) * cosf(data4));
}

void Upper_data_send::All_Data_get()
{
#if USE_DYF

#else
    //首先是12个电机的参数，共36个
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                All_data[i * 4 + j * 3 + k] = Visual_motor_send[i][j][k];
            }
        }
    }

    //其次是IMU的数据，共10个
    for(int i = 36; i < 46; i++)
    {
        All_data[i] = Visual_imu[i - 36];
    }

    //最后是地面反作用力。共4个
    for(int i = 0; i < 4; i++)
    {
        Reactive_Force[i] = Reactive_Force_Cal(motor[3 * i + 2].receive.toq, PI - motor[3 * i].receive.pos, motor[3 * i + 2].receive.pos,motor[3 * i + 1].receive.pos);
    }
    for(int i = 46; i < 50; i++)
    {
        All_data[i] = Reactive_Force[i - 46];
    }

    for (int i = 0; i < 50; i++)
    {
        tempFloat[i] = All_data[i];
    }
#endif
}

void Upper_data_send::All_Data_send()
{

}


