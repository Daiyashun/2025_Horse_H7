//
// Created by zcy on 2024/3/25.
//

#ifndef INC_2024MASHU_UART_DMA_H
#define INC_2024MASHU_UART_DMA_H



#ifdef __cplusplus
extern "C" {
#endif
#include "main.h"
#include "usart.h"

#define buffer_receive_1_length 50
#define buffer_receive_2_length 50
#define buffer_receive_3_length 50
#define buffer_receive_4_length 50
#define buffer_receive_5_length 50
#define buffer_receive_length_6 50
#define buffer_receive_length_7 100
#define buffer_receive_length_8 50
#define buffer_receive_length_9 50
#define buffer_receive_length_10 100

extern uint8_t buffer_receive_1[buffer_receive_1_length];//缓存数组全局定义
extern uint8_t buffer_receive_2[buffer_receive_1_length];//缓存数组全局定义
extern uint8_t buffer_receive_3[buffer_receive_1_length];//缓存数组全局定义
extern uint8_t buffer_receive_4[buffer_receive_1_length];//缓存数组全局定义
extern uint8_t buffer_receive_5[buffer_receive_1_length];//缓存数组全局定义
extern uint8_t buffer_receive_6[buffer_receive_length_6];//缓存数组全局定义
extern uint8_t buffer_receive_7[buffer_receive_length_7];//缓存数组全局定义
extern uint8_t buffer_receive_8[buffer_receive_length_8];//缓存数组全局定义
extern uint8_t buffer_receive_9[buffer_receive_length_9];//缓存数组全局定义
extern uint8_t buffer_receive_10[buffer_receive_length_10];//缓存数组全局定义



//extern _Visual Test
union Visual
{
  float data[1];
  uint8_t temp[8];
};
/**
  * @brief          初始化串口DMA接收
  * @param[in]      UART接口
  * @param[in]      缓存数组->推荐使用已定义数组
  * @param[in]      长度  【1-128】
  * @retval         none
  */
extern void UART_DMA_Receive_init(UART_HandleTypeDef *usart, uint8_t *buffer, uint8_t length);
/**
  * @brief          串口DMA接收中断函数->放入《USER CODE BEGIN USARTX_IRQn 1》 中
  * @param[in]      UART接口
  * @param[in]      UART DMA接口
  * @param[in]      缓存数组->推荐使用已定义数组
  * @param[in]      长度  【1-128】
  * @retval         none
  */
extern void UART_DMA_Receive_IT(UART_HandleTypeDef *usart, DMA_HandleTypeDef *DMA, uint8_t *buffer, uint8_t length);

#ifdef __cplusplus
}
#endif
#endif //INC_2024MASHU_UART_DMA_H
