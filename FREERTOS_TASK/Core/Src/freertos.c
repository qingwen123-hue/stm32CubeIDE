/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef StaticTask_t osStaticThreadDef_t;
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

/* USER CODE END Variables */
/* Definitions for Task_LED1 */
osThreadId_t Task_LED1Handle;
const osThreadAttr_t Task_LED1_attributes = {
  .name = "Task_LED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED2 */
osThreadId_t Task_LED2Handle;
uint32_t Task_LED2Buffer[ 128 ];
osStaticThreadDef_t Task_LED2ControlBlock;
const osThreadAttr_t Task_LED2_attributes = {
  .name = "Task_LED2",
  .cb_mem = &Task_LED2ControlBlock,
  .cb_size = sizeof(Task_LED2ControlBlock),
  .stack_mem = &Task_LED2Buffer[0],
  .stack_size = sizeof(Task_LED2Buffer),
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_LED1(void *argument);
void AppTask_LED2(void *argument);

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
  /* creation of Task_LED1 */
  Task_LED1Handle = osThreadNew(AppTask_LED1, NULL, &Task_LED1_attributes);

  /* creation of Task_LED2 */
  Task_LED2Handle = osThreadNew(AppTask_LED2, NULL, &Task_LED2_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_LED1 */
/**
  * @brief  Function implementing the Task_LED1 thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_LED1 */
void AppTask_LED1(void *argument)
{
  /* USER CODE BEGIN AppTask_LED1 */
	TickType_t ticks1=pdMS_TO_TICKS(1000); //时间（ms）转换为节拍�?
	TickType_t previousWakeTime= xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
	  vTaskDelayUntil(&previousWakeTime, ticks1); //循环周期1000ms
    //osDelay(1);
  }
  /* USER CODE END AppTask_LED1 */
}

/* USER CODE BEGIN Header_AppTask_LED2 */
/**
* @brief Function implementing the Task_LED2 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED2 */
void AppTask_LED2(void *argument)
{
  /* USER CODE BEGIN AppTask_LED2 */
	TickType_t ticks1=pdMS_TO_TICKS(500); //时间（ms）转换为节拍�?
	TickType_t previousWakeTime= xTaskGetTickCount();
  /* Infinite loop */
  for(;;)
  {
	  HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
	  vTaskDelayUntil(&previousWakeTime, ticks1); //循环周期1000m
    //osDelay(1);
  }
  /* USER CODE END AppTask_LED2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

