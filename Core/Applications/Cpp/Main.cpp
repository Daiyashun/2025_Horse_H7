#include "Main.h"
#include "main.h"
#include "cmsis_os.h"
#include "fdcan.h"
#include "FDCAN_receive_dm.h"
#include "UART_DMA.h"
#include "IMU_N300WP.h"
#include "tim.h"
#include "Upper_Visual.h"
#include "vofa_setting.h"
#include "bsp_mc02/can_bsp.h"

extern "C"
void MX_FREERTOS_Init(void);
void Usart_DMA_init();


extern Upper_data_receive Vdata_Rx;
extern Upper_data_send Vdata_Tx;
void Main()
{
    HAL_TIM_Base_Start_IT(&htim6);
    UART_DMA_Receive_init(&huart1, buffer_receive_1, buffer_receive_length_1);
    UART_DMA_Receive_init(&huart7, buffer_receive_7, buffer_receive_length_7);
    UART_DMA_Receive_init(&huart10, buffer_receive_10, buffer_receive_length_10);

    Vdata_Rx.Visual_Receive_Flag = 0;
    disable_all();
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
    if (HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15) == GPIO_PIN_RESET)
    {
        while (1)
        {

        }
    }
}