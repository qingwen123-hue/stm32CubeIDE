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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
const uint8_t KeyCodeLeft =0x01; //按键代码KeyLeft
const uint8_t KeyCodeRight =0x02; //按键代码KeyRight
//const uint8_t KeyCodeUp =0x03; //按键代码KeyUp
const uint8_t KeyCodeDown =0x04; //按键代码KeyDown

uint16_t curScreenX=100; //LCD当前X
uint16_t curScreenY=260; //LCD当前Y
uint16_t lastScreenX=100; // LCD前一步的X
uint16_t lastScreenY=260; // LCD前一步的Y
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Task_Draw */
osThreadId_t Task_DrawHandle;
const osThreadAttr_t Task_Draw_attributes = {
  .name = "Task_Draw",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for Task_ScanKeys */
osThreadId_t Task_ScanKeysHandle;
const osThreadAttr_t Task_ScanKeys_attributes = {
  .name = "Task_ScanKeys",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Queue_Keys */
osMessageQueueId_t Queue_KeysHandle;
const osMessageQueueAttr_t Queue_Keys_attributes = {
  .name = "Queue_Keys"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_Draw(void *argument);
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
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Task_Draw */
  Task_DrawHandle = osThreadNew(AppTask_Draw, NULL, &Task_Draw_attributes);

  /* creation of Task_ScanKeys */
  Task_ScanKeysHandle = osThreadNew(AppTask_ScanKeys, NULL, &Task_ScanKeys_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_Draw */
/**
  * @brief  Function implementing the Task_Draw thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_Draw */
void AppTask_Draw(void *argument)
{
  /* USER CODE BEGIN AppTask_Draw */
	TickType_t ticks1=pdMS_TO_TICKS(1000); //时间（ms）转换为节拍�?
	TickType_t previousWakeTime= xTaskGetTickCount();
	LCD_ShowString(30,5,210,12,12,"Demo:Using a Queue");
	char *aName = pcQueueGetName(Queue_KeysHandle);
	LCD_ShowString(30,20,210,12,12,"Queue Name =");
	LCD_ShowString(150,20,210,12,12,aName);

	UBaseType_t qSpces = uxQueueSpacesAvailable(Queue_KeysHandle);
	LCD_ShowString(30,35,210,12,12,"Queue Size = ");
	LCD_ShowNum(100,35,qSpces,4,12);
	LCD_ShowString(30,50,210,12,12,"uxQueueMessagesWaiting()= ");
	LCD_ShowString(30,65,210,12,12,"uxQueueSpacesAvailable()= ");

	//LCD_ShowString(30,20,210,12,12,"Queue Name =");
	curScreenX = 30;
	curScreenY = 80;
	lastScreenX = curScreenX;
	lastScreenY = curScreenY;
	int msgCount = 0,freeSpace = 0;
	uint8_t keyCode;
  /* Infinite loop */
	for(;;)
	{
		msgCount=uxQueueMessagesWaiting(Queue_KeysHandle);  //等待读取的消息个数
		LCD_ShowNum(150,50,msgCount,4,12);
		freeSpace=uxQueueSpacesAvailable(Queue_KeysHandle); //剩余空间个数
		LCD_ShowNum(150,65,freeSpace,4,12);

		//读取消息, 阻塞式等待
		BaseType_t  result=xQueueReceive(Queue_KeysHandle, &keyCode, pdMS_TO_TICKS(50));
		if (result != pdTRUE)
			continue;

		if(keyCode == KeyCodeDown)
		{
			lastScreenY += 25;
		}
		else if(keyCode == KeyCodeLeft)
		{
			lastScreenX -= 20;
		}
		else if(keyCode == KeyCodeRight)
		{
			lastScreenX += 20;
		}

		LCD_DrawLine(curScreenX, curScreenY, lastScreenX, lastScreenY);
		curScreenX = lastScreenX;
		curScreenY = lastScreenY;
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		vTaskDelayUntil(&previousWakeTime, ticks1);
	}
  /* USER CODE END AppTask_Draw */
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
	TickType_t ticks1=pdMS_TO_TICKS(100); //时间（ms）转换为节拍�?
	TickType_t previousWakeTime= xTaskGetTickCount();
  /* Infinite loop */
	for(;;)
	{
		if(KEY0 == KEY_DOWN)
		{
			xQueueSendToBack(Queue_KeysHandle,&KeyCodeLeft,pdMS_TO_TICKS(100));
			//vTaskDelayUntil(&previousWakeTime, ticks1);
		}
		else if(KEY1 == KEY_DOWN)
		{
			xQueueSendToBack(Queue_KeysHandle,&KeyCodeRight,pdMS_TO_TICKS(100));
			//vTaskDelayUntil(&previousWakeTime, ticks1);
		}
		else if(WK_UP == KEY_DOWN)
		{
			xQueueSendToBack(Queue_KeysHandle,&KeyCodeDown,pdMS_TO_TICKS(100));
			//vTaskDelayUntil(&previousWakeTime, ticks1);
		}
		vTaskDelayUntil(&previousWakeTime, ticks1);



	}
  /* USER CODE END AppTask_ScanKeys */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

