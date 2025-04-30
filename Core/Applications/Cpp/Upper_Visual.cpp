//
// Created by 27713 on 25-3-15.
//

#include "Upper_Visual.h"
#include "Main.h"
#include <cstring>

#include "vofa_setting.h"
float Receive_data[12];
extern motor_t motor[12];
void Stand()
{
    motor[0].send.pos = -0.2f;
    motor[3].send.pos = 0.2f;
    motor[6].send.pos = -0.2f;
    motor[9].send.pos = 0.2f;

    motor[1].send.pos = 0.9f;
    motor[4].send.pos = 0.9f;
    motor[7].send.pos = 0.9f;
    motor[10].send.pos = 0.9f;

    motor[2].send.pos = -1.35f;
    motor[5].send.pos = -1.35f;
    motor[8].send.pos = -1.35f;
    motor[11].send.pos = -1.35f;
}
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
        if((data[i + 51]==0x7f) && (data[i + 50] == 0x80) && (data[i + 49] == 0x00) && (data[i + 48] == 0x00))
        {
            Visual_Receive_Flag = 0;
            for(int cnt = 0; cnt < ((12 * 4) + 4) ; cnt++)
            {
                Real_data[cnt] = data[i + cnt];
            }
            int j = 0;
            int k = 0;
            while(j < 12)
            {
#if USE_DYF
                Visual_motor_receive_pos[j] = Vdata_transfer(Real_data[k],Real_data[k + 1],Real_data[k + 2],Real_data[k + 3]);
                tempFloat[j + 62] = Visual_motor_receive_pos[j];
#else
               //Receive_data[j] = Vdata_transfer(Real_data[k],Real_data[k + 1],Real_data[k + 2],Real_data[k + 3]);
               Visual_motor_receive_tor[j] = Vdata_transfer(Real_data[k],Real_data[k + 1],Real_data[k + 2],Real_data[k + 3]);
                tempFloat[j + 62] = Visual_motor_receive_tor[j];
#endif
                j ++;
                k += 4;
            }
            Visual_Receive_Flag = 1;
        }
        // Receive_data[0] = 2.0f/10;//+
        // Receive_data[1] = 1.5f;
        // Receive_data[2] = 6.0f/10;
        // Receive_data[3] = -2.0f/10;//-
        // Receive_data[4] = 1.5f;
        // Receive_data[5] = 6.0f/10;
        // Receive_data[6] = 2.0f/10;//+
        // Receive_data[7] = 2.0f;
        // Receive_data[8] = 10.0f/10;
        // Receive_data[9] = -2.0f/10;//-
        // Receive_data[10] = 2.0f;
        // Receive_data[11] = 10.0f/10;
    }
}

void Upper_data_receive::Vdata_send()
{
    if (VofaSlider[1] == 1)
    {
        Stand();
        if (Visual_Receive_Flag)
        {

        }
        else
        {
            Angle_transfer();
        }
    }
    else
    {
        for (int i = 0; i < 12; i++)
        {
            motor[i].send.pos = 0;
        }
    }

  if (Visual_Receive_Flag)
    {
#if USE_DYF
        motor[3].send.pos = Visual_motor_receive_pos[0];
        motor[4].send.pos = Visual_motor_receive_pos[1];
        motor[5].send.pos = Visual_motor_receive_pos[2];
        motor[0].send.pos = Visual_motor_receive_pos[3];
        motor[1].send.pos = Visual_motor_receive_pos[4];
        motor[2].send.pos = Visual_motor_receive_pos[5];
        motor[9].send.pos = Visual_motor_receive_pos[6];
        motor[10].send.pos = Visual_motor_receive_pos[7];
        motor[11].send.pos = Visual_motor_receive_pos[8];
        motor[6].send.pos = Visual_motor_receive_pos[9];
        motor[7].send.pos = Visual_motor_receive_pos[10];
        motor[8].send.pos = Visual_motor_receive_pos[11];
        Angle_transfer();
#else

        for(int i = 0; i < 12; i++)
        {
            //motor[i].send.tor = Receive_data[i];
            motor[i].send.tor = Visual_motor_receive_tor[i];
        }
        Tor_transfer();
#endif

    }

}

