//
// Created by zcy on 2024/3/25.
//

#include "main.h"
#include "usart.h"
#include "UART_DMA.h"
#include "string.h"
#include "vofa.h"
#include "Cpp/IMU_N300WP.h"
#include "Cpp/Upper_Visual.h"
//#include "task.h"

//_Visual test;
/*缓存数组预定义*/
uint8_t buffer_receive_1[buffer_receive_length_1];
// uint8_t buffer_receive_2[buffer_receive_1_length];
// uint8_t buffer_receive_3[buffer_receive_1_length];
// uint8_t buffer_receive_4[buffer_receive_1_length];
// uint8_t buffer_receive_5[buffer_receive_1_length];
// uint8_t buffer_receive_6[buffer_receive_length_6];
uint8_t buffer_receive_7[buffer_receive_length_7];
// uint8_t buffer_receive_8[buffer_receive_length_8];
// uint8_t buffer_receive_9[buffer_receive_length_9];
uint8_t buffer_receive_10[buffer_receive_length_10];

IMU_N300WP IMU;
extern Upper_data_receive Vdata_Rx;
extern Upper_data_send Vdata_Tx;
/**
  * @brief          UART1-6中断接收服务函数
  * @param[in]      接收数组
  * @param[in]      长度值【0-128】
  * @retval         none
  */
static void UART1_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART2_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART3_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART4_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART5_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART6_Receive_Serve(uint8_t *buffer, uint8_t length);
static void UART7_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART8_Receive_Serve(uint8_t *buffer, uint8_t length);
// static void UART9_Receive_Serve(uint8_t *buffer, uint8_t length);
static void UART10_Receive_Serve(uint8_t *buffer, uint8_t length);

void IMU_Receive_Serve(uint8_t *buffer,uint8_t length)
{
    for(uint8_t i = 0; i < length; i++)
    {
        if((buffer[i] == 0xFC) && (buffer[i+1] == TYPE_AHRS) && (buffer[i + 55] == 0xFD))
        {
            IMU.ahrs_flag = 1;
            for(int j = 0; j < AHRS_LEN; j++)
            {
                IMU.Fd_data[j] = buffer[i+j];
            }
            IMU.data_get(IMU.Fd_data);
        }

        if((buffer[i] == 0xFC) && (buffer[i+1] == TYPE_IMU)  && (buffer[i + 63] == 0xFD))
        {
            IMU.imu_flag = 1;
            for(int j = 0; j < IMU_LEN; j++)
            {
                IMU.Fd_data[j] = buffer[i+j];
            }
            IMU.data_get(IMU.Fd_data);
        }
    }
}


/**
  * @brief          初始化串口DMA接收
  * @param[in]      UART接口
  * @param[in]      缓存数组->推荐使用已定义数组
  * @param[in]      长度  【1-128】
  * @retval         none
  */
void UART_DMA_Receive_init(UART_HandleTypeDef *usart, uint8_t *buffer, uint8_t length)
{
    __HAL_UART_ENABLE_IT(usart,UART_IT_IDLE);
    HAL_UART_Receive_DMA(usart,buffer,length);//打开DMA接收
}
/**
  * @brief          串口DMA接收中断函数->放入《USER CODE BEGIN USARTX_IRQn 1》 中
  * @param[in]      UART接口
  * @param[in]      UART DMA接口
  * @param[in]      缓存数组->推荐使用已定义数组
  * @param[in]      长度  【1-128】
  * @retval         none
  */
void UART_DMA_Receive_IT(UART_HandleTypeDef *usart, DMA_HandleTypeDef *DMA, uint8_t *buffer, uint16_t length)
{
    if(usart->ReceptionType == HAL_UART_RECEPTION_STANDARD)
    {
        __HAL_UART_CLEAR_IDLEFLAG(usart);
        HAL_UART_DMAStop(usart);
        uint8_t real_length = length - __HAL_DMA_GET_COUNTER(DMA);

            if(usart->Instance == USART1)     UART1_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart2) UART2_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart3) UART3_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart4) UART4_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart5) UART5_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart6) UART6_Receive_Serve(buffer, real_length);//选择解码程序
            else if(usart->Instance == UART7) UART7_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart8) UART8_Receive_Serve(buffer, real_length);//选择解码程序
            //else if(usart == &huart9) UART9_Receive_Serve(buffer, real_length);//选择解码程序
            else if(usart->Instance == USART10) UART10_Receive_Serve(buffer, real_length);//选择解码程序
            memset(buffer,0,real_length);
            HAL_UART_Receive_DMA(usart, buffer, length);
        }


}
/**
  * @brief          串口异常的处理
  * @param[in]      UART接口
  * @retval         none
  */
void HAL_UART_ErrorCallback (UART_HandleTypeDef *huart)
{
    uint32_t data;
    __HAL_UNLOCK(huart);

    if(__HAL_UART_GET_FLAG(huart, UART_FLAG_ORE) != RESET)
    {
        __HAL_UART_CLEAR_FLAG(huart, UART_FLAG_ORE);      		//清除溢出中断
        data = huart->Instance->ISR;
        data = huart->Instance->RDR;
        __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    }
}




//UART1中断接收函数
static void UART1_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    Vofa_UART_Receive(buffer,length);
}
//UART2中断接收函数
static void UART2_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    //HAL_UART_Transmit(&huart2,buffer,length,0xff);
}
//UART3中断接收函数
static void UART3_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    //sbus_handler(buffer, length);
}
//UART4中断接收函数
static void UART4_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    //sbus_handler(buffer, length);

}
//UART5中断接收函数
static void UART5_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    //sbus_handler(buffer, length);
}
//UART6中断接收函数
static void UART6_Receive_Serve(uint8_t *buffer, uint8_t length)
{

    //HAL_UART_Transmit(&huart6,buffer,length,0xff);
}

//UART7中断接收函数
static void UART7_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    Vdata_Rx.Vdata_get(buffer,length);
}
//UART8中断接收函数
static void UART8_Receive_Serve(uint8_t *buffer, uint8_t length)
{

    //HAL_UART_Transmit(&huart6,buffer,length,0xff);
}
//UART9中断接收函数
static void UART9_Receive_Serve(uint8_t *buffer, uint8_t length)
{

    //HAL_UART_Transmit(&huart6,buffer,length,0xff);
}
//UART10中断接收函数
static void UART10_Receive_Serve(uint8_t *buffer, uint8_t length)
{
    IMU_Receive_Serve(buffer,length);
    Vdata_Tx.Visual_imu[0] = IMU.Qw;
    Vdata_Tx.Visual_imu[1] = IMU.Qx;
    Vdata_Tx.Visual_imu[2] = IMU.Qy;
    Vdata_Tx.Visual_imu[3] = IMU.Qz;
    Vdata_Tx.Visual_imu[4] = IMU.PitchSpeed;
    Vdata_Tx.Visual_imu[5] = IMU.PitchSpeed;
    Vdata_Tx.Visual_imu[6] = IMU.YawSpeed;
    Vdata_Tx.Visual_imu[7] = IMU.X_Accelerometer;
    Vdata_Tx.Visual_imu[8] = IMU.Y_Accelerometer;
    Vdata_Tx.Visual_imu[9] = IMU.Z_Accelerometer + 9.8;
}

