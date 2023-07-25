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
#include "lcd.h"
#include "adc.h"
#include "gpio.h"
#include "queue.h"
#include "semphr.h"
#include "event_groups.h" //事件组相关头文件
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BITMASK_KEY_LEFT 0x04 //事件位掩码定义
#define BITMASK_KEY_DOWN 0x02
#define BITMASK_KEY_RIGHT 0x01
#define BITMASK_SYNC BITMASK_KEY_LEFT | BITMASK_KEY_DOWN | BITMASK_KEY_RIGHT
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t adc_value;
EventGroupHandle_t eventGroupHandle; //事件组对象句�?
/* USER CODE END Variables */
/* Definitions for Task_LCD */
osThreadId_t Task_LCDHandle;
const osThreadAttr_t Task_LCD_attributes = {
  .name = "Task_LCD",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED1 */
osThreadId_t Task_LED1Handle;
const osThreadAttr_t Task_LED1_attributes = {
  .name = "Task_LED1",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED2 */
osThreadId_t Task_LED2Handle;
const osThreadAttr_t Task_LED2_attributes = {
  .name = "Task_LED2",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Queue_Keys */
osMessageQueueId_t Queue_KeysHandle;
const osMessageQueueAttr_t Queue_Keys_attributes = {
  .name = "Queue_Keys"
};
/* Definitions for BinSem_DataReady */
osSemaphoreId_t BinSem_DataReadyHandle;
const osSemaphoreAttr_t BinSem_DataReady_attributes = {
  .name = "BinSem_DataReady"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_LCD(void *argument);
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

  /* Create the semaphores(s) */
  /* creation of BinSem_DataReady */
  BinSem_DataReadyHandle = osSemaphoreNew(1, 1, &BinSem_DataReady_attributes);

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_Keys */
  Queue_KeysHandle = osMessageQueueNew (10, sizeof(uint8_t), &Queue_Keys_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  eventGroupHandle= xEventGroupCreate(); //创建事件�?
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_LCD */
  Task_LCDHandle = osThreadNew(AppTask_LCD, NULL, &Task_LCD_attributes);

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

/* USER CODE BEGIN Header_AppTask_LCD */
/**
  * @brief  Function implementing the Task_LCD thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_LCD */
void AppTask_LCD(void *argument)
{
  /* USER CODE BEGIN AppTask_LCD */
  /* Infinite loop */
	for(;;)
	{
		if(KEY0 == KEY_DOWN)
		{
			break;
		}
	}
	LCD_ShowString(30,55,210,12,12,"Task_LCD reaches sync point");
	xEventGroupSync(eventGroupHandle, BITMASK_KEY_LEFT, BITMASK_SYNC, portMAX_DELAY);
	for(;;)
	{
		for(uint8_t i=0; i<10; i++)
		{
			LCD_ShowNum(30,100,i,4,12);
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LCD */
}

/* USER CODE BEGIN Header_AppTask_LED1 */
/**
* @brief Function implementing the Task_LED1 thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED1 */
void AppTask_LED1(void *argument)
{
  /* USER CODE BEGIN AppTask_LED1 */
  /* Infinite loop */
	for(;;)
	{
		if(KEY1 == KEY_DOWN)
		{
			break;
		}
	}
	LCD_ShowString(30,70,210,12,12,"Task_LED1 reaches sync point");
	xEventGroupSync(eventGroupHandle, BITMASK_KEY_RIGHT, BITMASK_SYNC, portMAX_DELAY);
	for(;;)
	{
		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
		vTaskDelay(pdMS_TO_TICKS(300));
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
  /* Infinite loop */
	for(;;)
	{
		if(WK_UP == KEY_DOWN)
		{
			break;
		}
	}
	LCD_ShowString(30,85,210,12,12,"Task_LED2 reaches sync point");
	xEventGroupSync(eventGroupHandle, BITMASK_KEY_DOWN, BITMASK_SYNC, portMAX_DELAY);
	for(;;)
	{
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		vTaskDelay(pdMS_TO_TICKS(600));
	}
  /* USER CODE END AppTask_LED2 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

