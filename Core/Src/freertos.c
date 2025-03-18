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
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for UART_TX */
osThreadId_t UART_TXHandle;
const osThreadAttr_t UART_TX_attributes = {
  .name = "UART_TX",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for FDCAN */
osThreadId_t FDCANHandle;
const osThreadAttr_t FDCAN_attributes = {
  .name = "FDCAN",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void UART_TX_task(void *argument);
void FDCAN_Task(void *argument);

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of UART_TX */
  UART_TXHandle = osThreadNew(UART_TX_task, NULL, &UART_TX_attributes);

  /* creation of FDCAN */
  FDCANHandle = osThreadNew(FDCAN_Task, NULL, &FDCAN_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  /* Infinite loop */
  for(;;)
  {
    osDelay(1);
  }
  /* USER CODE END StartDefaultTask */
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
  // UART_DMA_Receive_init(&huart1, buffer_receive_1, buffer_receive_length_7);
  // UART_DMA_Receive_init(&huart7, buffer_receive_7, buffer_receive_length_7);
  UART_DMA_Receive_init(&huart10, buffer_receive_10, buffer_receive_length_10);
  /* Infinite loop */
  for(;;)
  {
    // for(int i = 0; i < 50; i++)
    // {
    //   tempFloat[i] = 0.5 + i;
    // }
    Vofa_Transmit(&huart1,51);
    //HAL_UART_Transmit_DMA(&huart1, (uint8_t *)tempFloat, 56 * 4);
    osDelay(10);
  }
  /* USER CODE END UART_TX_task */
}

/* USER CODE BEGIN Header_FDCAN_Task */
/**
* @brief Function implementing the FDCAN thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_FDCAN_Task */
void FDCAN_Task(void *argument)
{
  /* USER CODE BEGIN FDCAN_Task */
  //start_motor(&hfdcan1, 0X01);
  can_bsp_init();
  All_motor_enable();
  motor_init();
  /* Infinite loop */
  for(;;)
  {
    mit_send_in_TIM();
    osDelay(10);
  }
  /* USER CODE END FDCAN_Task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

