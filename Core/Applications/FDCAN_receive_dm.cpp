/**
  ****************************(C)SWJTU_ROBOTCON2025 马术****************************
  * @file       FDCAN_receive_dm.c/h
  * @brief      
  *             这里是FDCAN中断接收函数，接收电机数据,FDCAN发送函数发送电机电流控制电机.
  * @note       
  * @history
  *  Version    Date            Author          Modification
  *  V1.0.0     Dec-26-2018     RM              1. done
  *  V2.0.0     ***-**-2022     ZDYUKINO        加入各类can通信函数 CAN1控制底盘与上层电机通信
                                                CAN2控制sbus数据接收与miss电机通信
                                                带码盘通信函数（未启用）
*  V3.0.0       ***-**-2025     DYS             修改为使用FDCAN
  @verbatim
  ==============================================================================
  ==============================================================================
  @endverbatim
  ****************************(C)SWJTU_ROBOTCON2025 马术****************************
  */

#include "FDCAN_receive_dm.h"
#include "main.h"
#include "fdcan.h"
#include "vofa_setting.h"
#include "bsp_mc02/can_bsp.h"
#include "Cpp/main.h"
#include "Cpp/Upper_Visual.h"

//#include "state_control.h"
//#include "leg_control.h"

motor_t motor[12];
extern Upper_data_receive Vdata_Rx;
extern Upper_data_send Vdata_Tx;
//DM_measure_t DM_Motor_measure[8];//电机数据结构体定义
/**
  * @brief          float转int 带限幅
  * @param[in]      x：输入值
  * @param[in]      x_min:最小限幅
  * @param[in]      x_max:最大限幅
  * @param[in]      bits:位
  * @retval         返回值
  */
int float_to_uint(float x, float x_min, float x_max, int bits){
    /// Converts a float to an unsigned int, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return (int) ((x-offset)*((float)((1<<bits)-1))/span);
}
/**
  * @brief          uint转float 带限幅
  * @param[in]      x_int：输入值
  * @param[in]      x_min:最小限幅
  * @param[in]      x_max:最大限幅
  * @param[in]      bits:位
  * @retval         返回值
  */
float uint_to_float(int x_int, float x_min, float x_max, int bits){
    /// converts unsigned int to float, given range and number of bits ///
    float span = x_max - x_min;
    float offset = x_min;
    return ((float)x_int)*span/((float)((1<<bits)-1)) + offset;
}

/**
  * @brief          DM电机回传参数转换
  * @param[in]      ptr：电机数据结构体指针
  * @param[in]      Data：数据指针
  * @retval         None
  */
void get_moto_measure(DM_measure_t *ptr, uint8_t *Data)
{
    ptr->id = (Data[0])&0x0F;
    ptr->state = (Data[0])>>4;
    ptr->int_p= ( Data[1] << 8) | Data[2];
    ptr->int_v= ( Data[3] << 4) | (Data[4] >> 4);
    ptr->int_t= ((Data[4] & 0xF) << 8) | Data[5];
    ptr->pos = uint_to_float(ptr->int_p, P_MIN, P_MAX, 16); // (-12.5,12.5)
    ptr->speed = uint_to_float(ptr->int_v, V_MIN, V_MAX, 12); // (-45.0,45.0)
    ptr->toq = uint_to_float(ptr->int_t, T_MIN, T_MAX, 12); // (-18.0,18.0)
    ptr->T_mos  = (float)(Data[6]);
    ptr->T_coil = (float)(Data[7]);
}
/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */

/**
  * @brief          hal库CAN回调函数,接收电机数据
  * @param[in]      hcan:CAN句柄指针
  * @retval         none
  */

/**
  * @brief          MIT协议控制DM电机
  * @param[in]      hcan    :CAN接口
  * @param[in]      id      :DM电机ID
  * @param[in]      _pos    :位置设置(rad)
  * @param[in]      _vel    :速度设置(rad/s)
  * @param[in]      _KP     :位置P
  * @param[in]      _KD     :位置D
  * @param[in]      _torq   :力矩
  * @retval         none
  */