void Upper_data_receive::Angle_transfer()
{
    for (int i = 1; i < 13; i++)
    {
        switch (i)
        {
            case CAN_DM_M1_ID:
                motor[CAN_DM_M1_ID - 1].send.pos += MOTOR_147A_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M2_ID:
                motor[CAN_DM_M2_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                motor[CAN_DM_M2_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M3_ID:
                motor[CAN_DM_M3_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                motor[CAN_DM_M3_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M4_ID:
                motor[CAN_DM_M4_ID - 1].send.pos -= MOTOR_147A_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M5_ID:
                motor[CAN_DM_M5_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M6_ID:
                motor[CAN_DM_M6_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M7_ID:
                motor[CAN_DM_M7_ID - 1].send.pos += MOTOR_147A_ANGLE_OFFSET;
                motor[CAN_DM_M7_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M8_ID:
                motor[CAN_DM_M8_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                motor[CAN_DM_M8_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M9_ID:
                motor[CAN_DM_M9_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                motor[CAN_DM_M9_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M10_ID:
                motor[CAN_DM_M10_ID - 1].send.pos -= MOTOR_147A_ANGLE_OFFSET;
                motor[CAN_DM_M10_ID - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M11_ID:
                motor[CAN_DM_M11_ID - 1].send.pos -= MOTOR_258B_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            case CAN_DM_M12_ID:
                motor[CAN_DM_M12_ID - 1].send.pos += MOTOR_369C_ANGLE_OFFSET;
                //motor[i - 1].send.pos *= DIRECTION_CORRECTION;
                break;

            default:break;
        }
    }
}

void Upper_data_receive::Tor_transfer()
{
    // for (int i = 1; i < 13; i++)
    // {
    //     switch (i)
    //     {
    //         case CAN_DM_M1_ID:
    //             //motor[CAN_DM_M1_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M2_ID:
    //             motor[CAN_DM_M2_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M3_ID:
    //             motor[CAN_DM_M3_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M4_ID:
    //             //motor[CAN_DM_M4_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M5_ID:
    //             //motor[CAN_DM_M5_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M6_ID:
    //             //motor[CAN_DM_M6_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M7_ID:
    //             motor[CAN_DM_M7_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M8_ID:
    //             motor[CAN_DM_M8_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M9_ID:
    //             //motor[CAN_DM_M9_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M10_ID:
    //             motor[CAN_DM_M10_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M11_ID:
    //             //motor[CAN_DM_M11_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         case CAN_DM_M12_ID:
    //             motor[CAN_DM_M12_ID - 1].send.tor *= DIRECTION_CORRECTION;
    //             break;
    //
    //         default:break;
    //     }
    // }
    motor[CAN_DM_M2_ID - 1].send.tor *= DIRECTION_CORRECTION;
    motor[CAN_DM_M3_ID - 1].send.tor *= DIRECTION_CORRECTION;
    motor[CAN_DM_M7_ID - 1].send.tor *= DIRECTION_CORRECTION;
    motor[CAN_DM_M8_ID - 1].send.tor *= DIRECTION_CORRECTION;
    motor[CAN_DM_M10_ID - 1].send.tor *= DIRECTION_CORRECTION;
    motor[CAN_DM_M9_ID - 1].send.tor *= DIRECTION_CORRECTION;

    // motor[CAN_DM_M2_ID - 1].send.tor *= 2;
    // motor[CAN_DM_M5_ID - 1].send.tor *= 2;
    // motor[CAN_DM_M8_ID - 1].send.tor *= 2;
    // motor[CAN_DM_M11_ID - 1].send.tor *= 2;
}


float Upper_data_send::Reactive_Force_Cal(float data1, float data2, float data3, float data4)
{
    return (3.89281 * data1)/(sinf(data2 + data3) * cosf(data4));
}

void Upper_data_send::All_Data_get()
{
#if USE_DYF
    All_data[0] = motor[3].receive.pos;
    All_data[1] = motor[4].receive.pos;
    All_data[2] = motor[5].receive.pos;
    All_data[3] = motor[0].receive.pos;
    All_data[4] = motor[1].receive.pos;
    All_data[5] = motor[2].receive.pos;
    All_data[6] = motor[9].receive.pos;
    All_data[7] = motor[10].receive.pos;
    All_data[8] = motor[11].receive.pos;
    All_data[9] = motor[6].receive.pos;
    All_data[10] = motor[7].receive.pos;
    All_data[11] = motor[8].receive.pos;
    All_data[12] = motor[3].receive.speed;
    All_data[13] = motor[4].receive.speed;
    All_data[14] = motor[5].receive.speed;
    All_data[15] = motor[0].receive.speed;
    All_data[16] = motor[1].receive.speed;
    All_data[17] = motor[2].receive.speed;
    All_data[18] = motor[9].receive.speed;
    All_data[19] = motor[10].receive.speed;
    All_data[20] = motor[11].receive.speed;
    All_data[21] = motor[6].receive.speed;
    All_data[22] = motor[7].receive.speed;
    All_data[23] = motor[8].receive.speed;

    All_data[24] = Visual_imu[1];
    All_data[25] = Visual_imu[2];
    All_data[26] = Visual_imu[3];
    All_data[27] = Visual_imu[0];
    All_data[28] = Visual_imu[4];
    All_data[29] = Visual_imu[5];
    All_data[30] = Visual_imu[6];
#else
    //首先是12个电机的参数，共36个
    for(int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            for (int k = 0; k < 3; k++)
            {
                All_data[i * 4 * 3 + j * 3 + k] = Visual_motor_send[i][j][k];
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
        Reactive_Force[i] = -Reactive_Force_Cal(motor[3 * i + 2].receive.toq, motor[3 * i].receive.pos,PI -  motor[3 * i + 2].receive.pos,motor[3 * i + 1].receive.pos);
    }
    for(int i = 46; i < 50; i++)
    {
        All_data[i] = Reactive_Force[i - 46];
    }
#endif
    for (int i = 0; i < 50; i++)
    {
        tempFloat[i] = All_data[i];
    }

}

void Upper_data_send::All_Data_send(UART_HandleTypeDef *huart)
{
#if USE_DYF
    static uint8_t  tempData[DATA_LENGTH_DYF];
    static uint8_t  tempData1[DATA_LENGTH_DYF + 4];
    memcpy(tempData, (uint8_t *)All_data, DATA_LENGTH_DYF);
    tempData[DATA_LENGTH_DYF-4] = 0x00;
    tempData[DATA_LENGTH_DYF-3] = 0x00;
    tempData[DATA_LENGTH_DYF-2] = 0x80;
    tempData[DATA_LENGTH_DYF-1] = 0x7f;
    for(int i = 4; i < DATA_LENGTH_DYF + 4; i++)
    {
        tempData1[i] = tempData[i-4];
    }
    tempData1[0] = 0xef;
    tempData1[1] = 0xff;
    tempData1[2] = 0xff;
    tempData1[3] = 0xff;
#if VOFA_USE_DMA
    HAL_UART_Transmit_DMA(huart, (uint8_t *)tempData1, DATA_LENGTH_DYF + 4);
#else
    HAL_UART_Transmit(huart, (uint8_t *)tempData, len,0xff);
#endif

#else
    static uint8_t  tempData[DATA_LENGTH];
    static uint8_t  tempData1[DATA_LENGTH + 4];
    memcpy(tempData, (uint8_t *)All_data, DATA_LENGTH);
    tempData[DATA_LENGTH-4] = 0x00;
    tempData[DATA_LENGTH-3] = 0x00;
    tempData[DATA_LENGTH-2] = 0x80;
    tempData[DATA_LENGTH-1] = 0x7f;
    for(int i = 4; i < DATA_LENGTH + 4; i++)
    {
        tempData1[i] = tempData[i-4];
    }
    tempData1[0] = 0xef;
    tempData1[1] = 0xff;
    tempData1[2] = 0xff;
    tempData1[3] = 0xff;
#if VOFA_USE_DMA
    HAL_UART_Transmit_DMA(huart, (uint8_t *)tempData1, DATA_LENGTH + 4);
#else
    HAL_UART_Transmit(huart, (uint8_t *)tempData, len,0xff);
#endif

#endif
}


