#include "Rtos.h"
#include "cmsis_os2.h"
#include "fdcan.h"
#include "vofa.h"
#include "Cpp/Upper_Visual.h"
Upper_data_receive Vdata_Rx;
Upper_data_send Vdata_Tx;
extern IMU_N300WP IMU;
#ifdef __cplusplus
extern "C" {
#endif
    /*---------------------------- C Scope Start---------------------------*/


void Vofa_to_Motor()
{
    for (int i = 0; i < 8; i++)
    {
        if (VofaButton[i] == 1)
        {
            switch (i)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                set_zero_motor(&hfdcan1,i + 1);
                break;

            case 4:
            case 5:
            case 6:
            case 7:
                set_zero_motor(&hfdcan2,i + 1);
                break;
            }
            // set_zero_motor(&hfdcan1,i + 1);
            // set_zero_motor(&hfdcan2,i + 1);
            // set_zero_motor(&hfdcan3,i + 1);
        }
    }
    for (int i = 8; i < 12; i++)
    {
        if (VofaKey[i - 8] == 1)
        {
            // set_zero_motor(&hfdcan1,i);
            // set_zero_motor(&hfdcan2,i);
            set_zero_motor(&hfdcan3,i + 1);
        }
    }
}
void VofaReceiveTask(void *argument)
    {
        /* USER CODE BEGIN VofaReceiveTask */
        /* Infinite loop */
        for(;;)
        {
            Vofa_to_Motor();
            osDelay(1);
        }
        /* USER CODE END VofaReceiveTask */
    }

    /* USER CODE BEGIN Header_UART_TX_task */
    /**
    * @brief Function implementing the UART_TX thread.
    * @param argument: Not used
    * @retval None
    */
    /* USER CODE END Header_UART_TX_task */
void UART_TX_task(void *argument)
{
    /* USER CODE BEGIN UART_TX_task */

    /* Infinite loop */
    for(;;)
    {
        Vdata_Tx.All_Data_get();
        //All_motor_enable();
        Vdata_Tx.All_Data_send(&huart7);
        Vofa_Transmit(&huart1,51);
        osDelay(1);
    }
    /* USER CODE END UART_TX_task */
}
    /*---------------------------- C Scope End---------------------------*/
#ifdef __cplusplus
}
#endif