void MIT_motor_CTRL(FDCAN_HandleTypeDef *hfdcan,uint16_t id, float _pos, float _vel, float _KP, float _KD, float _torq)
{
#if SIMULATE_MODE_ALL
    _pos =0;_vel =0;_KP =0;_KD =0;_torq=0;
#endif

    uint16_t pos_tmp,vel_tmp,kp_tmp,kd_tmp,tor_tmp;  //数据格式转换
    pos_tmp = float_to_uint(_pos, P_MIN, P_MAX, 16);
    vel_tmp = float_to_uint(_vel, V_MIN, V_MAX, 12);
    kp_tmp  = float_to_uint(_KP, KP_MIN, KP_MAX, 12);
    kd_tmp  = float_to_uint(_KD, KD_MIN, KD_MAX, 12);
    tor_tmp = float_to_uint(_torq, T_MIN, T_MAX, 12);

    uint8_t              DM_can_send_data[8];  //发送数组
    DM_can_send_data[0] = (pos_tmp >> 8);
    DM_can_send_data[1] = pos_tmp;
    DM_can_send_data[2] = (vel_tmp >> 4);
    DM_can_send_data[3] = ((vel_tmp&0xF)<<4)|(kp_tmp>>8);
    DM_can_send_data[4] = kp_tmp;
    DM_can_send_data[5] = (kd_tmp >> 4);
    DM_can_send_data[6] = ((kd_tmp&0xF)<<4)|(tor_tmp>>8);
    DM_can_send_data[7] = tor_tmp;
    fdcanx_send_data(hfdcan, id, DM_can_send_data, FDCAN_DLC_BYTES_8);   //发送
}
/**
  * @brief          返回电机数据指针
  * @param[in]      i: 电机编号,范围[0,8]
  * @retval         电机数据指针
  */
