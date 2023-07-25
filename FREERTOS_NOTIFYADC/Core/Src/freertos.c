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
#define BITMASK_KEY_LEFT 0x04 //事件位掩码定�?
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
EventGroupHandle_t eventGroupHandle; //事件组对象句�??
/* USER CODE END Variables */
/* Definitions for Task_Show */
osThreadId_t Task_ShowHandle;
const osThreadAttr_t Task_Show_attributes = {
  .name = "Task_Show",
  .stack_size = 256 * 4,
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

void AppTask_Show(void *argument);

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
  eventGroupHandle= xEventGroupCreate(); //创建事件�??
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Show */
  Task_ShowHandle = osThreadNew(AppTask_Show, NULL, &Task_Show_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_Show */
/**
  * @brief  Function implementing the Task_Show thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_Show */
void AppTask_Show(void *argument)
{
  /* USER CODE BEGIN AppTask_Show */
	LCD_ShowString(30,55,210,12,12,"ADC Value = ");
	LCD_ShowString(30,70,210,12,12,"Voltage(mV) = ");
  /* Infinite loop */
	uint32_t notifyValue = 0;
	for(;;)
	{
		BaseType_t result = xTaskNotifyWait(0x00, 0xffffffffUL, &notifyValue, portMAX_DELAY);
		if(result == pdTRUE)
		{
			uint32_t tmpValue = notifyValue;
			LCD_ShowNum(150,55,tmpValue,4,12);
			uint32_t Volt = 3300 * tmpValue;
			Volt = Volt >> 12;
			LCD_ShowNum(150,70,Volt,4,12);
		}
	}
  /* USER CODE END AppTask_Show */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if(hadc->Instance == ADC1)
	{
		adc_value = HAL_ADC_GetValue(hadc);
		if(Task_ShowHandle != NULL)
		{
			BaseType_t taskWoken = pdFALSE;
			//xSemaphoreGiveFromISR(BinSem_DataReadyHandle,&highTaskWoken);
			xTaskNotifyFromISR(Task_ShowHandle,adc_value,eSetValueWithOverwrite,&taskWoken);
			portYIELD_FROM_ISR(taskWoken);
		}
	}
}
/* USER CODE END Application */

