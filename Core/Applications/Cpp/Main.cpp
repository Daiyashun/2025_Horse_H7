//
// Created by 27713 on 25-2-20.
//

#include "Main.h"
#include "main.h"
#include "cmsis_os.h"
#include "UART_DMA.h"

extern "C"
void MX_FREERTOS_Init(void);

void Main()
{
   // HAL_Delay(1000);
    __HAL_UART_CLEAR_IDLEFLAG(&huart10);
    __HAL_UART_ENABLE_IT(&huart10,UART_IT_IDLE);
    HAL_UART_Receive_DMA(&huart10,buffer_receive_10,100);
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
