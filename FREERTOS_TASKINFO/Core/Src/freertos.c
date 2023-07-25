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

/* USER CODE END Variables */
/* Definitions for Task_ADC */
osThreadId_t Task_ADCHandle;
const osThreadAttr_t Task_ADC_attributes = {
  .name = "Task_ADC",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Task_Info */
osThreadId_t Task_InfoHandle;
const osThreadAttr_t Task_Info_attributes = {
  .name = "Task_Info",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void AppTask_ADC(void *argument);
void AppTask_Info(void *argument);

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
  /* creation of Task_ADC */
  Task_ADCHandle = osThreadNew(AppTask_ADC, NULL, &Task_ADC_attributes);

  /* creation of Task_Info */
  Task_InfoHandle = osThreadNew(AppTask_Info, NULL, &Task_Info_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_AppTask_ADC */
/**
  * @brief  Function implementing the Task_ADC thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_AppTask_ADC */
void AppTask_ADC(void *argument)
{
  /* USER CODE BEGIN AppTask_ADC */
	LCD_ShowString(30,0,300,12,12,"Task_ADC: ADC by polling");
	LCD_ShowString(30,15,300,12,12,"ADC Value(mA)=");

	TickType_t previousWakeTime=xTaskGetTickCount();	//获取�?嗒信号计数�??
  /* Infinite loop */
  for(;;)
  {
		HAL_ADC_Start(&hadc1);
		if (HAL_ADC_PollForConversion(&hadc1,100)==HAL_OK)   //轮询方式等待转换完成
		{
			uint32_t  val=HAL_ADC_GetValue(&hadc1);	//读取ADC转换原始数据
			uint32_t  Volt=3300*val;	//mV
			Volt=Volt>>12;				//除以2^12
			LCD_ShowNum(200,15,Volt,4,12);
			//LCD_ShowUint(ADCX,40+LCD_SP10,Volt);	//转换为mV显示
		}
		vTaskDelayUntil(&previousWakeTime, pdMS_TO_TICKS(500));
    //osDelay(1);
  }
  /* USER CODE END AppTask_ADC */
}

/* USER CODE BEGIN Header_AppTask_Info */
/**
* @brief Function implementing the Task_Info thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_AppTask_Info */
void AppTask_Info(void *argument)
{
  /* USER CODE BEGIN AppTask_Info */
	int hwm;

	//====获取单个任务的信�?=====
		//	TaskHandle_t taskHandle=xTaskGetCurrentTaskHandle();	//获取当前任务句柄
		//	TaskHandle_t taskHandle=xTaskGetIdleTaskHandle();		//获取空闲任务句柄
		//	TaskHandle_t taskHandle=xTaskGetHandle("Task_ADC");		//通过任务名称获取任务句柄
	TaskHandle_t taskHandle=Task_ADCHandle;		//直接使用任务句柄变量

	TaskStatus_t taskInfo;
	BaseType_t getFreeStackSpace=pdTRUE;  	//是否获取高水位�??,
	eTaskState taskState=eInvalid; 			//当前的状�?
	vTaskGetInfo(taskHandle, &taskInfo, getFreeStackSpace, taskState);	//获取任务信息

	taskENTER_CRITICAL();		//�?始临界代码段，不允许任务调度

	POINT_COLOR=RED;
	LCD_ShowString(30,30,300,12,12,"Task_Info: Show task info");
	LCD_ShowString(30,45,300,12,12,"Get by vTaskGetInfo() ");
	LCD_ShowString(30,60,300,12,12,"Task Name= ");
	LCD_ShowString(150,60,300,12,12,taskInfo.pcTaskName);
	LCD_ShowString(30,75,300,12,12,"Task Number=  ");
	LCD_ShowString(180,75,300,12,12,taskInfo.xTaskNumber);
	LCD_ShowString(30,90,300,12,12,"Task State= ");
	//LCD_ShowString(180,90,300,12,12,taskInfo.eCurrentState);
	LCD_ShowNum(180,90,taskInfo.eCurrentState,4,12);
	LCD_ShowString(30,105,300,12,12,"Task Priority= ");
	LCD_ShowString(180,105,300,12,12,taskInfo.uxCurrentPriority);
	LCD_ShowString(30,130,300,12,12,"High Water Mark= ");
	LCD_ShowNum(250,130,taskInfo.usStackHighWaterMark,4,12);

//		LcdFRONT_COLOR=lcdColor_WHITE;
//		LCD_ShowStr(10, 100, (uint8_t *)"Task_Info: Show task info");
//		LCD_ShowStr(20, LCD_CurY+LCD_SP10, (uint8_t *)"Get by vTaskGetInfo() ");
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task Name= ");
//		LCD_ShowStr(LCD_CurX+10, LCD_CurY, (uint8_t *)taskInfo.pcTaskName);
//

//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task Number= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, taskInfo.xTaskNumber);
//
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task State= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, taskInfo.eCurrentState);
//
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task Priority= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, taskInfo.uxCurrentPriority);
//
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"High Water Mark= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, taskInfo.usStackHighWaterMark);

	//======用函数uxTaskGetStackHighWaterMark()单独获取每个任务的高水位�?=====
	POINT_COLOR=YELLOW;
	LCD_ShowString(30,145,300,12,12,"High Water Mark of tasks");

	taskHandle=xTaskGetIdleTaskHandle();	//获取空闲任务句柄
	hwm=uxTaskGetStackHighWaterMark(taskHandle);
	LCD_ShowString(30,145,300,12,12,"Idle Task= ");
	LCD_ShowNum(200,145,hwm,4,12);
	taskHandle=Task_ADCHandle;				//Task_ADC的任务句�?
	hwm=uxTaskGetStackHighWaterMark(taskHandle);
	LCD_ShowString(30,160,300,12,12,"Task_ADC= ");
	LCD_ShowNum(200,160,hwm,4,12);
	taskHandle=Task_InfoHandle;				//Task_Info的任务句�?
	hwm=uxTaskGetStackHighWaterMark(taskHandle);
	LCD_ShowString(30,175,300,12,12,"Task_Info= ");
	LCD_ShowNum(200,175,hwm,4,12);
//		LcdFRONT_COLOR=lcdColor_YELLOW;
//		LCD_ShowStr(20, LCD_CurY+LCD_SP15, (uint8_t *)"High Water Mark of tasks");
//
//		taskHandle=xTaskGetIdleTaskHandle();	//获取空闲任务句柄
//		UBaseType_t  hwm=uxTaskGetStackHighWaterMark(taskHandle);
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Idle Task= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, hwm);
//
//		taskHandle=Task_ADCHandle;				//Task_ADC的任务句�?
//		hwm=uxTaskGetStackHighWaterMark(taskHandle);
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task_ADC= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, hwm);
//
//		taskHandle=Task_InfoHandle;				//Task_Info的任务句�?
//		hwm=uxTaskGetStackHighWaterMark(taskHandle);
//		LCD_ShowStr(30, LCD_CurY+LCD_SP10, (uint8_t *)"Task_Info= ");
//		LCD_ShowUint(LCD_CurX+10, LCD_CurY, hwm);

	//=======获取内核的信�?==========
	POINT_COLOR=GREEN;
	LCD_ShowString(30,190,300,12,12,"Kernel Info ");
	int taskNum=uxTaskGetNumberOfTasks();  //获取系统任务个数
	LCD_ShowString(30,205,300,12,12,"uxTaskGetNumberOfTasks()= ");
	LCD_ShowNum(30,230,taskNum,4,12);

//		LcdFRONT_COLOR=lcdColor_GREEN;
//		LCD_ShowStr(10, LCD_CurY+LCD_SP15, (uint8_t *)"Kernel Info ");
//
//		UBaseType_t  taskNum=uxTaskGetNumberOfTasks();  //获取系统任务个数
//		LCD_ShowStr(20, LCD_CurY+LCD_SP10, (uint8_t *)"uxTaskGetNumberOfTasks()= ");
//		LCD_ShowUint(LCD_CurX, LCD_CurY, taskNum);
//
//		uint16_t lastRow=LCD_CurY;
		taskEXIT_CRITICAL();		//结束临界代码段，重新允许任务调度

  /* Infinite loop */
	int loopCount=0;
	for(;;)
	{
		loopCount++;
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		vTaskDelay(pdMS_TO_TICKS(300));
		if(loopCount >= 10)
			break;
		//osDelay(1);
	}
	POINT_COLOR=RED;
	LCD_ShowString(30,230,300,12,12,"Task_Info is deleted");
	vTaskDelete(NULL);	//删除任务自己
  /* USER CODE END AppTask_Info */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

