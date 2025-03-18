//
// Created by 27713 on 25-2-20.
//

#include "Main.h"
#include "main.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "FDCAN_receive_dm.h"
#include "UART_DMA.h"
#include "IMU_N300WP.h"
#include "tim.h"
#include "vofa_setting.h"
#include "bsp_mc02/can_bsp.h"


extern "C"
void MX_FREERTOS_Init(void);
void Usart_DMA_init();


void Main()
{
   // HAL_Delay(1000)
    Usart_DMA_init();
    HAL_TIM_Base_Start_IT(&htim6);                                //START_2ms_CYCLE

    /* Init scheduler */
    osKernelInitialize();

    /* Call init function for freertos objects (in freertos.c) */
    MX_FREERTOS_Init();

    /* Start scheduler */
    osKernelStart();

    while(1)
    {

    }
}

void Usart_DMA_init()
{
    __HAL_UART_CLEAR_IDLEFLAG(&huart10);
    __HAL_UART_ENABLE_IT(&huart10,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart10,buffer_receive_10,100);

    // __HAL_UART_CLEAR_IDLEFLAG(&huart1);
    // __HAL_UART_ENABLE_IT(&huart1,UART_IT_IDLE);
    // HAL_UART_Receive_DMA(&huart1,buffer_receive_1,100);
}