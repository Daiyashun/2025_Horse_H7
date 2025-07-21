/**
  ****************************(C) COPYRIGHT 2019 DJI****************************
  * @file       can_receive.c/h
  * @brief      there is CAN interrupt function  to receive motor data,
  *             and CAN send function to send motor current to control motor.
  *             这里是CAN中断接收函数，接收电机数据,CAN发送函数发送电机电流控制电机.


  ==============================================================================
  @endverbatim
  ****************************(C) COPYRIGHT 2019 DJI****************************
  */

#ifndef CAN_RECEIVE_DM_H
#define CAN_RECEIVE_DM_H


#ifdef __cplusplus
extern "C" {
#endif
#include "struct_typedef.h"
#include "main.h"
#include "user_lib.h"
#define CHASSIS_CAN hcan1
#define BUS_CAN     hcan2
/*DM Motor Default 参数*/
#define P_MIN -12.5f
#define P_MAX 12.5f
#define V_MIN -45.0f
#define V_MAX -V_MIN
#define KP_MIN 0.0f
#define KP_MAX 500.0f
#define KD_MIN 0.0f
#define KD_MAX 5.0f
#define T_MIN -20.0f
#define T_MAX 20.0f
/* CAN send and receive ID */
/*接收到的DM电机的参数结构体*/
/*
* ID 表示控制器的 ID，取 CAN_ID 的低 8 位
* ERR 表示故障，对应故障类型为：
* 8——超压；
* 9——欠压；
* A——过电流；
* B——MOS 过温；
* C——电机线圈过温；
* D——通讯丢失；
* E——过载；
* RECEIVE_POS 表示电机的位置信息
* VEL 表示电机的速度信息
* T 表示电机的扭矩信息
* T_MOS 表示驱动上 MOS 的平均温度，单位℃
* T_Rotor 表示电机内部线圈的平均温度，单位℃
*/


//typedef enum {
//    SEND_OK = 0,
//    SEND_BUSY,
//    SEND_ERROR ,
//}mit_StatusTypeDef;

typedef struct{
    uint8_t id;
    uint8_t state;
    int int_p;
    int int_v;
    int int_t;
    float pos;
    float speed;
    float toq;
    float T_mos;
    float T_coil;
}DM_measure_t;
//extern DM_measure_t  DM_Motor_measure[8];

//电机发送结构体
typedef struct {
  //电机结构体
  float speed;          //设置动作速度
  float pos;          //发送电机位置
  float tor;       //扭矩
  float P;                //发送电机P
  float D;                //发送电机D
  bool flag;//发送标志位
} Dm_send_t;

typedef enum
{
    CAN_DM_M1_ID = 0x01,
    CAN_DM_M2_ID = 0x02,
    CAN_DM_M3_ID = 0x03,
    CAN_DM_M4_ID = 0x04,

    CAN_DM_M5_ID = 0x05,
    CAN_DM_M6_ID = 0x06,
    CAN_DM_M7_ID = 0x07,
    CAN_DM_M8_ID = 0x08,

    CAN_DM_M9_ID = 0x09,
    CAN_DM_M10_ID = 0x0A,
    CAN_DM_M11_ID = 0x0B,
    CAN_DM_M12_ID = 0x0C,
} can_msg_id_e;

typedef enum
{
  MASTER_M1_ID = 0x11,
  MASTER_M2_ID = 0x12,
  MASTER_M3_ID = 0x13,
  MASTER_M4_ID = 0x14,
  MASTER_M5_ID = 0x15,
  MASTER_M6_ID = 0x16,
  MASTER_M7_ID = 0x17,
  MASTER_M8_ID = 0x18,
  MASTER_M9_ID = 0x19,
  MASTER_M10_ID = 0x1A,
  MASTER_M11_ID = 0x1B,
  MASTER_M12_ID = 0x1C,
} can_rec_id_e;
/* CAN send and receive ID */

/**
  * @brief          float转int 带限幅
  * @param[in]      x：输入值
  * @param[in]      x_min:最小限幅
  * @param[in]      x_max:最大限幅
  * @param[in]      bits:位
  * @retval         返回值
  */
int float_to_uint(float x, float x_min, float x_max, int bits);
/**
  * @brief          int转float 带限幅
  * @param[in]      x_int：输入值
  * @param[in]      x_min:最小限幅
  * @param[in]      x_max:最大限幅
  * @param[in]      bits:位
  * @retval         返回值
  */
float uint_to_float(int x_int, float x_min, float x_max, int bits);
/**
  * @brief          CAN1和CAN2滤波器配置
  * @param[in]      CAN_number：CAN接口数量
  * @retval         None
  */
void CAN_Filter_Init(uint8_t CAN_number);
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
void MIT_motor_CTRL(FDCAN_HandleTypeDef *hfdcan, uint16_t id, float _pos, float _vel, float _KP, float _KD, float _torq);


/**
  * @brief          返回电机数据指针
  * @param[in]      i: 电机编号,范围[0,3]
  * @retval         电机数据指针
  */
extern const DM_measure_t *get_motor_measure_point(uint8_t i);
/**
  * @brief          使能DM电机
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void start_motor(FDCAN_HandleTypeDef* hfdcan,uint16_t id);
/**
  * @brief          失能DM电机
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void lock_motor(FDCAN_HandleTypeDef* hfdcan,uint16_t id);
/**
  * @brief          设置电机零点
  * @param[in]      hcan: CAN接口
  * @param[in]      id:  DM电机ID（通过上位机设置）
  * @retval         none
  */
void set_zero_motor(FDCAN_HandleTypeDef *hfdcan,uint16_t id);

void get_moto_measure(DM_measure_t *ptr, uint8_t *Data);

void mit_send_in_TIM(void);

void All_motor_enable(void);

void motor_init(void);

typedef struct
{
    Dm_send_t send;       //发送结构体
    DM_measure_t receive; //接收结构体
    first_order_filter_type_t first_order_control_pos;   //电机角度一阶低通滤波
}motor_t;

// 定义电机配置结构
typedef struct {
  int id;
  float position;
}MotorConfig;

void PD_Send();
void limit(void);
#ifdef __cplusplus
}
#endif


#endif
