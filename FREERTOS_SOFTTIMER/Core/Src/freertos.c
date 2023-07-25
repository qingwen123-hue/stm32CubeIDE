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
#include "tim.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define BITMASK_KEY_LEFT 0x04 //事件位掩码定�?????
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
EventGroupHandle_t eventGroupHandle; //事件组对象句�??????
uint32_t counter=0; //计数变量
/* USER CODE END Variables */
/* Definitions for Task_Main */
osThreadId_t Task_MainHandle;
const osThreadAttr_t Task_Main_attributes = {
  .name = "Task_Main",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Queue_Keys */
osMessageQueueId_t Queue_KeysHandle;
const osMessageQueueAttr_t Queue_Keys_attributes = {
  .name = "Queue_Keys"
};
/* Definitions for Timer_Periodic */
osTimerId_t Timer_PeriodicHandle;
const osTimerAttr_t Timer_Periodic_attributes = {
  .name = "Timer_Periodic"
};
/* Definitions for Timer_Once */
osTimerId_t Timer_OnceHandle;
const osTimerAttr_t Timer_Once_attributes = {
  .name = "Timer_Once"
};
/* Definitions for BinSem_DataReady */
osSemaphoreId_t BinSem_DataReadyHandle;
const osSemaphoreAttr_t BinSem_DataReady_attributes = {
  .name = "BinSem_DataReady"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_Main(void *argument);
void AppTimer_Periodic(void *argument);
void AppTimer_Once(void *argument);

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

  /* Create the timer(s) */
  /* creation of Timer_Periodic */
  Timer_PeriodicHandle = osTimerNew(AppTimer_Periodic, osTimerPeriodic, NULL, &Timer_Periodic_attributes);

  /* creation of Timer_Once */
  Timer_OnceHandle = osTimerNew(AppTimer_Once, osTimerOnce, NULL, &Timer_Once_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* Create the queue(s) */
  /* creation of Queue_Keys */
  Queue_KeysHandle = osMessageQueueNew (10, sizeof(uint8_t), &Queue_Keys_attributes);

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  eventGroupHandle= xEventGroupCreate(); //创建事件�??????
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Main */
  Task_MainHandle = osThreadNew(AppTask_Main, NULL, &Task_Main_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_Main */
/**
  * @brief  Function implementing the Task_Main thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_Main */
void AppTask_Main(void *argument)
{
  /* USER CODE BEGIN AppTask_Main */
	xTimerChangePeriod( Timer_PeriodicHandle, pdMS_TO_TICKS(1000), portMAX_DELAY);
	xTimerChangePeriod( Timer_OnceHandle, pdMS_TO_TICKS(5000), portMAX_DELAY);
	xTimerStart( Timer_PeriodicHandle,portMAX_DELAY);
	xTimerStart( Timer_OnceHandle,portMAX_DELAY);
  /* Infinite loop */
	for(;;)
	{
		if(KEY0 == KEY_DOWN)
		{
			counter = 0;
			if(xTimerIsTimerActive(Timer_OnceHandle) == pdFALSE)
			{
				HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_RESET);
				xTimerReset(Timer_OnceHandle,portMAX_DELAY);
			}
		}
		vTaskDelay(10);
	}
  /* USER CODE END AppTask_Main */
}

/* AppTimer_Periodic function */
void AppTimer_Periodic(void *argument)
{
  /* USER CODE BEGIN AppTimer_Periodic */
	counter++;
	LCD_ShowNum(150,60,counter,4,12);
	HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  /* USER CODE END AppTimer_Periodic */
}

/* AppTimer_Once function */
void AppTimer_Once(void *argument)
{
  /* USER CODE BEGIN AppTimer_Once */
	HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin,GPIO_PIN_SET);
  /* USER CODE END AppTimer_Once */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//void FreeRtosHardwareTimeISR(TIM_HandleTypeDef *htim)
//{
//	if(htim->Instance == TIM3)
//	{
//		HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
//		BaseType_t taskWoken = pdFALSE;
//		vTaskNotifyGiveFromISR(Task_ChecklnHandle, &taskWoken);
//		portYIELD_FROM_ISR(taskWoken);
//	}
//}
//void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
//{
//
//}
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	if(hadc->Instance == ADC1)
//	{
//		adc_value = HAL_ADC_GetValue(hadc);
//		if(Task_ShowHandle != NULL)
//		{
//			BaseType_t taskWoken = pdFALSE;
//			//xSemaphoreGiveFromISR(BinSem_DataReadyHandle,&highTaskWoken);
//			xTaskNotifyFromISR(Task_ShowHandle,adc_value,eSetValueWithOverwrite,&taskWoken);
//			portYIELD_FROM_ISR(taskWoken);
//		}
//	}
//}
/* USER CODE END Application */