//const DM_measure_t *get_motor_measure_point(uint8_t i)
//{
//    return &DM_Motor_measure[(i&0x07)];//111B
//}
/**
  * @brief          使能DM电机
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void start_motor(FDCAN_HandleTypeDef *hfdcan,uint16_t id)
{
    uint8_t  DM_can_send_data[8];  //发送数组
    DM_can_send_data[0] = 0xFF;
    DM_can_send_data[1] = 0xFF;
    DM_can_send_data[2] = 0xFF;
    DM_can_send_data[3] = 0xFF;
    DM_can_send_data[4] = 0xFF;
    DM_can_send_data[5] = 0xFF;
    DM_can_send_data[6] = 0xFF;
    DM_can_send_data[7] = 0xFC;
    fdcanx_send_data(hfdcan, id, DM_can_send_data, FDCAN_DLC_BYTES_8);   //发送
}
/**
  * @brief          失能DM电机
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void lock_motor(FDCAN_HandleTypeDef *hfdcan,uint16_t id)
{
    FDCAN_TxHeaderTypeDef  DM_tx_message;        //发送CAN结构体
    uint8_t              DM_can_send_data[8];  //发送数组
    DM_can_send_data[0] = 0xFF;
    DM_can_send_data[1] = 0xFF;
    DM_can_send_data[2] = 0xFF;
    DM_can_send_data[3] = 0xFF;
    DM_can_send_data[4] = 0xFF;
    DM_can_send_data[5] = 0xFF;
    DM_can_send_data[6] = 0xFF;
    DM_can_send_data[7] = 0xFD;
    fdcanx_send_data(hfdcan, id, DM_can_send_data, FDCAN_DLC_BYTES_8);   //发送
}
/**
  * @brief          设置电机零点
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void set_zero_motor(FDCAN_HandleTypeDef *hfdcan,uint16_t id)
{
    FDCAN_TxHeaderTypeDef  DM_tx_message;        //发送CAN结构体
    uint8_t              DM_can_send_data[8];  //发送数组
    DM_can_send_data[0] = 0xFF;
    DM_can_send_data[1] = 0xFF;
    DM_can_send_data[2] = 0xFF;
    DM_can_send_data[3] = 0xFF;
    DM_can_send_data[4] = 0xFF;
    DM_can_send_data[5] = 0xFF;
    DM_can_send_data[6] = 0xFF;
    DM_can_send_data[7] = 0xFE;
    fdcanx_send_data(hfdcan, id, DM_can_send_data, FDCAN_DLC_BYTES_8);   //发送
}
void mit_send_in_TIM(void) {
#if SIMULATE_MODE
    limit();
    PD_Send();
    for (int j = 0; j < 12; j++)
    {
        tempFloat[j + 50] = motor[j].send.tor;
    }
    static int i = 0;
    if (i >= 4) {
        i = 0;
    }
    MIT_motor_CTRL(&hfdcan1, i + 0x01, 0,0, 0, 0,0);
    MIT_motor_CTRL(&hfdcan2, i + 0x05, 0,0, 0, 0,0);
    MIT_motor_CTRL(&hfdcan3, i + 0x09, 0,0, 0, 0,0);
    i++;
#else
    PD_Send();
    //limit();
    for (int j = 0; j < 12; j++)
    {
        tempFloat[j + 50] = motor[j].send.tor;
    }

    static int i = 0;
    if (i >= 4) {
        i = 0;
    }

#if USE_DYF

    MIT_motor_CTRL(&hfdcan1, i + 0x01, motor[i].send.pos,0, motor[i].send.P, motor[i].send.D,0);
    MIT_motor_CTRL(&hfdcan2, i + 0x05, motor[i + 4].send.pos,0, motor[i + 4].send.P, motor[i + 4].send.D,0);
    MIT_motor_CTRL(&hfdcan3, i + 0x09, motor[i + 8].send.pos,0, motor[i + 8].send.P, motor[i + 8].send.D,0);
#else
    MIT_motor_CTRL(&hfdcan1, i + 0x01, motor[i].send.pos,0, motor[i].send.P, motor[i].send.D,motor[i].send.tor);
    MIT_motor_CTRL(&hfdcan2, i + 0x05, motor[i + 4].send.pos,0, motor[i + 4].send.P, motor[i + 4].send.D,motor[i + 4].send.tor);
    MIT_motor_CTRL(&hfdcan3, i + 0x09, motor[i + 8].send.pos,0, motor[i + 8].send.P, motor[i + 8].send.D,motor[i + 8].send.tor);
#endif
    i++;
#endif
}

void All_motor_enable()
{
    for(int j = 0; j < 50; j++)
    {
        for(int i = 0; i < 4; i++)
        {
            start_motor(&hfdcan1, i+0X01);
            start_motor(&hfdcan2, i+0X05);
            start_motor(&hfdcan3, i+0X09);
            HAL_Delay(10);
        }
        HAL_Delay(10);
    }

}

void motor_init()
{
    for(int i = 0; i < 12; i++)
    {
        motor[i].send.pos = 0;
        motor[i].send.speed = 0;
        motor[i].send.P = 0;
        motor[i].send.D = 0;
        motor[i].send.tor = 0;
    }
}

void PD_Send()
{
    for(int i = 0; i < 12; i++)
    {
#if USE_DYF
        motor[i].send.D = SEND_D;
        motor[i].send.tor = 0;
        motor[i].send.speed = 0;

    #if TEST_MODE
        motor[i].send.P = SEND_P_TEST;
    #else
        if (VofaSlider[0] == 1)
        {
            motor[i].send.P = SEND_P;
            motor[8].send.P = 40;
            motor[11].send.P = 40;
        }
        else
        {
            motor[i].send.P = 5;
        }
    #endif

#else
        motor[i].send.P = 0;
        motor[i].send.D = 0;
        motor[i].send.speed = 0;
        motor[i].send.pos = 0;
        if (fabs(motor[i].send.tor) > T_MAX)
        {
            if (motor[i].send.tor > 0)
            {
                motor[i].send.tor = T_MAX;
            }
            else
            {
                motor[i].send.tor = T_MIN;
            }
        }

#endif
    }
}

void limit(void)
{
    for(int i = 1; i < 13; i++)
    {
        switch (i)
        {
        case CAN_DM_M1_ID:
        case CAN_DM_M4_ID:
        case CAN_DM_M7_ID:
        case CAN_DM_M10_ID:
            if (fabs(motor[i - 1].receive.pos) > 0.5f)
            {
                motor[i - 1].send.tor = 0;
                motor[i - 1].send.P = 0;
                motor[i - 1].send.D = 0;
            }
            break;

        case CAN_DM_M2_ID:
        case CAN_DM_M5_ID:
        case CAN_DM_M8_ID:
        case CAN_DM_M11_ID:
            if (fabs(motor[i - 1].receive.pos) > 2.5f)
            {
                motor[i - 1].send.tor = 0;
                motor[i - 1].send.P = 0;
                motor[i - 1].send.D = 0;
            }
            break;
            break;

        case CAN_DM_M3_ID:
        case CAN_DM_M6_ID:
        case CAN_DM_M9_ID:
        case CAN_DM_M12_ID:
            if (fabs(motor[i - 1].receive.pos) < 0.4f)
            {
                motor[i - 1].send.tor = 0;
                motor[i - 1].send.P = 0;
                motor[i - 1].send.D = 0;
            }
            break;

        default:break;
        }
    }
}