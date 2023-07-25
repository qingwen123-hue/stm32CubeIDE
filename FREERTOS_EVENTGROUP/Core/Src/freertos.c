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
#define BITMASK_KEY_LEFT 0x04 //KeyLeft的事件位掩码，使用Bit2位
#define BITMASK_KEY_RIGHT 0x01 //KeyRight的事件位掩码，使用Bit0位
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
uint32_t adc_value;
EventGroupHandle_t eventGroupHandle; //事件组对象句柄
/* USER CODE END Variables */
/* Definitions for Task_LCD */
osThreadId_t Task_LCDHandle;
const osThreadAttr_t Task_LCD_attributes = {
  .name = "Task_LCD",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_LED */
osThreadId_t Task_LEDHandle;
const osThreadAttr_t Task_LED_attributes = {
  .name = "Task_LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_ScanKeys */
osThreadId_t Task_ScanKeysHandle;
const osThreadAttr_t Task_ScanKeys_attributes = {
  .name = "Task_ScanKeys",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
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
void AppTask_LED(void *argument);
void AppTask_ScanKeys(void *argument);

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
  eventGroupHandle= xEventGroupCreate(); //创建事件组
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_LCD */
  Task_LCDHandle = osThreadNew(AppTask_LCD, NULL, &Task_LCD_attributes);

  /* creation of Task_LED */
  Task_LEDHandle = osThreadNew(AppTask_LED, NULL, &Task_LED_attributes);

  /* creation of Task_ScanKeys */
  Task_ScanKeysHandle = osThreadNew(AppTask_ScanKeys, NULL, &Task_ScanKeys_attributes);

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
	BaseType_t clearOnExit=pdTRUE; // pdTRUE=退出时清除事件位
	BaseType_t waitForAllBits=pdTRUE; //pdTRUE=逻辑与, pdFALSE=逻辑或
	EventBits_t bitsToWait=BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT; //等待的事件位
	for(;;)
	{
		EventBits_t result= xEventGroupWaitBits(eventGroupHandle, bitsToWait,
		clearOnExit, waitForAllBits, portMAX_DELAY );
		for(uint8_t i=0; i<10; i++)
		{
			LCD_ShowNum(30,70,i,4,12);
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LCD */
}

/* USER CODE BEGIN Header_AppTask_LED */
/**
* @brief Function implementing the Task_LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_LED */
void AppTask_LED(void *argument)
{
  /* USER CODE BEGIN AppTask_LED */
  /* Infinite loop */
	BaseType_t clearOnExit=pdTRUE; // pdTRUE=退出时清除事件位
	BaseType_t waitForAllBits=pdTRUE; //pdTRUE=逻辑与, pdFALSE=逻辑或
	EventBits_t bitsToWait=BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT; //等待的事件位
	for(;;)
	{
		EventBits_t result= xEventGroupWaitBits(eventGroupHandle, bitsToWait,
		clearOnExit, waitForAllBits, portMAX_DELAY );
		for(uint8_t i=0; i<10; i++)
		{ //使LED1闪烁几次
			HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
			vTaskDelay(pdMS_TO_TICKS(500));
		}
	}
  /* USER CODE END AppTask_LED */
}

/* USER CODE BEGIN Header_AppTask_ScanKeys */
/**
* @brief Function implementing the Task_ScanKeys thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_ScanKeys */
void AppTask_ScanKeys(void *argument)
{
  /* USER CODE BEGIN AppTask_ScanKeys */
  /* Infinite loop */
	for(;;)
	{
		EventBits_t curBits = xEventGroupGetBits(eventGroupHandle);
		LCD_ShowNum(30,50,curBits,4,12);
		if(KEY0 == KEY_DOWN)
		{
			xEventGroupSetBits(eventGroupHandle, BITMASK_KEY_LEFT);
			vTaskDelay(200);
		}
		else if(KEY1 == KEY_DOWN)
		{
			xEventGroupSetBits(eventGroupHandle, BITMASK_KEY_RIGHT);
			vTaskDelay(200);
		}
		else if(WK_UP == KEY_DOWN)
		{
			xEventGroupClearBits(eventGroupHandle, BITMASK_KEY_LEFT | BITMASK_KEY_RIGHT);
			vTaskDelay(200);
		}
		else
		{
			vTaskDelay(50);
		}
	}
  /* USER CODE END AppTask_ScanKeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
//void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
//{
//	if(hadc->Instance == ADC1)
//	{
//		adc_value = HAL_ADC_GetValue(hadc);
//		BaseType_t highTaskWoken = pdFALSE;
//		if(BinSem_DataReadyHandle != NULL)
//		{
//			xSemaphoreGiveFromISR(BinSem_DataReadyHandle,&highTaskWoken);
//			portYIELD_FROM_ISR(highTaskWoken);
//		}
//	}
//}
/* USER CODE END Application */

