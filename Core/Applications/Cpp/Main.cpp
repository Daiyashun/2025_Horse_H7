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
    HAL_TIM_Base_Start_IT(&htim6);                                //START_2ms_CYCLE

    UART_DMA_Receive_init(&huart1, buffer_receive_1, buffer_receive_length_1);
    UART_DMA_Receive_init(&huart7, buffer_receive_7, buffer_receive_length_7);
    UART_DMA_Receive_init(&huart10, buffer_receive_10, buffer_receive_length_10);

    can_bsp_init();
    All_motor_enable();
    motor_init();
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

void disable_all()
{

}