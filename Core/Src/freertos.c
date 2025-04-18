/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "fdcan.h"
#include "FDCAN_receive_dm.h"
#include "UART_DMA.h"
#include "vofa.h"
#include "bsp_mc02/can_bsp.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern motor_t motor[12];
/* USER CODE END Variables */
/* Definitions for Vofa_Receive */
osThreadId_t Vofa_ReceiveHandle;
const osThreadAttr_t Vofa_Receive_attributes = {
  .name = "Vofa_Receive",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UART_TX */
osThreadId_t UART_TXHandle;
const osThreadAttr_t UART_TX_attributes = {
  .name = "UART_TX",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void VofaReceiveTask(void *argument);
void UART_TX_task(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Vofa_Receive */
  Vofa_ReceiveHandle = osThreadNew(VofaReceiveTask, NULL, &Vofa_Receive_attributes);

  /* creation of UART_TX */
  UART_TXHandle = osThreadNew(UART_TX_task, NULL, &UART_TX_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_VofaReceiveTask */
/**
  * @brief  Function implementing the Vofa_Receive thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_VofaReceiveTask */
__weak void VofaReceiveTask(void *argument)
{
  /* USER CODE BEGIN VofaReceiveTask */
  /* Infinite loop */
  for(;;)
  {
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
__weak void UART_TX_task(void *argument)
{
  /* USER CODE BEGIN UART_TX_task */


  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END UART_TX_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

